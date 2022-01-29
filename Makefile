build:
	dotnet build src/

rebuild: grammar
	dotnet clean src/
	dotnet build src/

run:
	dotnet run --project src/Redstar.Frontend/ -- examples/scratch/main.red

testanno:
	dotnet run --project src/Redstar.Frontend/ -- examples/testanno/main.red

testscope:
	dotnet run --project src/Redstar.Frontend/ -- --printscope examples/testscope/main.red

testsymbols:
	dotnet run --project src/Redstar.Frontend/ -- --printsymbols examples/testscope/main.red

testrefs:
	dotnet run --project src/Redstar.Frontend/ -- --printreferences examples/testscope/main.red

grammar:
	antlr4 \
		-Dlanguage=CSharp \
		-package Redstar.Parse \
		src/Redstar.Parse/Redstar.g4

Redstar.Parse: grammar
	dotnet clean build src/Redstar.Parse

Redstar.Symbols:
	dotnet build src/Redstar.Symbols

Redstar.Frontend:
	dotnet build src/Redstar.Symbols
