SHELL=/bin/sh

TEST_DIR=test
JS_DIR=public/js
JS_FILES:=$(shell find "$(JS_DIR)" -name '*.js') $(shell find "$(TEST_DIR)" -name '*.js') server.js config.js

APTGI=sudo apt-get -y --force-yes install

all: setup run

setup: deps

run: lint test run-server

run-server:
	nodemon server/server.js

deps:
	$(APTGI) nodejs nodejs-legacy npm 
	sudo npm install bower jshint nodemon grunt-cli karma -g
deps-local:
	npm install
	bower install --allow-root

dist: deps-local
	grunt build

test:
	grunt karma

lint:
	jshint $(JS_FILES)

.PHONY: test setup node deps lint run run-server all dist
