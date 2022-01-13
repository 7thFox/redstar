using Redstar.Parser;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Internal
{
    public class TypeSymbol : ISymbol
    {
        public SymbolType Type => SymbolType.Type;
        public long ID { get; }
        public string Name { get; }
        [AllowNull]
        public IToken Declaration { get; }

        public TypeSymbol(long id, string name, [AllowNull] IToken decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }
    }
}