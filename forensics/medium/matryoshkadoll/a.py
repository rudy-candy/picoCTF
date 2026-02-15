import os

# 検索する文字列
search_str = "picoCTF{"
search_lower = search_str.lower()  # 小文字に変換して比較

# 探索するフォルダ
target_folder = "./"  # 必要に応じて変更

for root, dirs, files in os.walk(target_folder):  # サブディレクトリも含めて探索
    for file in files:
        file_path = os.path.join(root, file)
        try:
            with open(file_path, "r", errors="replace") as f:  # 文字列として読み込む
                for line_number, line in enumerate(f, start=1):
                    if search_lower in line.lower():
                        print(f"{file_path} (line {line_number}): {line.strip()}")
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
