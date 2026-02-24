# RL for Execution and Portfolio Allocation

**Goal.** Two compact RL studies in finance:  
1) **Optimal execution** under costs and temporary impact.  
2) **Portfolio allocation** with risk-aware reward.

**Why.** Exploratory research in basics of execution \& portfolio research with clear baselines, ablations, and limits.

## Tasks

### A. Optimal Execution
- Objective: minimize implementation shortfall over a fixed horizon.
- Environment: single-asset GBM returns, linear temporary impact, per-share fee.
- Actions: fraction of remaining inventory to trade at each step.
- Baselines: TWAP, VWAP, Almgren–Chriss schedule (deterministic reference).
- Agent: PPO (Stable-Baselines3), discrete action space.

### B. Portfolio Allocation
- Objective: maximize risk-adjusted return on a small equity universe.
- Data: daily bars (Yahoo Finance via yfinance).
- Actions: portfolio weights on simplex via softmax head.
- Baselines: equal-weight, buy-and-hold, momentum, mean-reversion.
- Agent: SAC or PPO (Stable-Baselines3).

## Metrics

- **Execution:** Implementation Shortfall (IS), slippage vs VWAP, volatility of IS.  
- **Allocation:** Annualized return, volatility, Sharpe, max drawdown, turnover.

## Experiments

- **Execution Ablation:** reward = IS vs IS + inventory penalty. Sensitivity to volatility and cost.  
- **Allocation Ablation:** reward = return − λ·vol; vs a Sharpe-like proxy; add transaction costs.

## Results
📊 Example analyses in [`notebooks/`](notebooks) visualize learning curves, risk metrics, and baselines.

## Limits and Next Steps
- Simulated environments are simplified.  
- Markets are nonstationary. Robustness and evaluation are hard.  
- Next: multi-asset execution, LOB toy sim, risk constraints, regime detectors.