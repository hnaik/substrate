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

import gymnasium as gym
from gymnasium import spaces
import numpy as np


class AllocationEnv(gym.Env):
    """Allocation environment for a multi-asset portfolio."""

    def __init__(
        self,
        prices,
        window=60,
        rebalance_every=5,
        cost_bps=2.0,
        reward_cfg=None,
    ):
        self.prices = prices
        self.returns = np.log(prices[1:] / prices[:-1])
        self.T, self.N = self.returns.shape
        self.window = window
        self.k = rebalance_every
        self.cost = cost_bps * 1e-4
        self.reward_cfg = reward_cfg or {
            'type': 'ret_minus_lambda_vol',
            'lambda_vol': 5.0e-2,
        }
        self.action_space = spaces.Box(
            low=0.0, high=1.0, shape=(self.N,), dtype=np.float32
        )
        self.observation_space = spaces.Box(
            low=-np.inf,
            high=np.inf,
            shape=(self.window, self.N),
            dtype=np.float32,
        )
        self.reset()

    def reset(self, seed=None, options=None):
        self.t = self.window
        self.w = np.ones(self.N) / self.N
        self.equity = 1.0
        return self._observe(), {}

    def _observe(self):
        end = min(self.t, self.T)
        start = end - self.window
        return self.returns[start:end].astype(np.float32)

    def _reward(self, seg_ret, ret_seg):
        if self.reward_cfg['type'] == 'ret_minus_lambda_vol':
            vol = np.sqrt((ret_seg @ self.w).var() + 1e-12)
            return seg_ret - self.reward_cfg['lambda_vol'] * vol
        elif self.reward_cfg['type'] == 'sharpe_proxy':
            mean = (ret_seg @ self.w).mean()
            std = np.sqrt((ret_seg @ self.w).var() + 1e-12)
            return mean / (std + 1e-6)
        return seg_ret

    def step(self, action):
        a = np.maximum(action, 0.0)
        a /= np.clip(a.sum(), 1e-6, None)

        # transaction costs on turnover
        turnover = np.abs(a - self.w).sum()
        tc = self.cost * turnover

        # apply k steps until next rebalance
        ret_seg = self.returns[self.t : self.t + self.k]
        seg_ret = (ret_seg @ self.w).sum()
        self.equity *= np.exp(seg_ret) * (1 - tc)
        self.w = a
        self.t += self.k
        terminated = self.t >= self.T
        reward = self._reward(seg_ret, ret_seg)
        return (
            self._observe(),
            reward,
            terminated,
            False,
            {'turnover': turnover},
        )
