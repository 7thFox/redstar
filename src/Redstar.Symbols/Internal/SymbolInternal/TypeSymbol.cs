using Redstar.Parse;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal class TypeSymbol : ISymbolInternal
    {
        public SymbolType Type => SymbolType.Type;
        public long ID { get; }
        public string Name { get; }
        public Location Declaration { get; }

        internal TypeSymbol(long id, string name, Location decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}