using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Linq;
using Redstar.Parse;

namespace Redstar.Symbols
{
    public partial class Scope
    {
        internal Scope(long id, [AllowNull] Scope parent)
        {
            Parent = parent;
            // parent?._children.Add(this);
            ID = id;
        }

        // private readonly List<Scope> _children = new List<Scope>();
        // internal IReadOnlyList<Scope> Children => _children;

        [AllowNull]
        internal Scope Parent { get; }

        // internal void AddSymbol(ISymbol symbol)
        // {
        //     if (!_declaredIdents.TryAdd(symbol.Name, symbol))
        //     {
        //         Out.Error(1, symbol.Declaration, $"{symbol.Name} redefined");
        //     }
        //     else
        //     {
        //         Out.Debug(DebugCategory.ScopeAndSymbol, symbol.Declaration, $"New ident: {symbol.Name,-15} (SymbolID {symbol.ID}, ScopeID {ID})");
        //     }
        // }

        // private Dictionary<string, ISymbol> _declaredIdents { get; } = new Dictionary<string, ISymbol>();
        // internal IReadOnlyDictionary<string, ISymbol> DeclaredIdents => _declaredIdents;

        // [return: MaybeNull]
        // internal ISymbol Find(string identName)
        // {
        //     if (_declaredIdents.TryGetValue(identName, out var symbol))
        //     {
        //         return symbol;
        //     }

        //     if (Parent != null)
        //     {
        //         return Parent.Find(identName);
        //     }

        //     return null;
        // }

        // [return: MaybeNull]
        // internal ISymbol FindInThisScope(string identName)
        // {
        //     if (_declaredIdents.TryGetValue(identName, out var symbol))
        //     {
        //         return symbol;
        //     }

        //     return null;
        // }

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

        internal void CopyInternalData()
        {

        }
    }
}