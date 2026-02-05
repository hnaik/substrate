import os
from ftplib import FTP
from pathlib import Path

import databento as db

# URI = "ftp://ftp.databento.com/D8V9NUWP/XNAS-20260204-NAPHKYS6FS"

# ftp = FTP("ftp.databento.com")
# ftp.login("harishgnaik@gmail.com", "")
# ftp.cwd("/D8V9NUWP/XNAS-20260204-NAPHKYS6FS")

# files = ftp.nlst()
# for file in files:
#     output_file = Path(f"/home/hnaik/ws/rootmethod/substrate/extdata/{file}")
#     output_file.parent.mkdir(parents=True, exist_ok=True)
#     ftp.retrbinary(f"RETR {file}", open(output_file, "wb").write)
# ftp.quit()

# db.download("XNAS", "20260204-NAPHKYS6FS", "/home/hnaik/ws/rootmethod/substrate/extdata")

# print(db.list_datasets())


def make_historical_client():
    api_key = os.getenv("DATABENTO_API_KEY")
    if not api_key:
        raise ValueError("DATABENTO_API_KEY is not set")
    return db.Historical(api_key)


client = make_historical_client()
data = client.timeseries.get_range(
    dataset="GLBX.MDP3",
    symbols="ALL_SYMBOLS",
    start="2022-06-02T14:20:00",
    end="2022-06-02T14:30:00",
)

# data.replay(print)
