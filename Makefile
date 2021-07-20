# debugparams = -printsymbols
debugparams = -printstrings

build-release:
	@[[ -d bin ]] || mkdir bin
	gcc src/*.c -Wall -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

build:
	@[[ -d bin ]] || mkdir bin
	gcc src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

test1: build
	bin/redstar -debug $(debugparams) tests/test1

testanno: build
	bin/redstar -debug $(debugparams) tests/testanno