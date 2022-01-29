using Redstar.Parse;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal class LocalVariableSymbol : ISymbolInternal
    {
        public SymbolType Type => SymbolType.LocalVariable;
        public long ID { get; }
        public string Name { get; }
        public IToken Declaration { get; }

        public LocalVariableSymbol(long id, string name, IToken decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}