using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        public ISymbol GetSymbol(long id) => _symbols[id];

        [return: MaybeNull]
        public ISymbol Find(string symbolName)
        {
            return FastFind(symbolName) ?? CurrentScope.Find(symbolName);
        }

        [return: MaybeNull]
        public ISymbol FastFind(string symbolName)
        {
            return ImplicitScope.FindInThisScope(symbolName)
                ?? CurrentScope.FindInThisScope(symbolName);
        }
    }
}