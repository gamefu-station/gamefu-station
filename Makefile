CFLAGS=@cflags.txt

.PHONY: all
all: \
	bin/libgfu-common.a \
	bin/libgfu-opcodes.a \
	bin/iselgen

.PHONY: clean
clean:
	rm -rf ./bin

COMMON_O=$(patsubst gfu-common/%.c,bin/o/gfu-common/%.o,$(wildcard gfu-common/*.c))
COMMON_H=$(wildcard gfu-common/include/**/*.h)
bin/libgfu-common.a: $(COMMON_O)
	@mkdir -p bin
	ar rcs $@ $^
	@echo "> Built libgfu-common.a"

bin/o/gfu-common/%.o: gfu-common/%.c $(COMMON_H)
	@mkdir -p bin/o/gfu-common
	cc -o $@ -c $< -Igfu-common/include $(CFLAGS)

OPCODES_O=$(patsubst gfu-opcodes/%.c,bin/o/gfu-opcodes/%.o,$(wildcard gfu-opcodes/*.c))
OPCODES_H=$(wildcard gfu-opcodes/include/**/*.h)
bin/libgfu-opcodes.a: $(OPCODES_O)
	@mkdir -p bin
	ar rcs $@ $^
	@echo "> Built libgfu-opcodes.a"

bin/o/gfu-opcodes/%.o: gfu-opcodes/%.c $(COMMON_H) $(OPCODES_H)
	@mkdir -p bin/o/gfu-opcodes
	cc -o $@ -c $< -Igfu-common/include -Igfu-opcodes/include $(CFLAGS)

ISELGEN_O=$(patsubst iselgen/%.c,bin/o/iselgen/%.o,$(wildcard iselgen/*.c))
ISELGEN_H=$(wildcard iselgen/include/**/*.h)
bin/iselgen: $(ISELGEN_O)
	@mkdir -p bin
	cc -o $@ $< $(CFLAGS)
	@echo "> Built ISEL generator utility"

bin/o/iselgen/%.o: iselgen/%.c $(COMMON_H) $(OPCODES_H) $(ISELGEN_H)
	@mkdir -p bin/o/iselgen
	cc -o $@ -c $< -Iinclude -Igfu-common/include -Igfu-opcodes/include $(CFLAGS)
