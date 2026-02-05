import polars as pl

from substrate.types import OrderSide


def simulate_fill(
    order_side: OrderSide,
    order_size: int,
    bid_px: float,
    ask_px: float,
    bid_sz: int,
    ask_sz: int,
) -> tuple[float, int]:
    if order_side == OrderSide.BUY:
        if order_size <= ask_sz:
            return ask_px, order_size
        else:
            return ask_px, ask_sz
    elif order_side == OrderSide.SELL:
        if order_size <= bid_sz:
            return bid_px, order_size
        else:
            return bid_px, bid_sz


class FillSimulator:
    def __init__(self, market_data: pl.DataFrame):
        self.market_data = market_data

    def execute_market_order(
        self, ts: int, symbol: str, side: OrderSide, size: int
    ) -> tuple[float, int]:
        row = (
            self.market_data.filter(
                (pl.col('timestamp') == ts) & (pl.col('symbol') == symbol)
            )
            .select(
                [
                    pl.col('bid_price').first(),
                    pl.col('ask_price').first(),
                    pl.col('bid_size').first(),
                    pl.col('ask_size').first(),
                ]
            )
            .to_dicts()[0]
        )

        fill_price, fill_size = simulate_fill(
            order_side=side,
            order_size=size,
            bid_px=row['bid_price'],
            ask_px=row['ask_price'],
            bid_sz=row['bid_size'],
            ask_sz=row['ask_size'],
        )

        return fill_price, fill_size

    def simulate_order(
        self, ts: int, symbol: str, side, size: int, order_type: str
    ): ...
