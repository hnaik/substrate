"""Load Databento MBP-10 files and yield BookSnapshot events.

Requires the ``databento`` Python package::

    pip install databento
"""

from __future__ import annotations

from pathlib import Path
from typing import Iterator

import databento as db
import polars as pl

from substrate.schema import BookLevel, BookSnapshot


def _row_to_snapshot(row) -> BookSnapshot:
    """Convert a single Databento MBP-10 DataFrame row to a BookSnapshot.

    The row is a pandas NamedTuple (from ``itertuples()``) with columns like
    ``bid_px_00``, ``bid_sz_00``, ..., ``ask_px_09``, ``ask_sz_09``.
    """
    bids: list[BookLevel] = []
    asks: list[BookLevel] = []

    for i in range(10):
        bp = getattr(row, f'bid_px_{i:02d}', None)
        bs = getattr(row, f'bid_sz_{i:02d}', None)
        if bp is not None and bs is not None and bs > 0:
            bids.append(BookLevel(price=float(bp), size=int(bs)))

        ap = getattr(row, f'ask_px_{i:02d}', None)
        as_ = getattr(row, f'ask_sz_{i:02d}', None)
        if ap is not None and as_ is not None and as_ > 0:
            asks.append(BookLevel(price=float(ap), size=int(as_)))

    return BookSnapshot(
        ts_event=int(
            row['ts_event'].timestamp() * 1e9
        ),  # datetime → nanoseconds
        symbol=str(row['symbol']) if row.get('symbol') else '',
        bids=tuple(bids),
        asks=tuple(asks),
    )


def load_dbn(
    path: str | Path,
    symbol: str | None = None,
) -> Iterator[BookSnapshot]:
    """Yield ``BookSnapshot`` events from a Databento ``.dbn.zst`` file.

    Parameters
    ----------
    path:
        Path to a ``.dbn.zst`` file containing MBP-10 records.
    symbol:
        If provided, filter to this symbol only. Databento files may contain
        multiple instruments.

    Yields
    ------
    BookSnapshot
        One snapshot per book update, in exchange-timestamp order.
    """
    store = db.DBNStore.from_file(path)
    df = pl.from_pandas(
        store.to_df()
    )  # Convert to Polars for easier filtering

    if symbol is not None:
        # df = df[df['symbol'] == symbol]
        df = df.filter(pl.col('symbol') == symbol)

    # Data is already sorted by ts_event (exchange timestamp).
    for row in df.iter_rows(named=True):
        yield _row_to_snapshot(row)


def load_dbn_multi(
    paths: list[str | Path],
    symbol: str | None = None,
) -> Iterator[BookSnapshot]:
    """Load multiple ``.dbn.zst`` files in sequence.

    Files should be provided in date order. Events within each file are
    already sorted; cross-file ordering is preserved by processing files
    sequentially.
    """
    for p in paths:
        yield from load_dbn(p, symbol=symbol)


def load_parquet(
    path: str | Path, symbol: str | None = None
) -> Iterator[BookSnapshot]:
    """Placeholder for future Parquet loading support."""

    df = pl.scan_parquet(path)
    if symbol is not None:
        df = df.filter(pl.col('symbol') == symbol)

    for row in df.collect().iter_rows(named=True):
        yield _row_to_snapshot(row)
