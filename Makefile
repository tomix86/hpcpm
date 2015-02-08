VERSION?=dev

build:
	mkdir -p dist
	g++ -Wall -o dist/main-$(VERSION).exe src/main.cpp

dist: build

test: