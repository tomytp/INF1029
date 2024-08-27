import numpy as np
import sys
import argparse

def read_matrix(filename, height, width):
    with open(filename, 'rb') as f:
        return np.fromfile(f, dtype=np.float32).reshape(height, width)

def main(height, width, scalar, input_a, input_b, output):
    a = read_matrix(input_a, height, width)
    b = read_matrix(input_b, height, width)
    c = read_matrix(output, height, width)

    a_scaled = a * scalar
    result = np.matmul(a_scaled, b)

    is_equal = np.allclose(result, c, rtol=1e-5, atol=1e-8)

    print(f"Matrices are {'equal' if is_equal else 'not equal'}")
    if (not is_equal):
        print(result)
        print()
        print(c)

if __name__ == "__main__":
    parser = argparse.ArgumentParser('Check Results')
    parser.add_argument('width', type=int)
    parser.add_argument('height', type=int)
    parser.add_argument('scalar', type=float)
    parser.add_argument('input_a', type=str)
    parser.add_argument('input_b', type=str)
    parser.add_argument('output', type=str)

    args = parser.parse_args()

    main(args.height, args.width, args.scalar, args.input_a, args.input_b, args.output)