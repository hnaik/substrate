import polars as pl

import pytest

from substrate.types import OrderSide
from substrate.messages import Fill
from substrate.md.snapshot import QuoteSnapshot
from substrate.fill_simulator import simulate_fill, FillSimulator

import substrate.fill_simulator as sfs


@pytest.fixture
def market_data() -> pl.DataFrame:
    df = pl.DataFrame({})
    return df


class MockLevel:
    def __init__(self, price: float, qty: int):
        self.price = price
        self.qty = qty


class MockQuoteSnapshot:
    def __init__(
        self, bid_price: float, bid_qty: int, ask_price: float, ask_qty: int
    ):
        self.bid = MockLevel(bid_price, bid_qty)
        self.ask = MockLevel(ask_price, ask_qty)


@pytest.fixture
def quote_snapshot():
    return MockQuoteSnapshot(
        bid_price=100.0, bid_qty=1000, ask_price=100.1, ask_qty=500
    )


def test_simulate_fill_buy_partial(quote_snapshot):
    fill = simulate_fill(OrderSide.BUY, 300, quote_snapshot)
    assert fill.price == 100.1
    assert fill.qty == 300


def test_simulate_fill_buy_exceed_ask(quote_snapshot):
    fill = simulate_fill(OrderSide.BUY, 1000, quote_snapshot)
    assert fill.price == 100.1
    assert fill.qty == 500


def test_simulate_fill_sell_partial(quote_snapshot):
    fill = simulate_fill(OrderSide.SELL, 500, quote_snapshot)
    assert fill.price == 100.0
    assert fill.qty == 500


def test_simulate_fill_sell_exceed_bid(quote_snapshot):
    fill = simulate_fill(OrderSide.SELL, 2000, quote_snapshot)
    assert fill.price == 100.0
    assert fill.qty == 1000


def test_execute_market_order_buy(quote_snapshot):
    simulator = FillSimulator(quote_snapshot)
    response = simulator.execute_market_order(
        OrderSide.BUY, 200, quote_snapshot
    )
    assert response.price == 100.1
    assert response.qty == 200
