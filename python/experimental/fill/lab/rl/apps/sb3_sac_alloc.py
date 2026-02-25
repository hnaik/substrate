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

from argparse import ArgumentParser
from pathlib import Path
import tomllib

from stable_baselines3 import SAC, PPO

from envs.alloc_env import AllocationEnv
from eval.metrics import evaluate_alloc
from utils.data import load_prices_yf


def process(cfg):
    prices = load_prices_yf(cfg['data'])
    print(prices)

    env = AllocationEnv(
        prices,
        window=cfg['env']['window'],
        rebalance_every=cfg['env']['rebalance_every'],
        cost_bps=cfg['data']['cost_bps'],
        reward_cfg=cfg['reward'],
    )
    algo = cfg['agent']['algo']
    Algo = SAC if algo == 'SAC' else PPO
    model = Algo(
        'MlpPolicy',
        env,
        verbose=0,
        learning_rate=cfg['agent']['learning_rate'],
        gamma=cfg['agent']['gamma'],
    )
    model.learn(total_timesteps=cfg['train']['timesteps'])
    stats = evaluate_alloc(model, env, n_episodes=cfg['eval']['n_episodes'])

    model_dir = Path(cfg.get('models_dir', 'models'))
    model_path = model_dir / 'alloc_sac_final.zip'
    model.save(model_path)
    return stats


def main(cfg):
    try:
        stats = process(cfg)
        print(stats)
    except RuntimeError:
        return 0


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--config', type=Path, required=True)
    args = parser.parse_args()
    cfg = tomllib.load(args.config.open('rb'))
    main(cfg)
