import subprocess
import sys


def run(executable, text="", args=(), expected=0):
    result = subprocess.run(
        [executable, *args], input=text, text=True, capture_output=True, timeout=30
    )
    if result.returncode != expected:
        raise AssertionError((args, result.returncode, result.stdout, result.stderr))
    return result.stdout + result.stderr


def seed():
    address = ["Минск", "Гикало", "9", "220013"]
    return ["1", "1", "Отправитель", *address,
            "1", "2", "Получатель", *address, "2", "13", *address]


def main():
    executable = sys.argv[1]
    assert "postal --demo" in run(executable, args=("--help",))
    run(executable, args=("--unknown",), expected=2)
    run(executable, args=("--demo", "extra"), expected=2)
    assert "Уведомление отмечено прочитанным" in run(executable, args=("--demo",))
    run(executable)
    run(executable, "0\n")
    run(executable, "1\n7\n")
    lines = seed()
    variants = [(1, "L1", [20, 5]), (2, "P1", [1500, 200, 100]),
                (3, "R1", [20, 5, 50]), (4, "E1", [20, 5, 24, 200]),
                (5, "F1", [1500, 200, 100, 50, 1]),
                (6, "I1", [1500, 200, 100, 10000, 100])]
    for kind, number, extra in variants:
        lines.extend(["3", "13", str(kind), number, "1", "2", *map(str, extra), "4", number])
    lines.extend(["5", "L1", "4", "L1", "6", "L1", "4", "L1",
                  "7", "2", "Брест", "Советская", "1", "224000", "8", "9", "0", "0"])
    output = run(executable, "\n".join(lines) + "\n")
    for expected in ["Клиентов: 2", "Отправлений: 6", "Статус: Принято",
                     "Статус: В пути", "Статус: Доставлено", "Адрес клиента обновлён"]:
        assert expected in output, expected
    errors = [
        ["abc", "1 trailing", "99"], ["4", "missing"], ["3", "99"], ["7", "99"],
        ["3", "13", "7"], ["3", "13", "1", "X", "99", "2"],
        ["3", "13", "1", "X", "1", "2", "0", "5"],
        ["3", "13", "5", "X", "1", "2", "1500", "200", "100", "50", "2"],
        ["9", "6"], ["1", "-1", "Имя", "Минск", "Гикало", "9", "220013"],
        ["2", "0", "Минск", "Гикало", "9", "220013"],
    ]
    for commands in errors:
        text = "\n".join([*seed(), *commands, "0"]) + "\n"
        assert "Ошибка:" in run(executable, text), commands
    for number in range(1, 6):
        run(executable, f"9\n{number}\n0\n")
    print("CLI smoke checks passed")


if __name__ == "__main__":
    main()
