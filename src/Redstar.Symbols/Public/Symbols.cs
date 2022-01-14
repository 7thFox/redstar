using System.Linq;
using Redstar.Parse;
using Redstar.Symbols.Listener;
using Redstar.Symbols.Mutable;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public static class Symbols
    {
        public static ISymbolTable ParseFiles(params string[] paths)
        {
            var path = paths.Single();

            var stream = CharStreams.fromPath(path);
            var lexer = new RedstarLexer(stream);
            var tokens = new CommonTokenStream(lexer);
            var parser = new RedstarParser(tokens);
            parser.BuildParseTree = true;

            var tree = parser.start();

            var symbolTable = new SymbolTable();
            // SetupImplicit(symbolTable);

            var buildProcess = new MultiListener();
            buildProcess.Listeners.Add(new ScopeBuilderListener(symbolTable));
            buildProcess.Listeners.Add(new SymbolListener(symbolTable));
            buildProcess.EnterEveryRule(tree);
            buildProcess.Listeners.Clear();
            buildProcess.Listeners.Add(new ScopeWalkerListener(symbolTable));
            buildProcess.Listeners.Add(new TypeCheckerListener(symbolTable));
            buildProcess.EnterEveryRule(tree);

            return symbolTable;
        }
    }
}