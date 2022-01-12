using System;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Redstar;
using Redstar.Parser;

var path = args.Length > 0 ? args[0] : "/home/josh/src/redstar/examples/testanno/main.red";

var stream = CharStreams.fromPath(path);
var lexer = new RedstarLexer(stream);
var tokens = new CommonTokenStream(lexer);
var parser = new RedstarParser(tokens);
parser.BuildParseTree = true;

var printer = new PrintListener();
while (stream.LA(1) != IntStreamConstants.EOF)
{
    RedstarParser.StatementContext tree = parser.statement();
    if (parser.NumberOfSyntaxErrors > 0){
        Environment.Exit(1);
    }
    ParseTreeWalker.Default.Walk(printer, tree);
}
