from __future__ import annotations

from substrate.event_consumer import EventConsumer
from substrate.md.snapshot import BookSnapshot, QuoteSnapshot
from substrate.replay_engine import ReplayEngine


def _snapshot(ts_event: int) -> BookSnapshot:
    snapshot = BookSnapshot(
        bid=QuoteSnapshot(price=100.0, qty=100),
        ask=QuoteSnapshot(price=100.5, qty=120),
    )
    # ReplayEngine uses this field for filtering/metrics.
    snapshot.ts_event = ts_event
    return snapshot


class RecordingConsumer(EventConsumer):
    def __init__(self) -> None:
        self.ts_seen: list[int] = []

    def on_book_update(self, snapshot: BookSnapshot) -> None:
        self.ts_seen.append(snapshot.ts_event)


def test_add_consumer_accepts_event_consumer_subclass() -> None:
    engine = ReplayEngine(source=iter([]))
    consumer = RecordingConsumer()

    engine.add_consumer(consumer)

    stats = engine.run()
    assert stats.events_emitted == 0


def test_add_consumer_rejects_non_event_consumer() -> None:
    engine = ReplayEngine(source=iter([]))

    try:
        engine.add_consumer(object())  # type: ignore[arg-type]
    except TypeError as exc:
        assert 'Consumer must implement EventConsumer protocol' in str(exc)
    else:
        raise AssertionError('Expected TypeError for invalid consumer')


def test_run_dispatches_events_and_updates_stats() -> None:
    source = iter([_snapshot(10), _snapshot(20), _snapshot(35)])
    engine = ReplayEngine(source=source)
    consumer = RecordingConsumer()
    engine.add_consumer(consumer)

    stats = engine.run()

    assert consumer.ts_seen == [10, 20, 35]
    assert stats.events_emitted == 3
    assert stats.events_skipped == 0
    assert stats.first_ts == 10
    assert stats.last_ts == 35
    assert stats.market_duration_sec == (35 - 10) / 1e9
    assert stats.elapsed_sec >= 0.0


def test_run_applies_start_filter_and_counts_skips() -> None:
    source = iter([_snapshot(5), _snapshot(10), _snapshot(15), _snapshot(20)])
    engine = ReplayEngine(source=source, start_ns=11)
    consumer = RecordingConsumer()
    engine.add_consumer(consumer)

    stats = engine.run()

    assert consumer.ts_seen == [15, 20]
    assert stats.events_skipped == 2
    assert stats.events_emitted == 2
    assert stats.first_ts == 15
    assert stats.last_ts == 20


def test_run_stops_when_snapshot_exceeds_end_ns() -> None:
    source = iter([_snapshot(10), _snapshot(20), _snapshot(30), _snapshot(40)])
    engine = ReplayEngine(source=source, end_ns=25)
    consumer = RecordingConsumer()
    engine.add_consumer(consumer)

    stats = engine.run()

    # 30 exceeds end_ns, so replay stops before dispatching it.
    assert consumer.ts_seen == [10, 20]
    assert stats.events_emitted == 2
    assert stats.events_skipped == 0
    assert stats.first_ts == 10
    assert stats.last_ts == 20


def test_run_supports_multiple_consumers() -> None:
    source = iter([_snapshot(101), _snapshot(102)])
    engine = ReplayEngine(source=source)
    c1 = RecordingConsumer()
    c2 = RecordingConsumer()
    engine.add_consumer(c1)
    engine.add_consumer(c2)

    stats = engine.run()

    assert c1.ts_seen == [101, 102]
    assert c2.ts_seen == [101, 102]
    assert stats.events_emitted == 2
