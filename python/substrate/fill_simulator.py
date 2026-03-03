import polars as pl

from substrate.types import OrderSide
from substrate.messages import ExecutionResponse, Fill
from substrate.types import OrderStatus, OrderSide
from substrate.md.snapshot import QuoteSnapshot


def simulate_fill(
    side: OrderSide, qty: int, quote: QuoteSnapshot
) -> tuple[float, int]:
    if side == OrderSide.BUY:
        if qty <= quote.ask.qty:
            return Fill(price=quote.ask.price, qty=qty)
        else:
            return Fill(price=quote.ask.price, qty=quote.ask.qty)
    if qty <= quote.bid.qty:
        return Fill(price=quote.bid.price, qty=qty)
    return Fill(price=quote.bid.price, qty=quote.bid.qty)


class FillSimulator:
    def __init__(self, market_data: QuoteSnapshot):
        self.market_data = market_data

    def execute_market_order(
        self, side: OrderSide, qty: int, quote: QuoteSnapshot
    ):
        fill = simulate_fill(side, qty, quote)
        return ExecutionResponse(
            price=fill.price, qty=fill.qty, status=OrderStatus.FILLED
        )
