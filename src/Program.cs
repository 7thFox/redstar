using System;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Redstar;
using Redstar.Internal;
using Redstar.Internal.Listener;
using Redstar.Parser;

var path = args.Length > 0
    ? args[0]
    : "/home/josh/src/redstar/examples/testscope/main.red";

var stream = CharStreams.fromPath(path);
var lexer = new RedstarLexer(stream);
var tokens = new CommonTokenStream(lexer);
var parser = new RedstarParser(tokens);
parser.BuildParseTree = true;

Out.DebugCategories = new[]
{
    DebugCategory.ScopeAndSymbol,
};

var tree = parser.start();
var symbolTable = new SymbolTable();
var buildProcess = new MultiListener();
buildProcess.Listeners.Add(new ScopeBuilderListener(symbolTable));
buildProcess.Listeners.Add(new SymbolListener(symbolTable));
buildProcess.EnterEveryRule(tree);
buildProcess.Listeners.Clear();
// buildProcess.Listeners.Add(new ScopeWalkerListener(symbolTable));
// buildProcess.Listeners.Add(new TypeCheckerListener(symbolTable));
// buildProcess.EnterEveryRule(tree);

// Console.WriteLine(symbolTable.ToplevelScope.ToString());