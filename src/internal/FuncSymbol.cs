using Redstar.Parser;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Internal
{
    public class FuncSymbol : ISymbol
    {
        public SymbolType Type => SymbolType.Function;
        public long ID { get; }
        public string Name { get; }
        public IToken Declaration { get; }

        public FuncSymbol(long id, string name, IToken decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }
    }
}