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


def twap_schedule(T, q0):
    """Time-weighted average price schedule."""

    # equal slices each step
    return np.full(T, q0 / T)


def vwap_schedule(vol_profile, q0):
    """Volume-weighted average price schedule."""

    vol = np.array(vol_profile, dtype=np.float32)
    vol /= vol.sum()
    return q0 * vol
