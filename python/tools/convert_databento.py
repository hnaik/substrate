from argparse import ArgumentParser
from pathlib import Path

from loguru import logger
import databento as db
import polars as pl


def make_dataframe(path: Path) -> pl.DataFrame:
    data = db.read_dbn(path)
    return pl.from_pandas(data.to_df())


def main(args):
    input_dir = args.input_dir
    output_dir = args.output_dir or input_dir

    for file in input_dir.glob("*.dbn.zst"):
        pq_out_path = file.parent / f"{file.stem}.parquet"
        if pq_out_path.exists():
            continue
        df = make_dataframe(file)
        print(f"loaded dataframe from {file} of dim:{df.shape}")
        df.write_parquet(pq_out_path)


if __name__ == "__main__":
    parser = ArgumentParser()
    parser.add_argument("-i", "--input-dir", type=Path, required=True)
    parser.add_argument("-o", "--output-dir", type=Path)

    parsed_args = parser.parse_args()
    main(parsed_args)
