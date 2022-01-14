using System.Linq;
using Redstar.Parse;

namespace Redstar.Symbols
{
    public static class Symbols
    {
        public static SymbolTable ParseFiles(params string[] paths)
        {
            var symbolTable = new SymbolTable();

            foreach (var path in paths)
            {
                new CompilationUnit(path);
            }


            return symbolTable;
        }
    }
}