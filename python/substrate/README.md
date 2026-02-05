## Steps
- Start with the standalone tool. Get the fill logic right first.
- Then build a lightweight adapter layer for backtesting that uses the validated fill logic but optimizes for your specific strategy needs.
- What type of strategy are you testing? Mid-frequency suggests you can tolerate some simplifying assumptions that HFT couldn't.
