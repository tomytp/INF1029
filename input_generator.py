import numpy
import argparse

def main(width: int, height: int, output_path: str, value: int | None ) -> None:
    total_cells = width * height
    
    if value is not None:
        matrix = numpy.full(total_cells, value, dtype=numpy.float32)
    else:
        matrix = numpy.random.Generator.random(width*height, dtype=numpy.float32) * 10

    matrix.tofile(output_path)

if __name__ == '__main__':
    parser = argparse.ArgumentParser('Input Generator')
    parser.add_argument('width', type=int)
    parser.add_argument('height', type=int)
    parser.add_argument('output_path', type=str)
    parser.add_argument('-v', '--value', type=int, help="value to fill the matrix (if empty the matrix is random)")
    args = parser.parse_args()
    main(args.width, args.height, args.output_path, args.value)