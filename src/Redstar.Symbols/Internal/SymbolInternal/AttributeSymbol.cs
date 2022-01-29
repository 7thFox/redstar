using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal class AttributeSymbol : ISymbolInternal
    {
        public SymbolType Type => SymbolType.Attribute;
        public long ID { get; }
        public string Name { get; }
        public Location Declaration { get; }

        public AttributeSymbol(long id, string name, Location decl)
        {
            ID = id;
            Name = name;
            Declaration = decl;
        }

        void ISymbolInternal.CopyInternalData() { }
    }
}