
build:
	@[[ -d bin ]] || mkdir bin
	gcc src/*.c -Wall -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

debug:
	@[[ -d bin ]] || mkdir bin
	gcc src/*.c -Wall -rdynamic -pedantic-errors -D _DEFAULT_SOURCE -o bin/redstar

test1: debug
	@bin/redstar -debug -printlex -printtree tests/test1

testanno: debug
	@bin/redstar -debug -printlex -printtree tests/annotest