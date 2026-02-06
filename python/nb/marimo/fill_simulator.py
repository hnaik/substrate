# /// script
# requires-python = ">=3.13"
# ///

import marimo

__generated_with = "0.19.8"
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
    import substrate.types as st

    return Path, db, json, pl, sdu, st


@app.cell
def _(Path, json):
    data_dir = Path().absolute() / 'extdata'

    def load_json(name: str) -> dict:
        path = data_dir / f'{name}.json'
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

    def get_book_snapshot(symbol: str, timestamp: str): ...

    return (refdata,)


@app.cell
def _(Path, data_dir, pl, refdata, sdu):
    def load_all(dir_path: Path) -> pl.DataFrame:
        files = [p for p in sorted(data_dir.glob('*.parquet'))]
        return pl.concat([sdu.load_market_data(p) for p in files])

    class MarketData:
        def __init__(self, path: Path, refdata: sdu.RefDataWrapper):
            # self._df = load_all(path)
            self._df = pl.scan_parquet(data_dir / '*.parquet')
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
    return (book_state,)


@app.cell
def _(book_state, pl, st):
    from dataclasses import dataclass
    from enum import Enum


    class OrderStatus(Enum):
        NewOrder = 1
        OrderAcked = 2
        Canceled = 3
        Partial = 4
        Filled = 5


    @dataclass
    class ExecutionResponse:
        price: float
        size: int
        status: OrderStatus.Filled


    class FillSimulator:
        def __init__(self): ...

        def execute_market_order(
            self, side: st.OrderSide, size: int, book_state: pl.DataFrame
        ):
            response = ExecutionResponse(0, size=size, status=OrderStatus.Filled)
            if side == st.OrderSide.BUY:
                row = book_state.select(pl.col('ask_sz_00', 'ask_px_00')).collect()
                response.price = row['ask_px_00']
                response.size = (
                    size if size <= row['ask_sz_00'] else row['ask_sz_00']
                )
            else:
                row = book_state.select(pl.col('bid_sz_00', 'bid_px_00')).collect()
                response.price = row['bid_px_00']
                response.size = (
                    size if size <= row['bid_sz_00'] else row['bid_sz_00']
                )
            return response


    fill_sim = FillSimulator()
    response = fill_sim.execute_market_order(
        st.OrderSide.BUY, size=100, book_state=book_state
    )
    print(response)
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
