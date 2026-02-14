import polars as pl

import substrate.data_utils as sdu
from substrate.md.md_base import MDBase
from substrate.md.snapshot import BookSnapshot


class DataBento(MDBase):
    def __init__(self, df: pl.DataFrame, refdata: sdu.RefDataWrapper):
        self._df = df
        self._ref_data = refdata

    def get_book_snapshot(
        self, symbol: str | int, timestamp: str
    ) -> BookSnapshot:
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
