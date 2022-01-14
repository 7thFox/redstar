using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        internal readonly Scope ImplicitScope;
        internal readonly Scope BuildScope;
        internal readonly Scope ToplevelScope;
        internal Scope CurrentScope { get; private set; }
        private Dictionary<long, ISymbol> _symbols = new ();
        private Dictionary<long, Scope> _scopeByID = new ();
        private long _lastSymbolID;
        private long _lastScopeID;

        internal SymbolTable()
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

        internal void RegisterSymbol(ISymbol symbol)
        {
            CurrentScope.AddSymbol(symbol);
        }

        internal void SetImplicitScope(Scope scope)
        {

        }
#region Creates
        internal ISymbol CreateLocalVariableSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new LocalVariableSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        internal ISymbol CreateAttributeSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new AttributeSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        internal ISymbol CreateFuncSymbol(RedstarParser.IdentContext ident)
        {
            return Add(new FuncSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
        }

        internal long CreateScope()
        {
            var scope = new Scope(++_lastScopeID, CurrentScope);
            _scopeByID[scope.ID] = scope;
            return scope.ID;
        }
#endregion

#region Utilities
        internal long SetScope(long id)
        {
            CurrentScope = _scopeByID[id];
            return CurrentScope.ID;
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