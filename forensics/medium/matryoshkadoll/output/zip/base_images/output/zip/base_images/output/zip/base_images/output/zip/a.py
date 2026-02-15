import os

search_str = "picoCTF{"
search_lower = search_str.lower()

target_folder = "./"

for root, dirs, files in os.walk(target_folder):
    for file in files:
        file_path = os.path.join(root, file)
        try:
            with open(file_path, "r", errors="replace") as f:
                content = f.read()  # ファイル全体を1つの文字列として読み込む
            content_lower = content.lower()
            idx = content_lower.find(search_lower)
            if idx != -1:
                # 見つかった場合、前後少しを切り出して表示
                start = max(0, idx - 10)
                end = idx + 100  # フラグの長さ分余裕を持たせる
                snippet = content[start:end]
                print(f"Found in {file_path}: {snippet}")
        except Exception as e:
            print(f"Error reading {file_path}: {e}")
