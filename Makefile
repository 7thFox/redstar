build: grammar
	cd src && \
		go build -o bin/redstar main.go

testanno: build
	src/bin/redstar examples/testanno/main.red

grammar:
	rm -rf src/parser
	antlr4 -Dlanguage=Go -Xexact-output-dir -o src/parser src/Redstar.g4
