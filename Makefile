PROJECT_ROOT := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
DEBUG_DIR := $(PROJECT_ROOT)/build/gcc-debug
RELEASE_DIR := $(PROJECT_ROOT)/build/gcc-release
DATA_DIR := $(PROJECT_ROOT)/data
SCRIPTS_DIR := $(PROJECT_ROOT)/scripts

default: gcc-debug

release: gcc-release

config: config-gcc-debug

gcc-%: config-gcc-%
	bash $(SCRIPTS_DIR)/build.sh build $* gcc

config-gcc-%:
	bash $(SCRIPTS_DIR)/build.sh config $* gcc
	ln -sf $(DEBUG_DIR)/compile_commands.json .

clang-%: config-clang-%
	bash $(SCRIPTS_DIR)/build.sh build $* clang
	ln -sf $(PROJECT_ROOT)/build/clang-$*/compile_commands.json .

config-clang-%:
	bash $(SCRIPTS_DIR)/build.sh config $* clang

.PHONY: run-tests
run-tests: gcc-debug
	$(DEBUG_DIR)/all_tests

.PHONY: run-smoke
run-smoke: gcc-release
	@echo "Running smoke tests"
	PYTHONPATH=$(PROJECT_ROOT) python3 -m scripts.smoke \
		--data-dir=$(DATA_DIR)

.PHONY: run-demo
run-demo: gcc-release
	$(RELEASE_DIR)/matching_engine < $(DATA_DIR)/0/input.txt

.PHONY: build-docker
build-docker:
	$(PROJECT_ROOT)/cr/start.sh build

.PHONY: run-docker
run-docker:
	$(PROJECT_ROOT)/cr/start.sh run

.PHONY: run-jupyter
run-jupyter:
	pixi run jupyter

.PHONY: clean
clean:
	find scripts -type d -name "__pycache__"
	rm -rf build
