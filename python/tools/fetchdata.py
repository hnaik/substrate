from ftplib import FTP
from pathlib import Path

URI = "ftp://ftp.databento.com/D8V9NUWP/XNAS-20260204-NAPHKYS6FS"

ftp = FTP("ftp.databento.com")
ftp.login("harishgnaik@gmail.com", "")
ftp.cwd("/D8V9NUWP/XNAS-20260204-NAPHKYS6FS")

files = ftp.nlst()
for file in files:
    output_file = Path(f"/home/hnaik/ws/rootmethod/substrate/extdata/{file}")
    output_file.parent.mkdir(parents=True, exist_ok=True)
    ftp.retrbinary(f"RETR {file}", open(output_file, "wb").write)
ftp.quit()
