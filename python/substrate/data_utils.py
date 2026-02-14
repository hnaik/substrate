import json
from pathlib import Path

import polars as pl


def load_market_data(data_path: Path) -> pl.DataFrame:
    """Load market data from a given file path into a Polars DataFrame."""
    return pl.read_parquet(data_path)


class RefDataWrapper:
    def __init__(self, path: Path):
        self._id_to_ticker, self._ticker_to_id = self._parse_symbology_file(
            Path(path)
        )

    def get_ticker(self, sym_id: int) -> str:
        return self._id_to_ticker.get(sym_id, '')

    def get_id(self, ticker: str) -> int:
        return self._ticker_to_id.get(ticker, -1)

    def _parse_symbology_file(self, path: Path):
        d = json.load(path.open('r'))
        id_to_ticker = {}
        ticker_to_id = {}
        for ticker, values in d['result'].items():
            sym_id = int(values[0]['s'])
            id_to_ticker.update({sym_id: ticker})
            ticker_to_id.update({ticker: sym_id})
        return id_to_ticker, ticker_to_id
