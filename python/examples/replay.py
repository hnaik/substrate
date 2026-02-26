"""Example: replay SPY book updates and print spread statistics.

Usage::

    python -m examples.replay_spy path/to/xnas-itch-20260126.mbp-10.dbn.zst
"""

from __future__ import annotations

import sys
from argparse import ArgumentParser
from dataclasses import dataclass, field

from substrate.data_loader import load_dbn, load_parquet
from substrate.replay_engine import ReplayEngine
from substrate.schema import BookSnapshot
from substrate.event_consumer import EventConsumer


@dataclass
class SpreadTracker(EventConsumer):
    """Example consumer that tracks spread statistics."""

    spreads: list[float] = field(default_factory=list)
    count: int = 0

    def on_book_update(self, snapshot: BookSnapshot) -> None:
        self.count += 1
        if snapshot.spread is not None:
            self.spreads.append(snapshot.spread)

        # Log progress every 100k events.
        if self.count % 100_000 == 0:
            print(f'  ... processed {self.count:,} events')

    def summary(self) -> None:
        if not self.spreads:
            print('No spread data collected.')
            return

        avg = sum(self.spreads) / len(self.spreads)
        mn = min(self.spreads)
        mx = max(self.spreads)
        print(f'\nSpread stats over {len(self.spreads):,} updates:')
        print(f'  Mean:  {avg:.4f}')
        print(f'  Min:   {mn:.4f}')
        print(f'  Max:   {mx:.4f}')


def load_dataset(path: str, **kwargs) -> list[BookSnapshot]:
    """Load a dataset from a .dbn.zst file and return a list of BookSnapshots."""
    if path.endswith('.dbn.zst'):
        return list(load_dbn(path, **kwargs))
    elif path.endswith('.parquet'):
        return list(load_parquet(path, **kwargs))
    raise ValueError(f'Unsupported file format: {path}')


def main(args) -> None:
    path = args.data_path
    symbol = args.symbol

    print(f'Loading {path} for {symbol} ...')
    source = load_dataset(path, symbol=symbol)

    tracker = SpreadTracker()
    engine = ReplayEngine(source)
    engine.add_consumer(tracker)

    stats = engine.run()

    print(f'\nReplay finished:')
    print(f'  Events emitted: {stats.events_emitted:,}')
    print(f'  Events skipped: {stats.events_skipped:,}')
    print(f'  Wall time:      {stats.elapsed_sec:.2f}s')
    print(f'  Throughput:     {stats.events_per_sec:,.0f} events/sec')
    if stats.market_duration_sec is not None:
        print(f'  Market window:  {stats.market_duration_sec:.1f}s')

    tracker.summary()


def parse_args():
    parser = ArgumentParser(
        description='Replay book updates and track spread stats.'
    )
    parser.add_argument(
        '--data-path',
        help='Path to the .dbn.zst file to replay',
        required=True,
    )
    parser.add_argument(
        '--symbol', default='SPY', help='Symbol to filter for (default: SPY)'
    )
    return parser.parse_args()


if __name__ == '__main__':
    main(parse_args())
