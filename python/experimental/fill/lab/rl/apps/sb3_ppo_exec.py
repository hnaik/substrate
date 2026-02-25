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
from stable_baselines3 import PPO

from envs.exec_env import ExecutionEnv
from eval.metrics import evaluate_exec


def main(cfg):
    env = ExecutionEnv(cfg['env'])
    model = PPO(
        'MlpPolicy',
        env,
        **{k: v for k, v in cfg['agent'].items() if k != 'policy_kwargs'},
        policy_kwargs=cfg['agent'].get('policy_kwargs', {}),
        verbose=0,
    )
    model.learn(total_timesteps=cfg['train']['timesteps'])
    stats = evaluate_exec(model, env, n_episodes=cfg['eval']['n_episodes'])

    model_dir = Path(cfg.get('models_dir', 'models'))
    model_path = model_dir / 'exec_ppo_final.zip'
    model.save(model_path)
    print(stats)


if __name__ == '__main__':
    parser = ArgumentParser()
    parser.add_argument('--config', type=Path, required=True)
    args = parser.parse_args()
    cfg = tomllib.load(args.config.open('rb'))
    main(cfg)
