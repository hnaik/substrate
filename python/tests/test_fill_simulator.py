import polars as pl

import pytest

import substrate.fill_simulator as sfs


@pytest.fixture
def market_data() -> pl.DataFrame:
    df = pl.DataFrame({})
    return df


def test_fill_simulator(market_data: pl.DataFrame):
    simulator = sfs.FillSimulator(market_data=market_data)

    # Test basic functionality
    fills = simulator.simulate_fills(
        symbol='SPY',
        side='buy',
        order_size=100,
        start_time='2026-01-26 09:30:00',
        end_time='2026-01-26 16:00:00',
    )

    assert len(fills) > 0
    for fill in fills:
        assert fill['symbol'] == 'SPY'
        assert fill['side'] == 'buy'
        assert fill['size'] <= 100
