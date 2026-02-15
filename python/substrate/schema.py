"""Minimal event types for the execution research toolkit.

All timestamps are UTC nanoseconds (int64). No datetime objects cross
component boundaries.
"""

from __future__ import annotations

from dataclasses import dataclass, field
from typing import Protocol, runtime_checkable

# ---------------------------------------------------------------------------
# Market data types
# ---------------------------------------------------------------------------


@dataclass(frozen=True, slots=True)
class BookLevel:
    """Single price level in the order book."""

    price: float
    size: int


@dataclass(frozen=True, slots=True)
class BookSnapshot:
    """L2 book state at a point in time.

    Contains up to 10 bid/ask levels from MBP-10 data.
    ``ts_event`` is the exchange timestamp in nanoseconds since epoch (UTC).
    """

    ts_event: int
    symbol: str
    bids: tuple[BookLevel, ...] = field(default_factory=tuple)
    asks: tuple[BookLevel, ...] = field(default_factory=tuple)

    # ---- convenience properties -----------------------------------------

    @property
    def best_bid(self) -> BookLevel | None:
        return self.bids[0] if self.bids else None

    @property
    def best_ask(self) -> BookLevel | None:
        return self.asks[0] if self.asks else None

    @property
    def mid(self) -> float | None:
        if self.best_bid and self.best_ask:
            return (self.best_bid.price + self.best_ask.price) / 2.0
        return None

    @property
    def spread(self) -> float | None:
        if self.best_bid and self.best_ask:
            return self.best_ask.price - self.best_bid.price
        return None


# ---------------------------------------------------------------------------
# Consumer protocol
# ---------------------------------------------------------------------------


@runtime_checkable
class EventConsumer(Protocol):
    """Interface that downstream components implement to receive events."""

    def on_book_update(self, snapshot: BookSnapshot) -> None: ...
