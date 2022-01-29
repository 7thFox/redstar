using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Redstar.Parse;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public partial class Scope
    {
        internal Scope(long id, Scope? parent, IToken? declaration)
        {
            Parent = parent;
            Declaration = declaration;
            parent?._children.Add(this);
            ID = id;
        }

        private readonly List<Scope> _children = new();
        internal IReadOnlyList<Scope> Children => _children;

        internal Scope? Parent { get; }

        internal bool AddSymbol(ISymbol symbol)
        {
            // fast check. SymbolCreationListener will be more thorough
            if (!_declaredSymbols.TryAdd(symbol.Name, symbol))
            {
                Out.Error(ErrorCodes.Redefined, symbol.Declaration, $"{symbol.Name} redefined");
                return false;
            }
            return true;
        }

        private Dictionary<string, ISymbol> _declaredSymbols { get; } = new Dictionary<string, ISymbol>();
        internal IReadOnlyDictionary<string, ISymbol> DeclaredSymbols => _declaredSymbols;

        internal ISymbol? FindInThisScope(string identName)
        {
            if (_declaredSymbols.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            return null;
        }

        // internal override string ToString()
        // {
        //     return ID.ToString() + "\n" + string.Join("\n", Children.Select(x => x.ToString(1)));
        // }

        // internal string ToString(int level)
        // {
        //     return new string(' ', level * 2) +
        //         ID.ToString() +
        //         "\n" +
        //         string.Join("\n", Children.Select(x => x.ToString(level + 1)));
        // }
    }
}