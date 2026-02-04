# /// script
# requires-python = ">=3.13"
# dependencies = [
#     "marimo>=0.19.0",
#     "pyzmq>=27.1.0",
# ]
# ///

import marimo

__generated_with = "0.19.7"
app = marimo.App(app_title="Fill Simulator Exploration")


@app.cell
def _():
    import json
    import os
    import time
    from datetime import date, datetime
    from pathlib import Path

    import databento as db
    import polars as pl

    return Path, db, json, pl


@app.cell
def _(Path, json):
    data_dir = Path().absolute() / "extdata"

    def load_json(name: str) -> dict:
        path = data_dir / f"{name}.json"
        return json.load(path.open())

    def print_dict(data: dict):
        for k, v in data.items():
            print(k, v)

    return (data_dir,)


@app.cell
def _(Path, db, pl):
    def make_dataframe(path: Path) -> pl.DataFrame:
        data = db.read_dbn(path)
        return pl.from_pandas(data.to_df())

    return (make_dataframe,)


@app.cell
def _(data_dir, make_dataframe, pl):
    def basic_explore(df_orig: pl.DataFrame, symbol: str):
        df = df_orig.filter(pl.col("symbol") == symbol.upper())
        print(df.head())

    for file_path in sorted(data_dir.glob("*.zst")):
        pq_out_path = file_path.parent / f"{file_path.stem}.parquet"
        if pq_out_path.exists():
            continue
        df = make_dataframe(file_path)
        print(f"loaded dataframe from {file_path} of dim:{df.shape}")
        df.write_parquet(pq_out_path)
    return


@app.cell
def _():
    # start = time.time()
    # test_df = pl.read_parquet('/home/hnaik/ws/rootmethod/substrate/extdata/xnas-itch-20260126.mbp-10.dbn.parquet')
    # stop = time.time()
    # print(f'loaded df. time={stop - start}')
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
