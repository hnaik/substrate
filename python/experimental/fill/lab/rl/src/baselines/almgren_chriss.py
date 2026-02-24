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


def almgren_chriss_linear(T, q0, lam=1e-6, sigma=0.2, eta=1e-6):
    """Almgren-Chriss linear price impact model.

    Simplified AC schedule for linear temp impact and variance penalty
    `lam * sigma^2`. Returns per-step shares to execute that sum to q0.

    """

    # In discrete-time with linear temp impact, optimal schedule is close to
    # uniform, with slight front-loading increasing with risk aversion.

    base = q0 / T
    tilt = np.linspace(1.0, 0.5, T)  # crude tilt
    tilt /= tilt.sum()
    alpha = np.clip(lam * sigma**2 / max(eta, 1e-12), 0.0, 1.0)
    sched = (1 - alpha) * np.full(T, base) + alpha * q0 * tilt
    return sched * (q0 / sched.sum())
