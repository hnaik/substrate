"""Replay engine: drives historical market data through registered consumers.

This is the minimal version — single symbol, as-fast-as-possible playback.
Multi-symbol interleaving and wall-clock replay modes will be added later.
"""

from __future__ import annotations

import logging
import time
from typing import Iterator

import polars as pl

from substrate.data_loader import _row_to_snapshot
from substrate.md.snapshot import BookSnapshot
from substrate.event_consumer import EventConsumer
from substrate.replay_stats import ReplayStats

logger = logging.getLogger(__name__)


class ReplayEngine:
    """Replays a stream of ``BookSnapshot`` events to registered consumers.

    Parameters
    ----------
    source:
        Any iterator that yields ``BookSnapshot`` objects. Typically this
        comes from ``data_loader.load_dbn`` or ``data_loader.load_dbn_multi``.
    start_ns:
        Optional. Skip events whose ``ts_event`` is before this timestamp
        (UTC nanoseconds).
    end_ns:
        Optional. Stop replaying once ``ts_event`` exceeds this timestamp
        (UTC nanoseconds).

    Example
    -------
    ::

        from exrt.data_loader import load_dbn
        from exrt.replay import ReplayEngine

        source = load_dbn("xnas-itch-20260126.mbp-10.dbn.zst", symbol="SPY")
        engine = ReplayEngine(source)
        engine.add_consumer(my_fill_simulator)
        stats = engine.run()
        print(f"Replayed {stats.events_emitted} events in {stats.elapsed_sec:.2f}s")
    """

    def __init__(
        self,
        source: Iterator[BookSnapshot] | pl.DataFrame | pl.LazyFrame,
        start_ns: int | None = None,
        end_ns: int | None = None,
    ) -> None:
        self._source: Iterator[BookSnapshot] | pl.DataFrame | pl.LazyFrame = (
            source
        )
        self._start_ns = start_ns
        self._end_ns = end_ns
        self._consumers: list[EventConsumer] = []

    def add_consumer(self, consumer: EventConsumer) -> None:
        """Register a downstream consumer to receive book update events."""
        if not isinstance(consumer, EventConsumer):
            raise TypeError(
                f'Consumer must implement EventConsumer protocol, '
                f'got {type(consumer).__name__}'
            )
        self._consumers.append(consumer)

    def run(self) -> ReplayStats:
        """Execute the replay loop. Returns statistics about the run.

        Iterates through the source, applies time filters, and dispatches
        each ``BookSnapshot`` to all registered consumers in order.
        """
        stats = ReplayStats()
        wall_start = time.perf_counter()

        for snapshot in self._iter_source():
            # Apply time window filters.
            if (
                self._start_ns is not None
                and snapshot.ts_event < self._start_ns
            ):
                stats.events_skipped += 1
                continue
            if self._end_ns is not None and snapshot.ts_event > self._end_ns:
                break

            # Track timestamps.
            if stats.first_ts is None:
                stats.first_ts = snapshot.ts_event
            stats.last_ts = snapshot.ts_event

            # Dispatch to consumers.
            for consumer in self._consumers:
                consumer.on_book_update(snapshot)

            stats.events_emitted += 1

        stats.elapsed_sec = time.perf_counter() - wall_start
        logger.info(
            'Replay complete: %d events in %.2fs (%.0f events/sec)',
            stats.events_emitted,
            stats.elapsed_sec,
            stats.events_per_sec,
        )
        return stats

    def _iter_source(self) -> Iterator[BookSnapshot]:
        if isinstance(self._source, pl.LazyFrame):
            try:
                df = self._source.collect(streaming=True)
            except TypeError:
                df = self._source.collect()
            for row in df.iter_rows(named=True):
                yield _row_to_snapshot(row)
            return

        if isinstance(self._source, pl.DataFrame):
            for row in self._source.iter_rows(named=True):
                yield _row_to_snapshot(row)
            return

        yield from self._source
