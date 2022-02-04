using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Diagnostics.CodeAnalysis;

namespace Redstar.Symbols.Internal
{
    internal class FuncSymbol : ITypedSymbolInternal
    {
        public SymbolKind Kind => SymbolKind.Function;
        public long ID { get; }
        public string Name { get; }
        public Location Declaration { get; }

        public TypeSymbol? Type { get; internal set; }
        // public TypeSymbol FuncType { get; }
        // public TypeSymbol ReturnType { get; }
        // public LocalVariableSymbol[] Parameters { get; }

        internal FuncSymbol(long id, string name, Location decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}