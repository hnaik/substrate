# ==============================================================================
# fill: FInance Laboratory for Learning
# ==============================================================================
# Copyright (C) 2025  Harish Naik <harishnaik@gmail.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU LesserGeneral Public License along
# with this program. If not, see <https://www.gnu.org/licenses/lgpl-3.0.html>.
# ==============================================================================

import numpy as np


def equal_weight(N):
    """Equal weight signal."""

    return np.ones(N) / N


def buy_and_hold(w0=None, N=None):
    """Buy and hold signal."""

    return equal_weight(N) if w0 is None else w0


def momentum_signal(returns, lookback=60):
    """Momentum signal."""

    mu = returns[-lookback:].mean(axis=0)
    w = np.maximum(mu, 0.0)
    s = w.sum()
    return w / s if s > 1e-8 else equal_weight(returns.shape[1])


def reverse_signal(returns, lookback=5):
    """Reverse momentum signal."""

    mu = returns[-lookback:].mean(axis=0)
    w = np.maximum(-mu, 0.0)
    s = w.sum()
    return w / s if s > 1e-8 else equal_weight(returns.shape[1])
