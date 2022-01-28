using System.Collections.Immutable;
using Antlr4.Runtime;

namespace Redstar.Symbols
{
    public partial class CompilationUnit : ISymbolInternal
    {
        public long ID { get; }
        public SymbolType Type => SymbolType.CompilationUnit;
        string ISymbol.Name => SourceName;
        IToken? ISymbol.Declaration => null;

        public string SourceName { get; }
        public ImmutableArray<Scope> Scopes { get; private set; }
    }
}