import os
import random
import subprocess
from typing import List, Tuple, Dict
from sys import argv
import matplotlib.pyplot as plt
import numpy as np

ARGS_0: List[str] = []
ARGS_1: List[str] = ["1"]
TIMEOUT = 60
ROUNDS = 50
LAYERS = ['0', '1', '2', '5s']  # ['0', '1', '2', '3', '3s', '4', '5']
LAYER_ATTACK = '5s'
COLORS = ['r', 'g', 'b', 'c', 'm', 'y']
SHAPES = ['.', 's', 'p', 'v']
NORMALIZATION = 0.9


def measurements_comparator(x, y):
    return sum(x) - sum(y)


# region Running
def start_victim(path: str, args: List[str]) -> subprocess.Popen:
    return subprocess.Popen([path, *args], stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)


def start_tester(path: str, args: List[str]) -> subprocess.Popen:
    return subprocess.Popen([path, *args], stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE)


def get_data_from_tester(p: subprocess.Popen) -> List[Tuple[int, int]]:
    output, err = p.communicate(timeout=TIMEOUT)
    if err:
        print(f'error: {err}')
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


# endregion

# region Graph utils
def x_axis(graph: List[Tuple[int, int]]) -> np.ndarray:
    return np.array([x for x, y in graph])


def y_axis(graph: List[Tuple[int, int]]) -> np.ndarray:
    return np.array([y for x, y in graph])


def select(data, n, comparator):
    """Find the nth rank ordered element (the least value has rank 0)."""
    data = list(data)
    if not 0 <= n < len(data):
        raise ValueError('not enough elements for the given rank')
    while True:
        pivot = random.choice(data)
        pcount = 0
        under, over = [], []
        uappend, oappend = under.append, over.append
        for elem in data:
            result = comparator(elem, pivot)
            if result < 0:
                uappend(elem)
            elif result > 0:
                oappend(elem)
            else:
                pcount += 1
        if n < len(under):
            data = under
        elif n < len(under) + pcount:
            return pivot
        else:
            data = over
            n -= len(under) + pcount


def normalize_data(data: List[Tuple[int, int]], comparator):
    elem = select(data, int(0.9 * len(data)), comparator)
    return [x for x in data if comparator(x, elem) < 0]


# endregion


def graph_0(data: Dict[str, Tuple[List[Tuple[int, int]], List[Tuple[int, int]], List[Tuple[int, int]]]],
            output_folder: str):
    """Graph of the difference between victim states """
    normal, data0, data1 = map(lambda x: normalize_data(x, measurements_comparator), list(data[LAYER_ATTACK]))

    plt.figure()

    plt.title('Sampling')
    plt.xlabel('clock cycles for 0')
    plt.ylabel('clock cycles for 1')
    plt.plot(x_axis(normal), y_axis(normal), COLORS[0] + SHAPES[0], label='normal')
    plt.plot(x_axis(data0), y_axis(data0), COLORS[1] + SHAPES[1], label='mode 0')
    plt.plot(x_axis(data1), y_axis(data1), COLORS[2] + SHAPES[2], label='mode 1')
    plt.legend(loc='best')

    plt.savefig(os.path.join(output_folder, 'graph_scatter_attack.png'))

    plt.clf()


def graph_1(data: Dict[str, Tuple[List[Tuple[int, int]], List[Tuple[int, int]], List[Tuple[int, int]]]],
            output_folder: str):
    """Graph calculating the overhead of the library"""
    for mode_index, name in enumerate(["normal", "mode_0", "mode_1"]):
        for i in [0, 1]:
            plt.figure()
            plt.title(f'Sampling {name} for {i}')
            plt.xlabel('Sample number')
            plt.ylabel(f'clock cycles for {i}')

            for layer_index, layer in enumerate(data.keys()):
                graph_data = [it[i] for it in normalize_data(data[layer][mode_index], measurements_comparator)]
                plt.plot(list(range(len(graph_data))), graph_data, COLORS[layer_index] + SHAPES[layer_index],
                         label=f'layer {layer}')

            plt.legend(loc='best')
            plt.savefig(os.path.join(output_folder, f'graph_overhead_{name}_{i}'))
            plt.clf()


def main():
    assert len(argv) == 6, "Usage: python layer_tester.py output_folder tester_path victim_path address0 address1"
    output_folder = argv[1]
    tester_path = argv[2]
    victim_path = argv[3]
    addresses = [int(argv[4], 16), int(argv[5], 16)]

    full_data = {}
    for layer in LAYERS:
        data = test_layer(tester_path, layer, victim_path, ROUNDS, addresses)
        # FIXME put this data into graph and stuff
        print(f'Layer: {layer}')
        print(data)
        full_data[layer] = data

    graph_0(full_data, output_folder)
    graph_1(full_data, output_folder)


if __name__ == '__main__':
    main()
