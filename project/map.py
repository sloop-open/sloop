#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
map.py v5
MDK style analyzer (ELF + MAP)

修正版：
1. 支持 GNU ld map 两行格式
2. 文件级统计接近 MDK
3. 函数级 TOP10
4. 全局变量/数组 TOP10
5. Stack 固定 1KB

使用：
python map.py project.elf project.map
"""

import argparse
import subprocess
import re
import sys
import os
from collections import defaultdict

STACK_RESERVED = 1024


# ------------------------------------------------------------
# utils
# ------------------------------------------------------------
def run(cmd):
    try:
        r = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            encoding="utf-8",
            errors="ignore",
            check=True
        )
        return r.stdout
    except Exception as e:
        print("Command failed:", " ".join(cmd))
        print(e)
        sys.exit(1)


def line():
    print("=" * 78)


def human(v):
    if v >= 1024:
        return f"{v/1024:.2f} KB"
    return f"{v} B"


def kb(v):
    return f"{v/1024:.2f}kB"


# ------------------------------------------------------------
# ELF sections
# ------------------------------------------------------------
def parse_sections(elf):
    txt = run(["arm-none-eabi-size", "-A", elf])

    sec = {}
    for s in txt.splitlines():
        m = re.match(r"^([.\w]+)\s+(\d+)\s+", s.strip())
        if m:
            sec[m.group(1)] = int(m.group(2))

    return {
        "text": sec.get(".text", 0),
        "rodata": sec.get(".rodata", 0),
        "data": sec.get(".data", 0),
        "bss": sec.get(".bss", 0),
    }


# ------------------------------------------------------------
# ELF symbols
# ------------------------------------------------------------
def parse_nm(elf):
    txt = run([
        "arm-none-eabi-nm",
        "--print-size",
        "--size-sort",
        "--demangle",
        elf
    ])

    funcs = []
    vars_ = []

    for s in txt.splitlines():
        m = re.match(
            r"^([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+([A-Za-z])\s+(.+)$",
            s.strip()
        )
        if not m:
            continue

        size = int(m.group(2), 16)
        typ = m.group(3)
        name = m.group(4).strip()

        if size == 0:
            continue

        if typ in ("T", "t"):
            funcs.append((size, name))

        elif typ in ("B", "b", "D", "d"):
            vars_.append((size, name))

    funcs.sort(reverse=True)
    vars_.sort(reverse=True)

    return funcs, vars_


# ------------------------------------------------------------
# MAP parser (supports two-line GNU ld style)
# ------------------------------------------------------------
def parse_map(mapfile):

    files = defaultdict(lambda: {
        "code": 0,
        "ro": 0,
        "rw": 0,
        "zi": 0
    })

    pending_section = None

    with open(mapfile, "r", encoding="utf-8", errors="ignore") as f:

        for raw in f:
            s = raw.rstrip()

            # ------------------------------------------------
            # line1: .text.foo
            # ------------------------------------------------
            m1 = re.match(r"^\s*(\.[^\s]+)\s*$", s)
            if m1:
                pending_section = m1.group(1)
                continue

            # ------------------------------------------------
            # line2:
            # 0x08000100 0x120 file.o
            # ------------------------------------------------
            m2 = re.match(
                r"^\s*0x[0-9a-fA-F]+\s+0x([0-9a-fA-F]+)\s+(.+)$",
                s
            )

            if m2 and pending_section:
                sec = pending_section
                size = int(m2.group(1), 16)
                path = m2.group(2).strip()
                pending_section = None

            else:
                # 单行格式
                m3 = re.match(
                    r"^\s*(\.[^\s]+)\s+0x[0-9a-fA-F]+\s+0x([0-9a-fA-F]+)\s+(.+)$",
                    s
                )

                if not m3:
                    continue

                sec = m3.group(1)
                size = int(m3.group(2), 16)
                path = m3.group(3).strip()

            if ".o" not in path:
                continue

            path = path.split("(")[0]
            name = os.path.basename(path)

            low = name.lower()

            if low.startswith("crt"):
                continue

            if sec.startswith(".text"):
                files[name]["code"] += size

            elif sec.startswith(".rodata"):
                files[name]["ro"] += size

            elif sec.startswith(".data"):
                files[name]["rw"] += size

            elif sec.startswith(".bss"):
                files[name]["zi"] += size

    result = []

    for k, v in files.items():

        flash = v["code"] + v["ro"] + v["rw"]
        ram = v["rw"] + v["zi"]

        result.append((
            flash,
            ram,
            v["code"],
            v["ro"],
            v["rw"],
            v["zi"],
            k
        ))

    result.sort(reverse=True)
    return result


# ------------------------------------------------------------
# print
# ------------------------------------------------------------
def print_files(arr, top):
    line()
    print("TOP10 Object File Usage")
    line()

    print(
        f"{'Rank':<5}"
        f"{'Code':>8}"
        f"{'RO':>8}"
        f"{'RW':>8}"
        f"{'ZI':>8}"
        f"{'Flash':>10}"
        f"{'RAM':>10}   File"
    )
    print()

    for i, item in enumerate(arr[:top], 1):
        flash, ram, code, ro, rw, zi, name = item

        print(
            f"{i:<5}"
            f"{code:8d}"
            f"{ro:8d}"
            f"{rw:8d}"
            f"{zi:8d}"
            f"{flash:10d}"
            f"{ram:10d}   {name}"
        )
    print()


def print_sym(title, arr, top):
    line()
    print(title)
    line()

    print(f"{'Rank':<6}{'Size':>10}   Name")
    print()

    for i, (size, name) in enumerate(arr[:top], 1):
        print(f"{i:<6}{human(size):>10}   {name}")

    print()


# ------------------------------------------------------------
# main
# ------------------------------------------------------------
def main():

    ap = argparse.ArgumentParser()
    ap.add_argument("elf")
    ap.add_argument("map")
    ap.add_argument("--top", type=int, default=10)
    args = ap.parse_args()

    sec = parse_sections(args.elf)
    funcs, vars_ = parse_nm(args.elf)
    files = parse_map(args.map)

    code = sec["text"]
    ro = sec["rodata"]
    rw = sec["data"]
    zi = sec["bss"]

    total_rw = rw + zi
    total_ram = total_rw + STACK_RESERVED
    total_rom = code + ro + rw

    print()
    line()
    print("Image component sizes")
    print()

    print(f"{'Code (inc. data)':>18} {'RO Data':>10} {'RW Data':>10} {'ZI Data':>10}")
    print()
    print(f"{code:18d} {ro:10d} {rw:10d} {zi:10d}")
    print()

    line()
    print("Memory Summary")
    line()

    print(f"{'Stack Reserved':<24}: {STACK_RESERVED:8d} ({kb(STACK_RESERVED)})")
    print(f"{'Static RAM (RW+ZI)':<24}: {total_rw:8d} ({kb(total_rw)})")
    print(f"{'Total RAM Footprint':<24}: {total_ram:8d} ({kb(total_ram)})")
    print(f"{'Total ROM Size':<24}: {total_rom:8d} ({kb(total_rom)})")
    print()

    line()
    print("Totals")
    line()

    print(f"Total RO  Size (Code + RO Data)       {code+ro:8d} ({kb(code+ro)})")
    print(f"Total RW  Size (RW Data + ZI Data)    {total_rw:8d} ({kb(total_rw)})")
    print(f"Total ROM Size (+RW Init Data)        {total_rom:8d} ({kb(total_rom)})")
    print()

    print_files(files, args.top)
    print_sym("TOP10 Code Functions", funcs, args.top)
    print_sym("TOP10 RAM Globals / Arrays", vars_, args.top)


if __name__ == "__main__":
    main()