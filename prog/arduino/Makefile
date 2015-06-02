TARGET_ARCH:=$(shell cat ino.ini | grep '^board-model=' | head -n 1 | sed 's/^board-model=\(.*\)$$/\1/')
FIRMWARE:=.build/$(TARGET_ARCH)/firmware.hex

#all: upload
all: build

.PHONY: $(FIRMWARE)
$(FIRMWARE): $(MAKEFILE_LIST)
	ino build

.PHONY: build
build: $(FIRMWARE)

.PHONY: upload
upload: $(FIRMWARE)
	ino upload

.PHONY: clean
clean:
	ino clean
