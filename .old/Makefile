.PHONY: test
debugparams = -printsymbols
# debugparams = -printstrings

build-release:
	@[[ -d bin ]] || mkdir bin
	gcc main.c src/*.c -Wall -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

build:
	@[[ -d bin ]] || mkdir bin
	gcc main.c src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

build-tests:
	@[[ -d bin ]] || mkdir bin
	gcc test/*.c src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar-test

test: build-tests
	bin/redstar-test 2> /dev/null

test-verbose: build-tests
	bin/redstar-test
