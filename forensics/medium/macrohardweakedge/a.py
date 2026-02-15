import os
import base64

# 検索する文字列
search_str = "ctf"
# base64エンコードして小文字に変換
search_bytes = base64.b64encode(search_str.encode()).lower()

# 検索するフォルダ
target_folder = "./"  # 必要に応じて変更

for root, dirs, files in os.walk(target_folder):
    for file in files:
        file_path = os.path.join(root, file)
        try:
            with open(file_path, "rb") as f:
                data = f.read()
            # ファイルデータも小文字に変換
            data_lower = data.lower()
            start = 0
            while True:
                idx = data_lower.find(search_bytes, start)
                if idx == -1:
                    break
                print(f"Found '{search_bytes.decode()}' in {file_path} at position {idx}")
                start = idx + 1
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
