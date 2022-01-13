using System;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar
{
    public static class Out
    {
        public static void Error(int id, IToken location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Red;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Warning(int id, IToken location, string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkYellow;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Info(int id, IToken location, string message)
        {
            Console.ForegroundColor = ConsoleColor.Blue;
            Console.WriteLine($"RS{id:0000}\t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
        public static void Debug(IToken location, string message)
        {
            Console.ForegroundColor = ConsoleColor.DarkMagenta;
            Console.WriteLine($"      \t{location.Location(),-40}{message}");
            Console.ForegroundColor = ConsoleColor.White;
        }
    }
}