using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Antlr4.Runtime;
using Redstar.Parse;

namespace Redstar.Symbols
{
    public partial class Scope : ISymbolInternal
    {
        public SymbolType Type => SymbolType.Scope;
        public long ID { get; }
        public IToken? Declaration { get; }
        string ISymbol.Name => string.Empty;
    }
}