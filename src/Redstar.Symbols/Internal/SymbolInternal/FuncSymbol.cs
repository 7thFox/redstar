using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal class FuncSymbol : ISymbolInternal
    {
        public SymbolType Type => SymbolType.Function;
        public long ID { get; }
        public string Name { get; }
        public IToken Declaration { get; }
        // public TypeSymbol FuncType { get; }
        // public TypeSymbol ReturnType { get; }
        // public LocalVariableSymbol[] Parameters { get; }

        public FuncSymbol(long id, string name, IToken decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}