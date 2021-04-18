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

stat = (
    "pid",
    "comm",
    "state",
    "ppid",
    "pgrp",
    "session",
    "tty_nr",
    "tpgid",
    "flags",
    "minflt",
    "cminflt",
    "majflt",
    "cmajflt",
    "utime",
    "stime",
    "cutime",
    "cstime",
    "priority",
    "nice",
    "num_threads",
    "itrealvalue",
    "starttime",
    "vsize",
    "rss",
    "rsslim",
    "startcode",
    "endcode",
    "startstack",
    "kstkesp",
    "kstkeip", # 30
    "signal",
    "blocked",
    "sigignore",
    "sigcatch",
    "wchan",
    "nswap",
    "cnswap",
    "exit_signal",
    "processor",
    "rt_priority",
    "policy",
    "delayacct_blkio_ticks",
    "guest_time",
    "cguest_time",
    "start_data",
    "end_data",
    "start_brk",
    "arg_start",
    "arg_end",
    "env_start",
    "env_end",
    "exit_code"
)


statm = (
    "size",
    "resident",
    "shared",
    "text",
    "lib",
    "data",
    "dt"
)

def print_file(path):
    with open(os.path.join(path), "r") as f:
        if path.endswith("statm"):
            raw = f.read()
            tmp = ""
            for i, line in enumerate(raw.split()):
                tmp += f"{statm[i]}: {line}\n"
        elif path.endswith("stat"):
            raw = f.read()
            tmp = ""
            for i, line in enumerate(raw.split()):
                tmp += f"{stat[i]}: {line}\n"
        else:
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
    proc_pid = sys.argv[1] if len(sys.argv) > 1 else str(os.getpid())
    proc_path = os.path.join("/proc", proc_pid)
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

    with open(f"proc_{proc_pid}", "w") as f:
        f.write(raw)


if __name__ == "__main__":
    main()