using Redstar.Parser;
using System.Collections.Generic;
using System.Linq;
using System.Diagnostics.CodeAnalysis;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar
{
    public class MultiListener : RedstarBaseListener
    {
        public List<IRedstarListener> Listeners = new List<IRedstarListener>();

        public override void EnterEveryRule([NotNull] ParserRuleContext context)
        {
            foreach (var listener in Listeners)
            {
                listener.EnterEveryRule(context);
                context.EnterRule(listener);
            }

            foreach (var child in context.children.OfType<ParserRuleContext>())
            {
                EnterEveryRule(child);
            }

            foreach (var listener in Listeners)
            {
                context.ExitRule(listener);
                listener.ExitEveryRule(context);
            }
        }
    }
}