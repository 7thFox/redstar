using System;
using Redstar.Parser;

namespace Redstar
{
    public class PrintListener : RedstarBaseListener
    {
        public override void EnterEveryRule(Antlr4.Runtime.ParserRuleContext context)
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.WriteLine(context.GetText());
        }
    }
}