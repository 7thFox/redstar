
build:
	@[[ -d bin ]] || mkdir bin
	# gcc src/*.c -D _DEFAULT_SOURCE -o bin/redstar
	gcc src/*.c -Wall -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

test1: build
	@bin/redstar -debug -printlex tests/test1