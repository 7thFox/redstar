build:
	dotnet build src/

rebuild: grammar
	dotnet clean
	dotnet build src/

testanno: build
	dotnet run --project src/ -- examples/testanno/main.red

grammar:
	rm -rf src/parser
	antlr4 \
		-Dlanguage=CSharp \
		-Xexact-output-dir \
		-o src/parser \
		-visitor \
		-package Redstar.Parser \
		src/Redstar.g4
