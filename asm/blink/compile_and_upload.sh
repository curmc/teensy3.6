#!/usr/bin/env sh

PREFIX="/usr/arm-linux-gnu/gcc-arm-none-eabi-8-2019-q3-update/bin"

compile_assembly () {
  pre=${1%.*}
  $PREFIX/arm-none-eabi-as -g -mcpu=cortex-m4 -mthumb -o "$pre.o" "$1"
  $PREFIX/arm-none-eabi-ld -T layout.ld -o "$pre.elf" "$pre.o"
  $PREFIX/arm-none-eabi-objcopy -O ihex -R .eeprom "$pre.elf" "$pre.hex"
  if test $# -gt 1; then
    echo "Reset teensy now"
    teensy_loader_cli -w --mcu=mk20dx256 "$pre.hex"
  fi
}

if test $# -gt 0; then
  compile_assembly $1 $2
else
  echo "No input"
fi
