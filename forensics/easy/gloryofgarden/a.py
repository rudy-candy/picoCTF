import sys
from pathlib import Path

if len(sys.argv) < 2:
    print(f"Usage: python {sys.argv[0]} <filename> [search_terms...]")
    sys.exit(1)

filename = sys.argv[1]

# デフォルトで CTF と ctf を探す
patterns = [b"CTF", b"ctf"]

# 引数で渡された検索ワードを追加
for term in sys.argv[2:]:
    patterns.append(term.encode())

data = Path(filename).read_bytes()

def hexdump_line(offset: int, chunk: bytes) -> str:
    hexview = " ".join(f"{b:02x}" for b in chunk)
    textview = "".join(chr(b) if 32 <= b < 127 else "." for b in chunk)
    return f"offset 0x{offset:08x}: {hexview:<48} |{textview}|"

for pattern in patterns:
    print(f"\n=== Searching for: {pattern.decode(errors='ignore')} ===")
    start = 0
    while True:
        idx = data.find(pattern, start)
        if idx == -1:
            break
        chunk = data[idx:idx+32]
        print(hexdump_line(idx, chunk))
        start = idx + 1
