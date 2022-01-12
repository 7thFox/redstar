using Redstar.Parser;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Collections.Generic;

namespace Redstar
{
    public class SymbolTable
    {
        private List<ISymbol> _symbols = new List<ISymbol>();
        private long _lastSymbolID = 0;

        public ISymbol CreateIdentiferSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new IdentifierSymbol(++_lastSymbolID, ident));
        }

        private ISymbol Add(ISymbol symbol)
        {
            _symbols.Add(symbol);
            return symbol;
        }

        private class IdentifierSymbol : ISymbol
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

    public interface ISymbol
    {
        long ID { get; }
        SymbolType Type { get; }
        string Name { get; }
        IToken Declaration { get; }
    }

    public enum SymbolType
    {
        Identifer,
    }
}