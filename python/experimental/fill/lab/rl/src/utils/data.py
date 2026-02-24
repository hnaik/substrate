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

import os
import time
import hashlib 

import pandas as pd
import yfinance as yf
 
 
def _cache_path(cfg):
    key = f'{",".join(cfg["tickers"])}|{cfg["start"]}|{cfg["end"]}'
    h = hashlib.md5(key.encode()).hexdigest()
    os.makedirs('data/cache', exist_ok=True)
    return f'data/cache/prices_{h}.parquet'

def load_prices_yf(cfg, max_retries=6):
    cache = _cache_path(cfg)
    if os.path.exists(cache):
        return pd.read_parquet(cache).values

    delay = 1.0
    for i in range(max_retries):
        try:
            df = yf.download(
                cfg['tickers'],
                start=cfg['start'],
                end=cfg['end'],
                progress=False,
                group_by='ticker',
                auto_adjust=True,
            )
            # Normalize to Adj Close-like wide frame
            if isinstance(df.columns, pd.MultiIndex):
                df = df.xs('Close', axis=1, level=1, drop_level=False)
                df.columns = [c[0] for c in df.columns]
            elif 'Close' in df.columns:
                df = df[['Close']]
            df = df.dropna()
            df.to_parquet(cache)
            return df.values
        except Exception as e:
            if 'Rate limited' in str(e) or 'Too Many Requests' in str(e):
                time.sleep(delay)
                delay *= 2.0
            else:
                raise
    # ultimate fallback: short window to reduce calls
    raise RuntimeError('Yahoo rate-limited after retries')
