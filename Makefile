.PHONY: test
debugparams = -printsymbols
# debugparams = -printstrings

build-release:
	@[[ -d bin ]] || mkdir bin
	gcc main.c src/*.c -Wall -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

build:
	@[[ -d bin ]] || mkdir bin
	gcc main.c src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

test:
	@[[ -d bin ]] || mkdir bin
	gcc test/main.c src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar-test
	bin/redstar-test 2> /dev/null

# test1: build
# 	bin/redstar -debug $(debugparams) tests/test1

# testanno: build
# 	bin/redstar -debug $(debugparams) tests/testanno

# testscope: build
# 	bin/redstar -debug $(debugparams) tests/testscope