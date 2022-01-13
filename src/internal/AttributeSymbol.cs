using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Internal
{
    public class AttributeSymbol : ISymbol
    {
        public SymbolType Type => SymbolType.Attribute;
        public long ID { get; }
        public string Name { get; }
        public IToken Declaration { get; }

        public AttributeSymbol(long id, string name, IToken decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }
    }
}