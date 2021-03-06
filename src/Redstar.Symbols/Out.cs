using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using System.Runtime.Serialization;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Reflection;

namespace Redstar.Symbols
{
    public enum DebugCategory
    {
        TESTING,

        Scope,
        Symbol,
        Type,
        Reference,
    }

    public static class Out
    {
        private static HashSet<DebugCategory>? _debugCategories;
        public static IEnumerable<DebugCategory> DebugCategories
        {
            set => _debugCategories = new HashSet<DebugCategory>(value);
        }

        [DoesNotReturn]
        public static void Panic(Location location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"FATAL ERROR: {message}");
            Console.WriteLine($"\tat {location,-40}");
            Console.ForegroundColor = ConsoleColor.White;

            throw new Exception("panic");
        }

        public static void Error(ErrorCodes error, IToken token, string message) => Error(error, token.Location(), message);
        public static void Error(ErrorCodes error, Location location, string message)
        {
            var stdOut = Console.Out;
            Console.SetOut(Console.Error);
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"RS{(int)error:0000}\t{location,-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetOut(stdOut);
        }

        public static void Warning(ErrorCodes error, IToken token, string message) => Warning(error, token.Location(), message);
        public static void Warning(ErrorCodes id, Location location, string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine($"RS{id:0000}\t{location,-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }

        public static void Info(ErrorCodes error, IToken token, string message) => Info(error, token.Location(), message);
        public static void Info(ErrorCodes id, Location location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine($"RS{id:0000}\t{location,-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }

        public static void Debug(DebugCategory category, IToken token, string message) => Debug(category, token.Location(), message);
        public static void Debug(DebugCategory category, Location location, string message)
        {
            if (category == DebugCategory.TESTING ||
                (_debugCategories?.Contains(category) ?? false))
            {
                Console.ForegroundColor = ConsoleColor.DarkMagenta;
                Console.WriteLine($"      \t{location,-40}{message}");
                Console.ForegroundColor = ConsoleColor.White;
            }
        }
    }
}