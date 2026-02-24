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

import pandas as pd
import pandas_datareader.data as web


def load_prices_stooq(tickers, start, end):
    # Stooq delivers daily data and is light for demos
    frames = []
    for t in tickers:
        df = web.DataReader(t, 'stooq', start=start, end=end)[
            ['Close']
        ].rename(columns={'Close': t})
        frames.append(df)
    out = pd.concat(frames, axis=1).sort_index().dropna()
    return out
