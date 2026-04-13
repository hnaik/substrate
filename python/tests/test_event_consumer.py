from __future__ import annotations

from substrate.event_consumer import EventConsumer
from substrate.md.snapshot import BookSnapshot, QuoteSnapshot


def _snapshot(ts_event: int) -> BookSnapshot:
    snapshot = BookSnapshot(
        bid=QuoteSnapshot(price=100.0, qty=10),
        ask=QuoteSnapshot(price=100.5, qty=12),
    )
    # ReplayEngine expects this attribute on snapshots.
    snapshot.ts_event = ts_event
    return snapshot


def test_event_consumer_base_methods_are_noop() -> None:
    consumer = EventConsumer()
    snapshot = _snapshot(123)

    assert consumer.on_book_snapshot(snapshot) is None
    assert consumer.on_book_update(snapshot) is None


def test_event_consumer_subclass_can_override_handlers() -> None:
    received: list[tuple[str, int]] = []

    class RecordingConsumer(EventConsumer):
        def on_book_snapshot(self, snapshot: BookSnapshot) -> None:
            received.append(("snapshot", snapshot.ts_event))

        def on_book_update(self, snapshot: BookSnapshot) -> None:
            received.append(("update", snapshot.ts_event))

    consumer = RecordingConsumer()
    snapshot = _snapshot(456)
    consumer.on_book_snapshot(snapshot)
    consumer.on_book_update(snapshot)

    assert received == [("snapshot", 456), ("update", 456)]
