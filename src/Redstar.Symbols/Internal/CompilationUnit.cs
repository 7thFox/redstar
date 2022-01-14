using System.IO;
using Redstar.Parse;
using Redstar.Symbols.Internal.Listener;
using Redstar.Symbols.Internal;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class CompilationUnit
    {
        internal Scope TopLevel { get; set; }

        internal CompilationUnit(string sourceName)
        {
            SourceName = sourceName;
        }

        internal void Parse(SymbolTable symbolTable)
        {
            var stream = CharStreams.fromPath(SourceName);
            var lexer = new RedstarLexer(stream);
            var tokens = new CommonTokenStream(lexer);
            var parser = new RedstarParser(tokens);
            parser.BuildParseTree = true;

            var tree = parser.start();

            TopLevel = SymbolFactory.CreateScope(symbolTable.BuildScope);
            symbolTable.SetScope(TopLevel);

            var buildProcess = new MultiListener();
            buildProcess.Listeners.Add(new ScopeBuilderListener(symbolTable));
            buildProcess.Listeners.Add(new SymbolListener(symbolTable));
            buildProcess.EnterEveryRule(tree);
            buildProcess.Listeners.Clear();
            buildProcess.Listeners.Add(new ScopeWalkerListener(symbolTable));
            buildProcess.Listeners.Add(new TypeCheckerListener(symbolTable));
            buildProcess.EnterEveryRule(tree);
        }
    }
}