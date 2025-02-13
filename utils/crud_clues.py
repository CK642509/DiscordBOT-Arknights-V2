import os
import re
from datetime import date


class ClueProcessor:
    INPUT_FILENAME = "input.txt"
    DETAIL_FILENAME = "detail.txt"
    USER_DATA_FILENAME = "_user_data.txt"
    RESULT_FILENAME = "results.txt"

    script_dir = os.path.dirname(os.path.abspath(__file__))
    input_path = os.path.join(script_dir, "exchange", INPUT_FILENAME)
    detail_path = os.path.join(script_dir, "exchange", DETAIL_FILENAME)
    user_data_path = os.path.join(script_dir, "exchange", USER_DATA_FILENAME)
    result_path = os.path.join(script_dir, "exchange", RESULT_FILENAME)

    @staticmethod
    def get_users() -> str:
        """取得所有人的名單"""
        with open(
            ClueProcessor.user_data_path, "r", errors="replace", encoding="cp950"
        ) as f:
            text = f.read()
            return "\n".join(text.split("\n")[2:])

    @staticmethod
    def get_clues() -> str:
        """取得所有人的線索資訊 (for START.exe)"""
        with open(ClueProcessor.input_path, "r") as f:
            return f.read()

    @staticmethod
    def get_detail() -> str:
        """取得所有人的詳細線索資訊"""
        with open(ClueProcessor.detail_path, "r") as f:
            return f.read()
        
    @staticmethod
    def get_result() -> str:
        """取得計算的結果"""
        with open(ClueProcessor.result_path, "r", errors="replace", encoding="cp950") as f:
            return f.read()

    @staticmethod
    def update_clue(user_id: int, clue: str) -> None:
        """更新線索資訊"""
        # get previous clues
        text = ClueProcessor.get_clues()
        clue_list = text.split("\n")[:8]

        # get user list (lowercase)
        user_list = ClueProcessor.get_users().split("\n")
        user_list = list(map(lambda x: x.lower(), user_list))

        # get the row number that needs to be updated
        idx = user_list.index(user_id)

        # set new clues
        new_clue = clue.strip()
        clue_list[idx] = new_clue
        new_clues = "\n".join(clue_list)

        with open(ClueProcessor.input_path, "w") as f:
            f.write(new_clues)

        # record details
        detail = ClueProcessor.get_detail()

        with open(ClueProcessor.detail_path, "w") as f:
            detail_list = detail.split("\n")
            detail_list[idx] = f"{date.today()} {user_list[idx]:<7} {new_clue}"
            new_detail = "\n".join(detail_list)
            f.write(new_detail)

    @staticmethod
    def validate_clue(clue: str) -> bool:
        """驗證線索格式"""
        # check if only contains 0-7
        return bool(re.match("/[^0-7]/", clue))

    @staticmethod
    def format_clue(clue: str) -> str:
        clue = clue.strip()
        result = re.split("\s+", clue)
        if len(result) == 1:
            return f"{clue} 0"
        elif len(result) == 2:
            return f"{result[0]} {result[1]}"
        else:
            return ""


if __name__ == "__main__":
    print(ClueProcessor.get_users())
    print(ClueProcessor.get_clues())
    print(ClueProcessor.get_detail())
    print(ClueProcessor.validate_clue("0 1"))
    print(ClueProcessor.format_clue("0 1"))
    ClueProcessor.update_clue("test", "0 1")
    print(ClueProcessor.get_clues())
    print(ClueProcessor.get_detail())

    # import subprocess
    # import re
    # from datetime import date
    # from sys import platform
    # import os

    # script_dir = os.path.dirname(os.path.abspath(__file__))
    # exe_dir = os.path.join(script_dir, "exchange")

    # def exchange():
    #     if platform == "linux" or platform == "linux2":
    #         subprocess.run(["./utils/exchange/main"], cwd=exe_dir)
    #     elif platform == "win32":
    #         subprocess.run(["./utils/exchange/main.exe"], cwd=exe_dir)

    # exchange()
