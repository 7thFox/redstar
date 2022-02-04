using Redstar.Parse;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal class LocalVariableSymbol : ITypedSymbolInternal
    {
        public SymbolKind Kind => SymbolKind.LocalVariable;
        public long ID { get; }
        public string Name { get; }
        public Location Declaration { get; }

        public TypeSymbol? Type { get; internal set; }

        public LocalVariableSymbol(long id, string name, Location decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}