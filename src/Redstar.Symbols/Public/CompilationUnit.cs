using System.Collections.Immutable;

namespace Redstar.Symbols
{
    public partial class CompilationUnit : ISymbolInternal
    {
        public string SourceName { get; }
        public ImmutableArray<Scope> Scopes { get; private set; }

        public void CopyInternalData()
        {
            Scopes = _scopes.ToImmutable();
        }
    }
}