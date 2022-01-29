using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        public Scope ImplicitScope { get; private set; }
        public ImmutableArray<CompilationUnit> Units { get; private set; }

        // public ISymbol GetSymbol(long id) => _symbols[id];

        // [return: AllowNull]
        // public ISymbol Find(string symbolName)
        // {
        //     return FastFind(symbolName) ?? CurrentScope.Find(symbolName);
        // }

        public ISymbol? FastFind(string symbolName)
        {
            return ImplicitScope.FindInThisScope(symbolName)
                ?? CurrentScope.FindInThisScope(symbolName);
        }

        public ISymbol? FastFind(IToken token)
        {
            if (_symbolByToken.TryGetValue(token, out var symbol))
            {
                return symbol;
            }
            return null;
        }

        public ISymbol? FastFind([DisallowNull] RedstarParser.IdentContext ident)
        {
            return FastFind(ident.Start) ?? FastFind(ident.GetText());
        }

        internal void CopyInternalData()
        {
            Units = _units.ToImmutable();
        }
    }
}