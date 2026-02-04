from argparse import ArgumentParser
from pathlib import Path

from loguru import logger

def main(args):
    input_dir = args.input_dir
    output_dir = args.output_dir or input_dir
    
if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('-i', '--input-dir', type=Path, required=True)
    parser.add_argument('-o', '--output-dir', type=Path)

    parsed_args = parser.parse_args()
    main(parsed_args)
