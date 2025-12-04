CFLAGS=@cflags.txt

.PHONY: all
all: \
	bin/hx \
	bin/libgfu-common.a \
	bin/libgfu-opcodes.a \
	bin/iselgen \
	gfu-as/include/gamefu/as/x/mnemonics.h \
	gfu-as/isel_tables.c \
	bin/as \
	bin/sx

.PHONY: clean
clean:
	rm -rf ./bin
	rm -rf ./*.gfu
	rm gfu-as/include/gamefu/as/x/mnemonics.h
	rm gfu-as/isel_tables.c

HX_O:=$(patsubst gfu-hx/%.c,bin/o/gfu-hx/%.o,$(wildcard gfu-hx/*.c))
HX_H:=$(wildcard gfu-hx/include/**/*.h)
bin/hx: $(HX_O)
	@mkdir -p bin
	cc -o $@ $(HX_O) $(CFLAGS)
	@echo "> Built HX utility"
bin/o/gfu-hx/%.o: gfu-hx/%.c $(wildcard gfu-hx/*.h) $(HX_H)
	@mkdir -p bin/o/gfu-hx
	cc -o $@ -c $< -Igfu-hx/include $(CFLAGS)

COMMON_O:=$(patsubst gfu-common/%.c,bin/o/gfu-common/%.o,$(wildcard gfu-common/*.c))
COMMON_H:=$(wildcard gfu-common/include/**/*.h)
bin/libgfu-common.a: $(COMMON_O)
	@mkdir -p bin
	ar rcs $@ $(COMMON_O)
	@echo "> Built libgfu-common.a"
bin/o/gfu-common/%.o: gfu-common/%.c $(wildcard gfu-common/*.h) $(COMMON_H)
	@mkdir -p bin/o/gfu-common
	cc -o $@ -c $< -Igfu-common/include $(CFLAGS)

OPCODES_O:=$(patsubst gfu-opcodes/%.c,bin/o/gfu-opcodes/%.o,$(wildcard gfu-opcodes/*.c))
OPCODES_H:=$(wildcard gfu-opcodes/include/**/*.h)
bin/libgfu-opcodes.a: $(OPCODES_O)
	@mkdir -p bin
	ar rcs $@ $(OPCODES_O)
	@echo "> Built libgfu-opcodes.a"
bin/o/gfu-opcodes/%.o: gfu-opcodes/%.c $(wildcard gfu-opcodes/*.h) $(COMMON_H) $(OPCODES_H)
	@mkdir -p bin/o/gfu-opcodes
	cc -o $@ -c $< -Igfu-common/include -Igfu-opcodes/include $(CFLAGS)

ISELGEN_O:=$(patsubst iselgen/%.c,bin/o/iselgen/%.o,$(wildcard iselgen/*.c))
ISELGEN_H:=$(wildcard iselgen/include/**/*.h)
bin/iselgen: $(ISELGEN_O)
	@mkdir -p bin
	cc -o $@ $(ISELGEN_O) $(CFLAGS)
	@echo "> Built ISEL generator utility"
bin/o/iselgen/%.o: iselgen/%.c iselgen/isel_source.h $(wildcard iselgen/*.h) $(COMMON_H) $(OPCODES_H) $(ISELGEN_H)
	@mkdir -p bin/o/iselgen
	cc -o $@ -c $< -Iinclude -Igfu-common/include -Igfu-opcodes/include -Iiselgen/include $(CFLAGS)

iselgen/isel_source.h: iselgen/isel.txt bin/hx
	bin/hx $< -i -n isel > $@

AS_C=gfu-as/as.c
AS_O:=$(patsubst gfu-as/%.c,bin/o/gfu-as/%.o,$(subst $(AS_C),,$(subst gfu-as/isel_tables.c,,$(wildcard gfu-as/*.c))))
AS_H:=gfu-as/include/as/x/mnemonics.h $(wildcard gfu-as/include/**/*.h)
bin/as: bin/o/gfu-as/as.o bin/libgfu-as.a
	@mkdir -p bin
	cc -o $@ $^ $(CFLAGS)
	@echo "> Built GameFU Assembler"
bin/libgfu-as.a: gfu-as/isel_tables.c $(AS_O)
	@mkdir -p bin
	ar rcs $@ $(AS_O)
	@echo "> Built libgfu-as.a"
bin/o/gfu-as/%.o: gfu-as/%.c $(wildcard gfu-as/*.h) $(COMMON_H) $(OPCODES_H) $(ISELGEN_H) $(AS_H)
	@mkdir -p bin/o/gfu-as
	cc -o $@ -c $< -Iinclude -Igfu-common/include -Igfu-opcodes/include -Iiselgen/include -Igfu-as/include $(CFLAGS)
gfu-as/include/gamefu/as/x/mnemonics.h: iselgen/isel.txt bin/iselgen
	./bin/iselgen
gfu-as/isel_tables.c: iselgen/isel.txt bin/iselgen
	./bin/iselgen

bin/bios.gfu: lib/bios/boot.fus bin/as
	@mkdir -p bin
	./bin/as -o $@ lib/bios/boot.fus
	@echo "> Built GameFU Station default/official BIOS"

gfu-sx/default_bios.h: bin/bios.gfu
	./bin/hx bin/bios.gfu -i -n gfusx_default_bios > ./gfu-sx/default_bios.h

SX_C=gfu-sx/sx.c
SX_O:=$(patsubst gfu-sx/%.c,bin/o/gfu-sx/%.o,$(subst $(SX_C),,$(wildcard gfu-sx/*.c)))
SX_H:=$(wildcard gfu-sx/include/**/*.h)
bin/sx: bin/o/gfu-sx/sx.o bin/libgfu-common.a bin/libglfw3.a bin/libgfu-sx.a gfu-sx/default_bios.h $(SX_O)
	@mkdir -p bin
	cc -o $@ bin/o/gfu-sx/sx.o bin/libgfu-common.a bin/libgfu-sx.a bin/libglfw3.a -lm $(CFLAGS)
	@echo "> Built GameFU Station emulator"
bin/libgfu-sx.a: $(SX_O)
	@mkdir -p bin
	ar rcs $@ $(SX_O)
	@echo "> Built libgfu-sx.a"
bin/o/gfu-sx/%.o: gfu-sx/%.c $(wildcard gfu-sx/*.h) $(SX_H)
	@mkdir -p bin/o/gfu-sx
	cc -o $@ -c $< -Igfu-common/include -Igfu-sx/include -Ivendor/glfw3/include -DGFUSX_PLATFORM_GLFW $(CFLAGS)
bin/libglfw3.a: $(patsubst %.c,bin/o/glfw3/%.o,glfw3.c glfw3_linux.c glfw3_null.c glfw3_osmesa.c glfw3_wl.c glfw3_x11.c)
	@mkdir -p bin
	ar rcs $@ $^
	@echo "> Built libglfw3.a"
bin/o/glfw3/%.o: gfu-sx/glfw3/%.c
	@mkdir -p bin/o/glfw3
	cc -o $@ -c $< -Wno-macro-redefined -D_GNU_SOURCE -Ivendor/glfw3/include
