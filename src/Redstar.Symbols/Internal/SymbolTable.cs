using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Redstar.Parse;
using Redstar.Symbols.Internal;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Collections.Immutable;

namespace Redstar.Symbols
{
    public partial class SymbolTable
    {
        internal Scope CurrentScope { get; private set; }
        internal ImmutableArray<CompilationUnit>.Builder _units;

        internal SymbolTable(Scope implicitScope)
        {
            CurrentScope = ImplicitScope = implicitScope;
            _units = ImmutableArray.CreateBuilder<CompilationUnit>();
        }

        internal void CreateImplicitSymbols()
        {
            Register(SymbolFactory.Type("void", null));
            Register(SymbolFactory.Type("byte", null));
            Register(SymbolFactory.Type("short", null));
            Register(SymbolFactory.Type("int", null));
            Register(SymbolFactory.Type("long", null));
            Register(SymbolFactory.Type("bool", null));
            Register(SymbolFactory.Type("char", null));
            Register(SymbolFactory.Type("string", null));
            Register(SymbolFactory.Type("decimal", null));
            Register(SymbolFactory.Type("float", null));
            Register(SymbolFactory.Type("double", null));
        }

        internal T Register<T>(T symbol) where T : ISymbolInternal
        {
            // CurrentScope.AddSymbol(symbol);
            return symbol;
        }

        internal void SetScope(Scope scope)
        {
            CurrentScope = scope;
        }

//         internal void SetImplicitScope(Scope scope)
//         {

//         }
// #region Creates
//         internal ISymbol CreateLocalVariableSymbol(RedstarParser.IdentContext ident)
//         {
//             return Add(new LocalVariableSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
//         }

//         internal ISymbol CreateAttributeSymbol(RedstarParser.IdentContext ident)
//         {
//             return Add(new AttributeSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
//         }

//         internal ISymbol CreateFuncSymbol(RedstarParser.IdentContext ident)
//         {
//             return Add(new FuncSymbol(++_lastSymbolID, ident.GetText(), ident.Start));
//         }

//         internal long CreateScope()
//         {
//             var scope = new Scope(++_lastScopeID, CurrentScope);
//             _scopeByID[scope.ID] = scope;
//             return scope.ID;
//         }
// #endregion

// #region Utilities
//         internal long SetScope(long id)
//         {
//             CurrentScope = _scopeByID[id];
//             return CurrentScope.ID;
//         }
// #endregion
// #region Private
//         private ISymbol Add(ISymbol symbol)
//         {
//             _symbols.Add(symbol.ID, symbol);
//             CurrentScope.AddSymbol(symbol);
//             return symbol;
//         }
// #endregion
    }
}