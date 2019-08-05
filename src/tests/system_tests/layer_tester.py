import subprocess
from typing import List, Tuple
from sys import argv

ARGS_0: List[str] = []
ARGS_1: List[str] = ["1"]
TIMEOUT = 60
ROUNDS = 50
LAYERS = ['0', '1', '2', '3', '3s', '4', '5']


def start_victim(path: str, args: List[str]) -> subprocess.Popen:
    return subprocess.Popen([path, *args], stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)


def start_tester(path: str, args: List[str]) -> subprocess.Popen:
    return subprocess.Popen([path, *args], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def get_data_from_tester(p: subprocess.Popen) -> List[Tuple[int, int]]:
    output, _ = p.communicate(timeout=TIMEOUT)
    # noinspection PyTypeChecker
    return [tuple(map(int, line.split(' '))) for line in output.decode('utf-8').splitlines()]


def test_layer(tester_path: str, layer_name: str, victim_path: str, rounds: int, addresses: List[int]) -> \
        Tuple[List[Tuple[int, int]], List[Tuple[int, int]], List[Tuple[int, int]]]:
    # First, measure without victim running
    p = start_tester(tester_path, [layer_name, str(rounds), victim_path, *map(hex, addresses)])
    data_normal = get_data_from_tester(p)

    # Second, run with 0 as input
    victim = start_victim(victim_path, ARGS_0)
    p = start_tester(tester_path, [layer_name, str(rounds), victim_path, *map(hex, addresses)])
    data_0 = get_data_from_tester(p)
    victim.kill()

    # Third, run with 1 as input
    victim = start_victim(victim_path, ARGS_1)
    p = start_tester(tester_path, [layer_name, str(rounds), victim_path, *map(hex, addresses)])
    data_1 = get_data_from_tester(p)
    victim.kill()

    return data_normal, data_0, data_1


def main():
    assert len(argv) == 5, "Usage: python layer_tester.py tester_path victim_path address0 address1"
    tester_path = argv[1]
    victim_path = argv[2]
    addresses = [int(argv[3], 16), int(argv[4], 16)]

    for layer in LAYERS:
        data = test_layer(tester_path, layer, victim_path, ROUNDS, addresses)
        # FIXME put this data into graph and stuff
        print(f'Layer: {layer}')
        print(data)


if __name__ == '__main__':
    main()
