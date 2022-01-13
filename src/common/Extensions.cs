using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar
{
    public static class Extensions
    {
        public static string Location(this IToken token)
        {
            if (token == null)
            {
                return "implicit";
            }

            return $"{token.InputStream.SourceName}:{token.Line}:{token.Column}";
        }
    }
}