using System;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols.Internal
{
    internal static class Extensions
    {
        public static string Location(this IToken? token)
        {
            if (token == null)
            {
                return "implicit";
            }

            return $"{token.InputStream.SourceName}:{token.Line}:{token.Column+1}";
        }

        [return: NotNull]
        public static ISymbol MustFind(this ISymbol? symbol)
            => symbol ?? throw new Exception("Could not find implicit symbol");

        // public static bool SymbolEqual(this ISymbol left, ISymbol right)
        // {
        //     return left.ID == right.ID;
        // }
    }
}