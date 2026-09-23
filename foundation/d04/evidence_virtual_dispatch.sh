#!/usr/bin/env bash
set -euo pipefail

root="$(cd "$(dirname "$0")/.." && pwd)"
src="$root/d04/08_virtual_dispatch_controlled.cpp"
include="$root/common"
obj="/tmp/fd_d04_virtual_dispatch.o"
asm="/tmp/fd_d04_virtual_dispatch.s"

c++ -std=c++17 -O0 -fno-inline -I"$include" -c "$src" -o "$obj"
c++ -std=c++17 -O0 -fno-inline -I"$include" -S "$src" -o "$asm"

echo "== L3 symbols (this compiler, -O0 -fno-inline) =="
nm -C "$obj" | rg 'without_virtual::|with_virtual::'

echo "== L3 call sites =="
rg -n 'call|bl|blr|evaluate' "$asm"

echo "Interpretation:"
echo "- a call/bl naming without_virtual::Base::evaluate is a direct call;"
echo "- an indirect call/blr after loading a function address is virtual dispatch."
echo "This output describes this compiler/ABI/flag combination, not every C++ implementation."
