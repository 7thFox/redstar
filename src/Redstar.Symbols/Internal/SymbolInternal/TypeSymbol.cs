using Redstar.Parse;
using System;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class TypeSymbol
    {
        internal TypeSymbol(long id)
        {
            ID = id;
            Name = "typeref";
            Declaration = Location.Implicit;
            Type = this;
        }

        internal TypeSymbol(long id, string name, Location decl, TypeSymbol type)
        {
            ID = id;
            Name = name;
            Declaration = decl;
            Type = type;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}