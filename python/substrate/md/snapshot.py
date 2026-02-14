from dataclasses import dataclass

import polars as pl


@dataclass
class QuoteSnapshot:
    price: float
    qty: int


@dataclass
class BookSnapshot:
    bid: QuoteSnapshot
    ask: QuoteSnapshot

    @classmethod
    def from_dbn(cls, df: pl.DataFrame) -> 'BookSnapshot':
        row = df.select(
            pl.col('bid_px_00', 'bid_sz_00', 'ask_px_00', 'ask_sz_00')
        ).collect()
        return cls(
            bid=QuoteSnapshot(
                price=row['bid_px_00'].item(),
                qty=row['bid_sz_00'].item(),
            ),
            ask=QuoteSnapshot(
                price=row['ask_px_00'].item(),
                qty=row['ask_sz_00'].item(),
            ),
        )
