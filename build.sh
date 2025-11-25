#!/usr/bin/env bash

set -e

mkdir -p bin

if [ -z ${flags+x} ]; then flags=devcc; fi
if [ -z ${gfusx_platform+x} ]; then gfusx_platform=GLFW; fi

echo "Building GameFU HXD Utility..."
cc -o hxd src/hxd.c @${flags}.txt

echo "Building GameFU Assembler ISel Table Generator..."
./hxd ./lib/fuasm.isel -i -n isel > ./iselgen/isel_source.h
cc -o bin/iselgen iselgen/iselgen.c @${flags}.txt

echo "Running GameFU Assembler ISel Table Generator..."
./bin/iselgen
rm bin/iselgen # don't need it for now, de-clutter while we're here

echo "Building GameFU Assembler..."
cc -o fuasm src/fuasm.c @${flags}.txt

# echo "Building GameFU C Compiler..."
# cc -o fucc src/fucc.c @${flags}.txt

# echo "Building GameFU Compiler Collection..."
# cc -o fucc src/fucc2.c @${flags}.txt

echo "Building GameFU Station BIOS..."
./fuasm -o bios.gfu lib/bios/boot.fus
./hxd bios.gfu -i -n gfusx_default_bios > ./lib/gfusx/default_bios.h
# rm bios.gfu # once it's in the include file we don't need the raw binary, de-clutter while we're here

gfusx_platform_lib=""
if [ "$gfusx_platform" = "GLFW" ] && [ ! -f libglfw3.a ]; then
    echo "Building GLFW3..."
    cc -o glfw3.o -c lib/glfw3/glfw3.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    cc -o glfw3_linux.o -c lib/glfw3/glfw3_linux.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    cc -o glfw3_null.o -c lib/glfw3/glfw3_null.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    cc -o glfw3_osmesa.o -c lib/glfw3/glfw3_osmesa.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    cc -o glfw3_wl.o -c lib/glfw3/glfw3_wl.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    cc -o glfw3_x11.o -c lib/glfw3/glfw3_x11.c -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
    ar rcs libglfw3.a glfw3.o glfw3_linux.o glfw3_null.o glfw3_osmesa.o glfw3_wl.o glfw3_x11.o
    rm glfw3.o glfw3_linux.o glfw3_null.o glfw3_osmesa.o glfw3_wl.o glfw3_x11.o
    gfusx_platform_lib=libglfw3.a
else
    gfusx_platform_lib="NULL"
fi

echo "Building GFUSX Emulator..."
cc -o gfusx src/gfusx.c @${flags}.txt -DGFUSX_PLATFORM_$gfusx_platform -Ivendor/glfw3/include libglfw3.a -lm
