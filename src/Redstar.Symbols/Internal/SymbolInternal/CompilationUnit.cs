using System.IO;
using Redstar.Parse;
using Redstar.Symbols.Internal.Listener;
using Redstar.Symbols.Internal;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Collections.Immutable;

namespace Redstar.Symbols
{
    public partial class CompilationUnit
    {
        internal Scope UnitScope { get; }
        private ImmutableArray<Scope>.Builder _scopes { get; }

        internal CompilationUnit(long id, string sourceName, Scope scope)
        {
            ID = id;
            SourceName = sourceName;
            UnitScope = scope;
            _scopes = ImmutableArray.CreateBuilder<Scope>();
        }

        internal void Parse(SymbolTable symbolTable, ICharStream stream)
        {
            var lexer = new RedstarLexer(stream);
            var tokens = new CommonTokenStream(lexer);
            var parser = new RedstarParser(tokens);
            parser.BuildParseTree = true;

            var tree = parser.start();

            symbolTable.SetScope(UnitScope);

            var buildProcess = new MultiListener();

            buildProcess.Listeners.Add(new ScopeBuilderListener(symbolTable, this));
            buildProcess.Listeners.Add(new SymbolCreationListener(symbolTable));
            buildProcess.Listeners.Add(new SymbolReferenceResolverListener(symbolTable));
            buildProcess.EnterEveryRule(tree);

            // buildProcess.Listeners.Clear();

            // buildProcess.Listeners.Add(new ScopeWalkerListener(symbolTable));
            // buildProcess.Listeners.Add(new TypeCheckerListener(symbolTable));
            // buildProcess.EnterEveryRule(tree);
        }

        internal void AddScope(Scope scope)
        {
            _scopes.Add(scope);
        }

        void ISymbolInternal.CopyInternalData()
        {
            Scopes = _scopes.ToImmutable();
        }
    }
}