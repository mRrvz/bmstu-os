import os
import sys

filenames = {
    "cmdline": "file",
    "cwd": "link",
    "environ": "file",
    "exe": "link",
    "fd": "fd",
    "maps": "file",
    "root": "link",
    "stat": "file",
    "statm": "file",
}


def print_file(path):
    with open(os.path.join(path), "r") as f:
        tmp = f.read()
        print(tmp)
        return tmp


def print_link(path):
    tmp = os.readlink(path)
    print(tmp)
    return tmp


def print_fd(path):
    _, _, dirfiles = next(os.walk(path))
    tmp = ""
    for fd in dirfiles:
        tmp += f"FD ({fd}) ===> {os.readlink(os.path.join(path, fd))}\n"

    print(tmp)
    return tmp


def main():
    proc_path = os.path.join("/proc", sys.argv[1])
    raw = ""

    for key in filenames:
        fpath = os.path.join(proc_path, key)
        raw += f"\n\n[{key.upper()}]\n"

        print(key.upper())

        if filenames[key] == "file":
            raw += print_file(fpath)
        elif filenames[key] == "link":
            raw += print_link(fpath)
        elif filenames[key] == "fd":
            raw += print_fd(fpath)

        input()

    with open(f"proc_{sys.argv[1]}", "w") as f:
        f.write(raw)


if __name__ == "__main__":
    main()