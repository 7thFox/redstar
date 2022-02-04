using System.Collections.Immutable;
using Antlr4.Runtime;

namespace Redstar.Symbols
{
    public partial class CompilationUnit : ISymbolInternal
    {
        public long ID { get; }
        public SymbolKind Kind => SymbolKind.CompilationUnit;
        string ISymbol.Name => SourceName;
        Location ISymbol.Declaration => Location.Implicit;

        public string SourceName { get; }
        public Scope UnitScope { get; }
        public ImmutableArray<Scope> Scopes { get; private set; }
    }
}