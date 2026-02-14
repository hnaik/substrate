# /// script
# requires-python = ">=3.13"
# ///

import marimo

__generated_with = "0.19.8"
app = marimo.App(app_title="Fill Simulator Exploration")


@app.cell
def _():
    from pathlib import Path

    import databento as db
    import polars as pl

    import substrate.data_utils as sdu
    import substrate.types as st
    from substrate.md.data_bento import DataBento
    from substrate.fill_simulator import FillSimulator

    return DataBento, FillSimulator, Path, pl, sdu, st


@app.cell
def _(Path):
    data_dir = Path().absolute() / 'extdata'
    return (data_dir,)


@app.cell
def _(data_dir, sdu):
    refdata = sdu.RefDataWrapper(data_dir / 'symbology.json')
    return (refdata,)


@app.cell
def _(DataBento, data_dir, pl, refdata):
    df = pl.scan_parquet(data_dir / '*.parquet')
    md = DataBento(df, refdata=refdata)
    book_state = md.get_book_snapshot(
        symbol='SPY', timestamp='2026-01-26T10:30:00'
    )
    return (book_state,)


@app.cell
def _(FillSimulator, book_state, st):
    fill_sim = FillSimulator()
    response = fill_sim.execute_market_order(
        st.OrderSide.BUY, qty=100, quote=book_state
    )
    print(response)
    return


if __name__ == "__main__":
    app.run()
