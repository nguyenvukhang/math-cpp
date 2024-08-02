current: run

prebuild:
	@rm -rf mini.*
	@mkdir -p $(BUILD_DIR)

build: prebuild
	$(CMAKE_CONFIG)
	$(CMAKE_BUILD)
	@ln -sf $(BUILD_DIR)/compile_commands.json .

run: clean build
	@echo '──────────'
	@-$(BIN) $(RUNTIME_ARGS) -R=dev.tex --tex --label
	@echo '──────────'

test: prebuild
	$(CMAKE_CONFIG) -DCMAKE_BUILD_TYPE=Test
	$(CMAKE_BUILD)
	ctest --test-dir $(BUILD_DIR) \
		|| ctest --rerun-failed --output-on-failure --test-dir $(BUILD_DIR)

std:
	-@open -a Skim mini.pdf
	-@open -a Preview $(TEXTBOOKS)

clean:
	@rm -rf *.aux *.out *.pdf $(BUILD_DIR)
	@find * -name '*.log' -o -name '*.bak' | xargs rm -f

# ────────────────────────────────────────────────────────────────────
MAKEFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_DIR  := $(dir $(MAKEFILE_PATH))
BUILD_DIR     := .build
M             := cargo run -Fdev --
M             := minimath
ICLOUD_DIR    := /Users/khang/Library/Mobile\ Documents/com~apple~CloudDocs
ROOT_TEX_CI   := all.tex
M_VER         := $$(cargo run --quiet -- version)

# C++
BIN := $(BUILD_DIR)/minimath
CMAKE_CONFIG := cmake -S src -B$(BUILD_DIR)
# CMAKE_CONFIG += -DCMAKE_BUILD_TYPE=Release
CMAKE_BUILD := cmake --build $(BUILD_DIR) -j8

include .github/profiles.mk
