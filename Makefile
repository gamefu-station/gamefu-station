CFLAGS=@cflags.txt

.PHONY: all
all: \
	bin/libgfu-common.a \
	bin/libgfu-opcodes.a

.PHONY: clean
clean:
	rm -rf ./bin

GFU_COMMON_O=$(patsubst gfu-common/%.c,bin/gfu-common/%.o,$(wildcard gfu-common/*.c))
GFU_COMMON_H=$(wildcard gfu-common/include/**/*.h)
bin/libgfu-common.a: $(GFU_COMMON_O)
	@mkdir -p bin
	ar rcs $@ $^
	@echo "> Built libgfu-common.a"

bin/gfu-common/%.o: gfu-common/%.c $(GFU_COMMON_H)
	@mkdir -p bin/gfu-common
	cc -o $@ -c $< -Igfu-common/include $(CFLAGS)

GFU_OPCODES_O=$(patsubst gfu-opcodes/%.c,bin/gfu-opcodes/%.o,$(wildcard gfu-opcodes/*.c))
GFU_OPCODES_H=$(wildcard gfu-opcodes/include/**/*.h)
bin/libgfu-opcodes.a: $(GFU_OPCODES_O)
	@mkdir -p bin
	ar rcs $@ $^
	@echo "> Built libgfu-opcodes.a"

bin/gfu-opcodes/%.o: gfu-opcodes/%.c $(GFU_COMMON_H) $(GFU_OPCODES_H)
	@mkdir -p bin/gfu-opcodes
	cc -o $@ -c $< -Igfu-common/include -Igfu-opcodes/include $(CFLAGS)
