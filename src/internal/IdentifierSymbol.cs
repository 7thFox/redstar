using Redstar.Parser;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Internal
{
    public class IdentifierSymbol : ISymbol
    {
        public SymbolType Type => SymbolType.Identifer;
        public long ID { get; }
        public string Name { get; }
        public IToken Declaration { get; }

        public IdentifierSymbol(long id, RedstarParser.IdentContext ident)
        {
            ID = id;
            Name = ident.GetText();
            Declaration = ident.Start;
        }
    }
}