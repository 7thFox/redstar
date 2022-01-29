using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Antlr4.Runtime;
using Redstar.Parse;

namespace Redstar.Symbols
{
    public partial class Scope
    {
        public long ID { get; }
        public IToken? Declaration { get; }

        public IReadOnlyDictionary<string, ISymbol> DeclaredSymbols => _declaredSymbols;

        public ISymbol? Find(string identName)
        {
            if (_declaredSymbols.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            if (Parent != null)
            {
                return Parent.Find(identName);
            }

            return null;
        }
    }
}