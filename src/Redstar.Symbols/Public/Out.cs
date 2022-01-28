using System;
using Redstar.Symbols.Internal;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar.Symbols
{
    public enum DebugCategory
    {
        Scope,
        Symbol,
        Type,
    }

    public static class Out
    {
        private static HashSet<DebugCategory>? _debugCategories;
        public static IEnumerable<DebugCategory> DebugCategories
        {
            set => _debugCategories = new HashSet<DebugCategory>(value);
        }

        public static void Error(int id, IToken? location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Warning(int id, IToken? location, string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Info(int id, IToken? location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Debug(DebugCategory category, IToken? location, string message)
        {
            if (_debugCategories?.Contains(category) ?? false)
            {
                Console.ForegroundColor = ConsoleColor.DarkMagenta;
                Console.WriteLine($"      \t{location.Location(),-40}{message}");
                Console.ForegroundColor = ConsoleColor.White;
            }
        }
    }
}