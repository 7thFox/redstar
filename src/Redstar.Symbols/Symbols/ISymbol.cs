using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Microsoft.VisualBasic;

namespace Redstar.Symbols
{
    public interface ISymbol
    {
        long ID { get; }
        SymbolKind Kind { get; }
        string Name { get; }
        Location Declaration { get; }
    }

    public interface ITypedSymbol : ISymbol
    {
        [Backfilled]
        TypeSymbol? Type { get; }
    }

    internal interface ITypedSymbolInternal : ISymbolInternal, ITypedSymbol
    {
        // void SetType(TypeSymbol type);
    }

    [AttributeUsage(AttributeTargets.Property)]
    internal class BackfilledAttribute : Attribute
    {
        // Doesn't do anything, but informational
    }

    public static class ISymbolExtensions
    {
        public static T? AssertKind<T>(this ISymbol symbol, SymbolKind kind, ErrorCodes error, Location location)
            where T : class, ISymbol
        {
            if (symbol.Kind != kind)
            {
                Out.Error(error, location, $"Expected {kind}.");
                return null;
            }

            return (symbol as T) ?? throw new Exception("Internal compiler exception: Matched type, but couldn't cast");
        }
    }
}