using Redstar.Parse;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class TypeSymbol : ITypedSymbolInternal
    {
        public SymbolKind Kind => SymbolKind.Type;
        public long ID { get; }
        public string Name { get; }
        public TypeSymbol? Type { get; }
        public Location Declaration { get; }
    }
}