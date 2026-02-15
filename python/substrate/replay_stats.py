from __future__ import annotations

import logging
from dataclasses import dataclass

logger = logging.getLogger(__name__)


@dataclass
class ReplayStats:
    """Counters collected during a replay run."""

    events_emitted: int = 0
    events_skipped: int = 0
    elapsed_sec: float = 0.0
    first_ts: int | None = None
    last_ts: int | None = None

    @property
    def events_per_sec(self) -> float:
        if self.elapsed_sec == 0:
            return 0.0
        return self.events_emitted / self.elapsed_sec

    @property
    def market_duration_sec(self) -> float | None:
        if self.first_ts is not None and self.last_ts is not None:
            return (self.last_ts - self.first_ts) / 1e9
        return None
