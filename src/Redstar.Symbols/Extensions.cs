using System;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    internal static class Extensions
    {
        public static Location Location(this IToken token)
        {
            return new Location()
            {
                FileName = token.InputStream.SourceName,
                Line = token.Line,
                Column = token.Column + 1,
            };
        }

        public static ISymbol MustFind(this ISymbol? symbol)
            => symbol ?? throw new Exception("Could not find implicit symbol");


        public static T MustFind<T>(this ISymbol? symbol)
            where T : class, ISymbol
            => (symbol as T) ?? throw new Exception("Could not find implicit symbol");

        public static bool SymbolEqual(this ISymbol left, ISymbol right)
        {
            return left.ID == right.ID;
        }
    }
    internal static class Assertions
    {
        public static void Assert(bool success)
        {
            if (!success)
            {
                throw new Exception("Internal compiler assertion failed");
            }
        }
    }
}