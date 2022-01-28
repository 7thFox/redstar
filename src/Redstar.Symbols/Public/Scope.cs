using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Redstar.Parse;

namespace Redstar.Symbols
{
    public partial class Scope : ISymbolInternal
    {
        public long ID { get; }
    }
}