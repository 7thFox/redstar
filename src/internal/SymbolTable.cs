using Redstar.Parser;
using System.Collections.Generic;

namespace Redstar.Internal
{
    public class SymbolTable : ISymbolTable
    {
        public readonly Scope ImplicitScope;
        public readonly Scope ToplevelScope;
        public Scope CurrentScope => _scopes.Peek();
        private List<ISymbol> _symbols = new List<ISymbol>();
        private Stack<Scope> _scopes = new Stack<Scope>();
        private long _lastSymbolID = 0;
        private long _lastScopeID = 0;

        public SymbolTable()
        {
            ImplicitScope = new Scope(++_lastScopeID, null);
            _scopes.Push(ImplicitScope);

            Add(new TypeSymbol(-100, "bool", null));
            Add(new TypeSymbol(-101, "byte", null));
            Add(new TypeSymbol(-102, "short", null));
            Add(new TypeSymbol(-103, "int", null));
            Add(new TypeSymbol(-104, "long", null));
            Add(new TypeSymbol(-105, "string", null));
            Add(new TypeSymbol(-106, "char", null));
            Add(new TypeSymbol(-107, "decimal", null));
            Add(new TypeSymbol(-108, "float", null));
            Add(new TypeSymbol(-109, "double", null));

            ToplevelScope = new Scope(++_lastScopeID, ImplicitScope);
            _scopes.Push(ToplevelScope);
        }

        public ISymbol CreateIdentiferSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new IdentifierSymbol(++_lastSymbolID, ident));
        }

        public void OpenScope()
        {
            _scopes.Push(new Scope(++_lastScopeID, _scopes.Peek()));
        }

        public void CloseScope()
        {
            _scopes.Pop();
        }

        private ISymbol Add(ISymbol symbol)
        {
            _symbols.Add(symbol);
            CurrentScope.AddSymbol(symbol);
            return symbol;
        }
    }
}