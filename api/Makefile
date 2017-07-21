VERSION?=0.0.0+dev

PROJ_NAME=api
PROJ_DIR=hpcpm

TEST_DIR=tests

VENV?=venv

VENV_BIN=$(VENV)/bin
SYSTEM_PYTHON=$(shell which python3)
PYTHON_VER=$(shell ls -la /usr/bin/python3 | awk '{print $$11}')
VENV_PYLIB=lib/python3.4

PYTHON=$(VENV_BIN)/python
PIP=$(VENV_BIN)/pip
PEP8=$(VENV_BIN)/pep8
PYLINT=$(VENV_BIN)/pylint
PYFLAKES=$(VENV_BIN)/pyflakes
PYTEST=$(VENV_BIN)/py.test
PYTESTOPTS=--cov $(PROJ_DIR) -v --cov-config coverage.rc --cov-report html --cov-report xml --cov-report term

VIRTUALENV=virtualenv -p $(SYSTEM_PYTHON)

PWD:=$(shell pwd)

PY_FILES:=$(shell find "$(PROJ_DIR)" -name '*.py') $(shell find "$(TEST_DIR)" -name '*.py')

PYTHONPATH=$(PWD)
export PYTHONPATH

export VERSION

APTI=sudo apt-get -y --force-yes install


define deb-dep
debs: $(1)
$(1): $(2)
$(2):
	$(APTI) $(1)
.PHONY: $(1)
endef


.PHONY: all debs venv devenv rebuild_venv dist pep8 lint_pyflakes lint_pylint lint \
    test install run clean clean_venv clean_all unit_tests


all: dist test

debs:
ifneq ($(wildcard /usr/bin/apt-get),)
    $(eval $(call deb-dep, python3-dev, /usr/include/$(PYTHON_VER)/pyconfig.h))
    $(eval $(call deb-dep, python3-setuptools, /usr/bin/pip))
    $(eval $(call deb-dep, python-virtualenv, /usr/bin/virtualenv))
endif

$(VENV_BIN)/activate: dev_requirements.txt requirements.txt
	test -d $(VENV) || $(VIRTUALENV) $(VENV)
	$(PYTHON) -m pip install -U pip
	$(PIP) install -Ur dev_requirements.txt
	$(PIP) install -Ur requirements.txt
	touch $(VENV_BIN)/activate

venv: $(VENV_BIN)/activate

devenv: debs venv

rebuild_venv: clean_venv venv

dist: devenv
	sed -i "s/__version__ = .*/__version__ = \"$(VERSION)\"/g" $(PROJ_DIR)/$(PROJ_NAME)/version.py
	$(PYTHON) setup.py sdist --formats=gztar,zip bdist_wheel

pep8: venv
	$(PEP8) --config=pep8.cfg $(PY_FILES)

lint_pyflakes: venv
	$(PYFLAKES) $(PY_FILES)

lint_pylint: venv
	$(PYLINT) --rcfile=pylintrc -r n $(PROJ_DIR) $(TEST_DIR)

lint: pep8 lint_pyflakes lint_pylint

test: venv lint unit_tests

unit_tests:
	$(PYTEST) $(PYTESTOPTS) $(TEST_DIR)

install: venv
	$(PIP) install -U $(shell find ./dist -name "*$(PROJ_NAME)*whl")

run: devenv
	. $(VENV_BIN)/activate && hpc-power-management-api $(RUN_PARAMS)

clean:
	rm -rf *.pyc
	rm -rf *.egg-info
	rm -rf .coverage
	rm -rf dist
	rm -rf build
	rm -rf htmlcov
	find $(PROJ_DIR)/ -name __pycache__ -type d -exec rm -rf {} \;
	find $(TEST_DIR)/ -name __pycache__ -type d -exec rm -rf {} \;

clean_venv:
	rm -rf $(VENV)

clean_all: clean clean_venv