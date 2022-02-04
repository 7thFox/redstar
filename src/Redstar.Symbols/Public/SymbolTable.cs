using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Redstar.Symbols.Internal;
using static Redstar.Symbols.Internal.Assertions;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        public Scope ImplicitScope { get; private set; }
        public ImmutableArray<CompilationUnit> Units { get; private set; }

        // public ISymbol GetSymbol(long id) => _symbols[id];

        public ISymbol? FastFind(string symbolName)
        {
            return ImplicitScope.FindInThisScope(symbolName)
                ?? CurrentScope.FindInThisScope(symbolName);
        }

        public ISymbol? FastFind(IToken token) => FastFind(token.Location());
        public ISymbol? FastFind(Location location)
        {
            Assert(!location.IsImplicit);
            if (_symbolByToken.TryGetValue(location, out var symbol))
            {
                return symbol;
            }
            return null;
        }

        public ISymbol? FastFind([DisallowNull] RedstarParser.IdentContext ident)
        {
            return FastFind(ident.Start) ?? Find(ident.GetText());
        }

        public ISymbol? Find(string symbolName)
        {
            var symbol = FastFind(symbolName);
            if (symbol != null)
            {
                return symbol;
            }

            var scope = CurrentScope.Parent!;// CurrentScope already checked
            while (scope.ID != ImplicitScope.ID)
            {
                symbol = scope.FindInThisScope(symbolName);
                if (symbol != null)
                {
                    return symbol;
                }
                scope = scope.Parent!;
            }

            return null;
        }

        internal void CopyInternalData()
        {
            Units = _units.ToImmutable();
        }
    }
}