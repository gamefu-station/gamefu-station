#!/usr/bin/env bash

set -e

mkdir -p bin

if [ -z ${flags+x} ]; then flags=devcc; fi
if [ -z ${gfusx_platform+x} ]; then gfusx_platform=GLFW; fi

echo "Building GameFU HX Utility..."
cc -o bin/hx gfu-hx/hx.c @${flags}.txt

echo "Building GameFU Assembler ISel Table Generator..."
./bin/hx ./iselgen/isel.txt -i -n isel > ./iselgen/isel_source.h
cc -o bin/iselgen iselgen/iselgen.c @${flags}.txt

echo "Running GameFU Assembler ISel Table Generator..."
./bin/iselgen
rm bin/iselgen # don't need it for now, de-clutter while we're here

echo "Building libgfu-opcodes.a..."
cc -o bin/gfu-opcodes.o -c gfu-opcodes/register.c @${flags}.txt
ar rcs bin/libgfu-opcodes.a bin/gfu-opcodes.o
rm bin/gfu-opcodes.o # don't need it for now, de-clutter while we're here

echo "Building GameFU Assembler..."
cc -o bin/as gfu-as/as.c @${flags}.txt

# echo "Building GameFU C Compiler..."
# cc -o fucc src/fucc.c @${flags}.txt

echo "Building GameFU Station BIOS..."
./bin/as -o bin/bios.gfu lib/bios/boot.fus
./bin/hx bin/bios.gfu -i -n gfusx_default_bios > ./gfu-sx/default_bios.h
rm bin/bios.gfu # once it's in the include file we don't need the raw binary, de-clutter while we're here

gfusx_platform_lib=""
if [ "$gfusx_platform" = "GLFW" ]; then
    gfusx_platform_lib=bin/libglfw3.a
    if [ ! -f bin/libglfw3.a ]; then
        echo "Building GLFW3..."
        cc -o bin/glfw3.o -c gfu-sx/glfw3/glfw3.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        cc -o bin/glfw3_linux.o -c gfu-sx/glfw3/glfw3_linux.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        cc -o bin/glfw3_null.o -c gfu-sx/glfw3/glfw3_null.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        cc -o bin/glfw3_osmesa.o -c gfu-sx/glfw3/glfw3_osmesa.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        cc -o bin/glfw3_wl.o -c gfu-sx/glfw3/glfw3_wl.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        cc -o bin/glfw3_x11.o -c gfu-sx/glfw3/glfw3_x11.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
        ar rcs bin/libglfw3.a bin/glfw3.o bin/glfw3_linux.o bin/glfw3_null.o bin/glfw3_osmesa.o bin/glfw3_wl.o bin/glfw3_x11.o
        rm bin/glfw3.o bin/glfw3_linux.o bin/glfw3_null.o bin/glfw3_osmesa.o bin/glfw3_wl.o bin/glfw3_x11.o
    fi
else
    gfusx_platform_lib=""
fi

echo "Building GFUSX Emulator..."
cc -o bin/sx gfu-sx/sx.c @${flags}.txt -DGFUSX_PLATFORM_$gfusx_platform -Ivendor/glfw3/include ${gfusx_platform_lib} -lm
