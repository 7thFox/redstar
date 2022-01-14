using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Mutable
{
    internal class SymbolTable : ISymbolTable
    {
        public readonly Scope ImplicitScope;
        public readonly Scope ToplevelScope;
        public Scope CurrentScope { get; private set; }
        private Dictionary<long, ISymbol> _symbols = new ();
        private Dictionary<long, Scope> _scopeByID = new ();
        private long _lastSymbolID;
        private long _lastScopeID;

        public SymbolTable()
        {
            CurrentScope = ImplicitScope = new Scope(-1, null);
            _scopeByID[ImplicitScope.ID] = ImplicitScope;

            Add(new TypeSymbol(-100, "void", null));
            Add(new TypeSymbol(-101, "byte", null));
            Add(new TypeSymbol(-102, "short", null));
            Add(new TypeSymbol(-103, "int", null));
            Add(new TypeSymbol(-104, "long", null));
            Add(new TypeSymbol(-105, "bool", null));
            Add(new TypeSymbol(-106, "char", null));
            Add(new TypeSymbol(-107, "string", null));
            Add(new TypeSymbol(-108, "decimal", null));
            Add(new TypeSymbol(-109, "float", null));
            Add(new TypeSymbol(-110, "double", null));

            _lastSymbolID = 0;
            _lastScopeID = 0;

            CurrentScope = ToplevelScope = new Scope(++_lastScopeID, ImplicitScope);
            _scopeByID[ToplevelScope.ID] = ToplevelScope;
        }

        public void RegisterSymbol(ISymbol symbol)
        {
            CurrentScope.AddSymbol(symbol);
        }

        public void SetImplicitScope(Scope scope)
        {

        }
#region Creates
        public ISymbol CreateLocalVariableSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new LocalVariableSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        public ISymbol CreateAttributeSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new AttributeSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        public ISymbol CreateFuncSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new FuncSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        public long CreateScope()
        {
            var scope = new Scope(++_lastScopeID, CurrentScope);
            _scopeByID[scope.ID] = scope;
            return scope.ID;
        }
#endregion

#region Utilities
        public long SetScope(long id)
        {
            CurrentScope = _scopeByID[id];
            return CurrentScope.ID;
        }

        public ISymbol GetSymbol(long id) => _symbols[id];

        [return: MaybeNull]
        public ISymbol Find(string symbolName)
        {
            return FastFind(symbolName) ?? CurrentScope.Find(symbolName);
        }

        [return: MaybeNull]
        public ISymbol FastFind(string symbolName)
        {
            return ImplicitScope.FindInThisScope(symbolName)
                ?? CurrentScope.FindInThisScope(symbolName);
        }
#endregion
#region Private
        private ISymbol Add(ISymbol symbol)
        {
            _symbols.Add(symbol.ID, symbol);
            CurrentScope.AddSymbol(symbol);
            return symbol;
        }
#endregion
    }
}