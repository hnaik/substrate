from __future__ import annotations

from substrate.replay_stats import ReplayStats


def test_replay_stats_defaults() -> None:
    stats = ReplayStats()

    assert stats.events_emitted == 0
    assert stats.events_skipped == 0
    assert stats.elapsed_sec == 0.0
    assert stats.first_ts is None
    assert stats.last_ts is None


def test_events_per_sec_returns_zero_when_elapsed_is_zero() -> None:
    stats = ReplayStats(events_emitted=10, elapsed_sec=0.0)

    assert stats.events_per_sec == 0.0


def test_events_per_sec_computes_ratio() -> None:
    stats = ReplayStats(events_emitted=25, elapsed_sec=2.5)

    assert stats.events_per_sec == 10.0


def test_market_duration_sec_none_when_timestamps_missing() -> None:
    assert ReplayStats(first_ts=None, last_ts=100).market_duration_sec is None
    assert ReplayStats(first_ts=100, last_ts=None).market_duration_sec is None
    assert ReplayStats(first_ts=None, last_ts=None).market_duration_sec is None


def test_market_duration_sec_converts_ns_to_seconds() -> None:
    stats = ReplayStats(first_ts=1_000_000_000, last_ts=4_500_000_000)

    assert stats.market_duration_sec == 3.5
