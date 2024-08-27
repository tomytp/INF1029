import re
import sys
import plotext as plt

def format_subplot(subplot, title, size_indices, sizes, algorithms, data):
    subplot.title(title)
    subplot.xlabel("Matrix Size")
    subplot.ylabel("Time (ms)")
    # subplot.yscale("log")
    subplot.xticks(size_indices, sizes)

    for algorithm in algorithms:
        subplot.plot(size_indices, [data[algorithm][sizes.index(size)] for size in sizes], label=f'{algorithm}')
    subplot.theme('dark')

def main():
    benchmark_data = sys.stdin.read()

    pattern = r'Running for size=(\d+x\d+) / algorithm=(\w+)\nScalar Mult duration: ([\d.]+) ms\nMatrix Mult duration: ([\d.]+) ms\nTotal duration: ([\d.]+) ms'
    error_pattern = r'ERROR: (\w+)'
    matches = re.findall(pattern, benchmark_data)
    error_matches = re.findall(error_pattern, benchmark_data)

    scalar_mult_times = {}
    matrix_mult_times = {}
    total_times = {}
    sizes = set()
    algorithms = set()

    for match in matches:
        size, algorithm, scalar, matrix, total = match
        sizes.add(int(size.split('x')[0]))
        algorithms.add(algorithm)

        if algorithm not in scalar_mult_times:
            scalar_mult_times[algorithm] = []
            matrix_mult_times[algorithm] = []
            total_times[algorithm] = []

        scalar_mult_times[algorithm].append(float(scalar))
        matrix_mult_times[algorithm].append(float(matrix))
        total_times[algorithm].append(float(total))

    sizes = sorted(sizes)
    size_indices = list(range(len(sizes)))
    
    plt.clf()
    plt.subplots(1 ,3)
    plt.plot_size(plt.terminal_width(), plt.terminal_height() * .8)

    format_subplot(plt.subplot(1,1), "Scalar Mult Duration", size_indices, sizes, algorithms, scalar_mult_times)
    format_subplot(plt.subplot(1,2), "Matrix Mult Duration", size_indices, sizes, algorithms, matrix_mult_times)
    format_subplot(plt.subplot(1,3), "Total Duration", size_indices, sizes, algorithms, total_times)
    plt.show()

    if (error_matches):
        print("ERRORS:", ', '.join(set(error_matches)))

if __name__ == '__main__':
    main()
