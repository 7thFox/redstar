package main

import (
	"fmt"

	"github.com/7thFox/redstar/parser"
	"github.com/antlr/antlr4/runtime/Go/antlr"
)

type Foobar struct {
	*parser.BaseRedstarListener
}

func (this *Foobar) EnterEveryRule(ctx antlr.ParserRuleContext) {
	fmt.Println(ctx.GetText())
}

func main() {
	input, err := antlr.NewFileStream("/home/josh/src/redstar/examples/testanno/main.red") //os.Args[1])
	_ = err
	lexer := parser.NewRedstarLexer(input)
	stream := antlr.NewCommonTokenStream(lexer, 0)
	p := parser.NewRedstarParser(stream)
	p.AddErrorListener(antlr.NewDiagnosticErrorListener(true))
	p.BuildParseTrees = true

	for tree := p.Statement(); tree != nil; tree = p.Statement() {
		antlr.ParseTreeWalkerDefault.Walk(new(Foobar), tree)
	}

}
