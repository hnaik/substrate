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


def implementation_shortfall(paid, ideal):
    return paid - ideal


def sharpe(returns, eps=1e-8):
    r = np.array(returns)
    return r.mean() / (r.std() + eps)


def max_drawdown(equity_curve):
    ec = np.array(equity_curve)
    peak = np.maximum.accumulate(ec)
    dd = (ec - peak) / peak
    return dd.min()


def evaluate_exec(model, env, n_episodes=20):
    is_list = []
    for _ in range(n_episodes):
        obs, _ = env.reset()
        done = False
        cash0 = env.mid_hist[0] * env.init_inventory
        while not done:
            action, _ = model.predict(obs, deterministic=True)
            obs, rew, done, _, _ = env.step(action)
        paid = -env.cash
        ideal = cash0
        ishort = implementation_shortfall(paid, ideal)
        is_list.append(ishort)
    return {
        'IS_mean': float(np.mean(is_list)),
        'IS_std': float(np.std(is_list)),
    }


def evaluate_alloc(model, env, n_episodes=5):
    # episodic eval by rolling windows
    rets = []
    for _ in range(n_episodes):
        obs, _ = env.reset()
        done = False
        eq = [1.0]
        while not done:
            action, _ = model.predict(obs, deterministic=True)
            obs, r, done, _, info = env.step(action)
            eq.append(eq[-1] * np.exp(r))
            rets.append(r)
    return {'Sharpe': float(sharpe(rets)), 'MaxDD': float(max_drawdown(eq))}
