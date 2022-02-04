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
        public long ID { get; }
        public Location Declaration { get; }

        public IReadOnlyDictionary<string, ISymbol> DeclaredSymbols => _declaredSymbols;

        public ISymbol? Find(string identName)
        {
            if (_declaredSymbols.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            if (Parent != null)
            {
                return Parent.Find(identName);
            }

            return null;
        }
    }

    public partial class Scope
    {
        internal Scope(long id, Scope? parent, Location declaration)
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

        internal ISymbol? FindInThisScope(string identName)
        {
            if (_declaredSymbols.TryGetValue(identName, out var symbol))
            {
                return symbol;
            }

            return null;
        }
    }
}