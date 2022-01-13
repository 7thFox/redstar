using System;
using System.Linq;
using Redstar.Parser;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar
{
    public interface ISymbol
    {
        long ID { get; }
        SymbolType Type { get; }
        string Name { get; }
        IToken Declaration { get; }
    }
}