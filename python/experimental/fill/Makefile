PROJECT_ROOT := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))
PYTHONPATH := $(PROJECT_ROOT):$(PROJECT_ROOT)lab/rl/src
MODELS_DIR := $(PROJECT_ROOT)lab/rl/models
CONFIGS_DIR := $(PROJECT_ROOT)lab/rl/experiments

.PHONY: run-jupyter
run-jupyter:
	PYTHONPATH=$(PYTHONPATH) MODELS_DIR=$(MODELS_DIR) CONFIGS_DIR=$(CONFIGS_DIR) \
		pixi run launch-jupyter
