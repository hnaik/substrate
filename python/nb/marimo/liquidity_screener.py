import marimo

__generated_with = "0.19.11"
app = marimo.App(width="full")


@app.cell
def _():
    import time
    from datetime import datetime

    import yfinance as yf
    import pandas as pd
    import polars as pl

    return datetime, pd, time, yf


@app.cell
def _():
    # symbols = ['SPY', 'QQQ', 'AAPL', 'MSFT', 'NVDA', 'TSLA', 'TLT']

    # results = []

    # for sym in symbols:
    #     try:
    #         ticker = yf.Ticker(sym)

    #         # Get recent history (less rate-limited than .info)
    #         hist = ticker.history(period='5d')

    #         if not hist.empty:
    #             avg_volume = hist['Volume'].mean()
    #             recent_close = hist['Close'].iloc[-1]

    #             results.append({
    #                 'Symbol': sym,
    #                 'Avg Volume (5d)': f"{avg_volume:,.0f}",
    #                 'Recent Close': f"${recent_close:.2f}"
    #             })

    #     except Exception as e:
    #         print(f"{sym}: Error - {e}")

    # # Display as table
    # df = pd.DataFrame(results)
    # print(df.to_string(index=False))
    return


@app.cell
def _(datetime, time, yf):

    ss = ['SPY', 'QQQ', 'AAPL', 'MSFT', 'NVDA', 'TSLA', 'TLT', 'RIME']

    print(f"Starting screening at {datetime.now()}")
    print("This will take ~30 seconds due to rate limits...\n")

    for sym in ss:
        max_retries = 3
        for attempt in range(max_retries):
            try:
                ticker = yf.Ticker(sym)
                hist = ticker.history(period='1mo')

                if not hist.empty:
                    avg_volume = hist['Volume'].mean()
                    print(f"{sym}: {avg_volume:,.0f} shares/day avg")
                    break

            except Exception as e:
                if attempt < max_retries - 1:
                    wait_time = (attempt + 1) * 5
                    print(f"{sym}: Rate limited, waiting {wait_time}s...")
                    time.sleep(wait_time)
                else:
                    print(f"{sym}: Failed after {max_retries} attempts")

        time.sleep(3)  # Wait between symbols

    print(f"\nCompleted at {datetime.now()}")
    return


@app.cell
def _(pd):
    import requests

    url = "https://en.wikipedia.org/wiki/S%26P_100"
    headers = {"User-Agent": "Mozilla/5.0"}
    resp = requests.get(url, headers=headers, timeout=30)
    resp.raise_for_status()

    tables = pd.read_html(resp.text)
    sp100 = tables[2]  # The constituent table

    print(sp100[['Symbol', 'Company']].head(20))
    return


@app.cell
def _():
    return


if __name__ == "__main__":
    app.run()
