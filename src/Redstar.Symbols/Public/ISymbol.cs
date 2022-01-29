using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Microsoft.VisualBasic;

namespace Redstar.Symbols
{
    public interface ISymbol
    {
        long ID { get; }
        SymbolType Type { get; }
        string Name { get; }
        Location Declaration { get; }
    }
}