The attack
==========
**Target:** *Hash to curve* implementation of WPA3's s Simultaneous Authentication of Equals (SAE) handshake.
**How:** Cache based side-channel attack

## General info
* The goal is to learn if the Quadratic Residue (QR) test
in the first iteration of the hash to curve algorithm succeeded or not.
* The implementation of the
hash to curve algorithm for ECC groups does include mitigations
against side channel attacks. Those mitigations include performing extra dummy iterations on random data
* Code runs on **a user connecting the network**.
* Can be run from unprivileged user-mode process.
* Attacking hostapd's ([git](git://w1.fi/hostap.git) , commit `0eb34f8f2`), function `sae_derive_pwe_ecc` (see last section)
* The attack is Flush-Reload.

## Implementation detail

* We want to monitor the branch inside the `k` fake iterations to find out whether we succeed in the first time: `if (res > 0 && !x) {`.
* However, we need to differentiate between the first time the loop runs and afterward. 
* For that, we monitor another cache line that is accessed once per iteration - `sha256_prf_bits` a "far away" cache line, since monitoring multiple closed cache line cause inference.
* Measuring a trace of the cache access pattern by monitoring the two addresses (the “clock” and the non-QR case) in fixed intervals of 50000 clock cycles (each iteration takes ≈ 200000 clock cycles on our test machine).
* Encode each trace with two bits for the two addresses.
* We only keep active measurements ≠ 0.
* Our attack returns the value of the first two active
measurements, meaning the return value consists of four bits
* Then, machine learning on output to do THE MAGIC.

## Attacked code
```c
static int sae_derive_pwe_ecc(struct sae_data *sae, const u8 *addr1,
    const u8 *addr2, const u8 *password,
    size_t password_len, const char *identifier){
        ...
        if (random_get_bytes(dummy_password, dummy_password_len) < 0)
        return -1;
        ...
        /* Create a random quadratic residue (qr) and quadratic non-residue
        * (qnr) modulo p for blinding purposes during the loop. */
        if (get_random_qr_qnr(prime, prime_len, sae->tmp->prime, bits,
        1 &qr, &qnr) < 0)
        return -1;
        ...
        /* Continue for at least k iterations to protect against  side-channel
         * attacks that attempt to determine the number of iterations required
         * in the loop. */
        for (counter = 1; counter <= k || !x; counter++) {
        ...
        res = sae_test_pwd_seed_ecc(sae, pwd_seed, prime, qr, qnr, &x_cand);
        if (res < 0)
            goto fail;
        if (res > 0 && !x) { // WE CAN MONITOR THIS BRANCH
            ...
            x = x_cand; /* saves the current x value */
            ...
            /* Use a dummy password for the following rounds, if
             * any. */
            addr[0] = dummy_password;
            len[0] = dummy_password_len;
        } else if (res > 0) {
            crypto_bignum_deinit(x_cand, 1);
        }
    }
    ...
```