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

    import substrate.data_utils as sdu
    return Path, db, json, pl, sdu


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
    return


@app.cell
def _(data_dir, sdu):
    refdata = sdu.RefDataWrapper(data_dir / 'symbology.json')
    # md = sdu.load_market_data(data_dir / '')


    def get_book_snapshot(symbol: str, timestamp: str): ...
    return (refdata,)


@app.cell
def _(Path, data_dir, pl, refdata, sdu):
    def load_all(dir_path: Path) -> pl.DataFrame:
        files = [p for p in sorted(data_dir.glob('*.parquet'))]
        return pl.concat([sdu.load_market_data(p) for p in files])


    class MarketData:
        def __init__(self, path: Path, refdata: sdu.RefDataWrapper):
            self._df = load_all(path)
            self._ref_data = refdata

        def get_book_snapshot(
            self, symbol: str | int, timestamp: str
        ) -> pl.DataFrame:
            instrument_id = (
                self._ref_data.get_id(symbol)
                if isinstance(symbol, str)
                else symbol
            )
            ts_event = pl.lit(timestamp).str.to_datetime(
                time_zone='UTC', time_unit='ns'
            )

            return (
                self._df.filter((pl.col('instrument_id') == instrument_id))
                .sort((pl.col('ts_event') - ts_event).abs())
                .head(1)
            )


    md = MarketData(path=data_dir, refdata=refdata)
    book_state = md.get_book_snapshot(
        symbol='SPY', timestamp='2026-01-26T10:30:00'
    )

    print(book_state.head())
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
