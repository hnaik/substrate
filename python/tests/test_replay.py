"""Tests for the replay engine and schema.

These tests use synthetic data so they run without Databento installed.
"""

from __future__ import annotations

import pytest

from exrt.replay import ReplayEngine, ReplayStats
from exrt.schema import BookLevel, BookSnapshot, EventConsumer


# ---------------------------------------------------------------------------
# Fixtures / helpers
# ---------------------------------------------------------------------------


def make_snapshot(
    ts_ns: int, bid_px: float = 100.0, ask_px: float = 100.02
) -> BookSnapshot:
    """Build a simple one-level BookSnapshot."""
    return BookSnapshot(
        ts_event=ts_ns,
        symbol='TEST',
        bids=(BookLevel(price=bid_px, size=500),),
        asks=(BookLevel(price=ask_px, size=300),),
    )


def synthetic_stream(
    n: int = 100, start_ns: int = 1_000_000_000
) -> list[BookSnapshot]:
    """Return a list of snapshots with incrementing timestamps."""
    return [make_snapshot(start_ns + i * 1_000_000) for i in range(n)]


class RecordingConsumer:
    """Consumer that records every event it receives."""

    def __init__(self) -> None:
        self.events: list[BookSnapshot] = []

    def on_book_update(self, snapshot: BookSnapshot) -> None:
        self.events.append(snapshot)


# ---------------------------------------------------------------------------
# Schema tests
# ---------------------------------------------------------------------------


class TestBookSnapshot:
    def test_mid_price(self):
        snap = make_snapshot(0, bid_px=100.0, ask_px=100.04)
        assert snap.mid == pytest.approx(100.02)

    def test_spread(self):
        snap = make_snapshot(0, bid_px=100.0, ask_px=100.04)
        assert snap.spread == pytest.approx(0.04)

    def test_empty_book(self):
        snap = BookSnapshot(ts_event=0, symbol='EMPTY')
        assert snap.best_bid is None
        assert snap.best_ask is None
        assert snap.mid is None
        assert snap.spread is None

    def test_frozen(self):
        snap = make_snapshot(0)
        with pytest.raises(AttributeError):
            snap.ts_event = 1  # type: ignore[misc]


# ---------------------------------------------------------------------------
# Replay engine tests
# ---------------------------------------------------------------------------


class TestReplayEngine:
    def test_emits_all_events(self):
        data = synthetic_stream(50)
        consumer = RecordingConsumer()
        engine = ReplayEngine(iter(data))
        engine.add_consumer(consumer)
        stats = engine.run()

        assert stats.events_emitted == 50
        assert len(consumer.events) == 50

    def test_start_filter(self):
        data = synthetic_stream(100, start_ns=1_000_000_000)
        cutoff = 1_000_000_000 + 50 * 1_000_000  # halfway
        consumer = RecordingConsumer()
        engine = ReplayEngine(iter(data), start_ns=cutoff)
        engine.add_consumer(consumer)
        stats = engine.run()

        assert stats.events_emitted == 50
        assert stats.events_skipped == 50
        assert all(e.ts_event >= cutoff for e in consumer.events)

    def test_end_filter(self):
        data = synthetic_stream(100, start_ns=1_000_000_000)
        cutoff = 1_000_000_000 + 25 * 1_000_000
        consumer = RecordingConsumer()
        engine = ReplayEngine(iter(data), end_ns=cutoff)
        engine.add_consumer(consumer)
        stats = engine.run()

        assert stats.events_emitted == 26  # events 0..25 inclusive
        assert all(e.ts_event <= cutoff for e in consumer.events)

    def test_multiple_consumers(self):
        data = synthetic_stream(10)
        c1 = RecordingConsumer()
        c2 = RecordingConsumer()
        engine = ReplayEngine(iter(data))
        engine.add_consumer(c1)
        engine.add_consumer(c2)
        engine.run()

        assert len(c1.events) == 10
        assert len(c2.events) == 10

    def test_empty_source(self):
        consumer = RecordingConsumer()
        engine = ReplayEngine(iter([]))
        engine.add_consumer(consumer)
        stats = engine.run()

        assert stats.events_emitted == 0
        assert len(consumer.events) == 0

    def test_rejects_bad_consumer(self):
        engine = ReplayEngine(iter([]))
        with pytest.raises(TypeError):
            engine.add_consumer('not a consumer')  # type: ignore[arg-type]


class TestReplayStats:
    def test_events_per_sec(self):
        s = ReplayStats(events_emitted=1000, elapsed_sec=2.0)
        assert s.events_per_sec == pytest.approx(500.0)

    def test_market_duration(self):
        s = ReplayStats(first_ts=1_000_000_000, last_ts=2_000_000_000)
        assert s.market_duration_sec == pytest.approx(1.0)

    def test_market_duration_none(self):
        s = ReplayStats()
        assert s.market_duration_sec is None
