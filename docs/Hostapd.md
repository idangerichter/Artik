Hostapd
=======

## Installation
```bash
# Dependencies
sudo apt-get install git libnl-3-dev libssl-dev libnl-genl-3-dev

# Clone
git clone http://w1.fi/hostap.git
cd hostap
git checkout 0eb34f8f2
cd hostapd
cp defconfig .config
make
make install

# Run
hostapd

```
