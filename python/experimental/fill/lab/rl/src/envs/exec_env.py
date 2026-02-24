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
import gymnasium as gym
from gymnasium import spaces


class ExecutionEnv(gym.Env):
    """Execution environment for a single asset."""

    metadata = {'render.modes': []}

    def __init__(self, config):
        super().__init__()
        self.config = config
        self.steps = config.get('steps', 50)
        self.init_inventory = config.get('init_inventory', 1000)
        self.fee = config.get('fee_per_share', 0.0001)
        self.impact = config.get('impact_coeff', 2e-3)
        self.mu = config.get('gbm_mu', 0.00)
        self.sigma = config.get('gbm_sigma', 0.02)
        self.dt = config.get('dt', 1.0)
        self.rng = np.random.default_rng(config.get('seed', 123))
        self.reward_cfg = config.get(
            'reward', {'type': 'is_only', 'inv_penalty': 0.0}
        )
        self.action_fracs = np.array([0.0, 0.05, 0.1, 0.2, 0.4])
        self.action_space = spaces.Discrete(len(self.action_fracs))
        self.observation_space = spaces.Box(
            low=-np.inf, high=np.inf, shape=(3,), dtype=np.float32
        )
        self.reset()

    def reset(self, seed=None, options=None):
        self.t = 0
        self.inv = self.init_inventory
        self.mid = 100.0
        self.mid_hist = [self.mid]
        self.cash = 0.0
        self.done = False
        return self._observe(), {}

    def step(self, action):
        frac = self.action_fracs[int(action)]
        qty = min(self.inv, int(np.round(frac * self.inv)))
        trade_price = self.mid * (
            1 - self.impact * (qty / max(self.init_inventory, 1))
        )
        cost = qty * (trade_price + self.fee)
        self.cash -= cost
        self.inv -= qty
        self.mid_hist.append(self.mid)
        self.t += 1
        terminated = self.t >= self.steps or self.inv == 0
        reward = 0.0
        if terminated:
            if self.inv > 0:
                liq_price = self.mid * (
                    1 - self.impact * (self.inv / max(self.init_inventory, 1))
                )
                self.cash -= self.inv * (liq_price + self.fee)
                self.inv = 0
            ideal = self.mid_hist[0] * self.init_inventory
            paid = -self.cash
            ishort = paid - ideal
            reward = -ishort
            if self.reward_cfg['type'] == 'is_plus_inv_pen':
                reward += self.reward_cfg.get('inv_penalty', 0.0) * (
                    self.inv**2
                )
        return self._observe(), reward, terminated, False, {}

    def _observe(self):
        tfrac = self.t / self.steps
        rem = self.inv / max(self.init_inventory, 1)
        last_ret = (
            0.0
            if len(self.mid_hist) < 2
            else np.log(self.mid_hist[-1] / self.mid_hist[-2])
        )
        return np.array([tfrac, rem, last_ret], dtype=np.float32)
