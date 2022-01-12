using System;
using System.Linq;
using Redstar.Parser;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;

namespace Redstar
{
    public static class Extensions
    {
        public static string Location(this IToken token)
        {
            return $"{token.InputStream.SourceName}:{token.Line}:{token.Column}";
        }
    }

    public class ScopeVisitor : RedstarBaseVisitor<Scope>
    {
        public Stack<Scope> _scope = new Stack<Scope>();
        public SymbolTable _symbols = new SymbolTable();

        private void VisitChildren(ParserRuleContext context)
        {
            foreach (var child in context.children)
            {
                child.Accept(this);
            }
        }

        public override Scope VisitStart([NotNull] RedstarParser.StartContext context)
        {
            _scope.Push(new Scope(null));
            VisitChildren(context);
            return ValidateScope(_scope.Pop());
        }

        public override Scope VisitFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            _scope.Peek().NewIdent(_symbols, context.ident());
            _scope.Push(new Scope(_scope.Peek()));
            context.funcParameters()?.Accept(this);
            context.funcReturn()?.Accept(this);
            context.body().Accept(this);
            return _scope.Pop();
        }

        public override Scope VisitBody([NotNull] RedstarParser.BodyContext context)
        {
            _scope.Push(new Scope(_scope.Peek()));
            VisitChildren(context);
            return _scope.Pop();
        }

        public override Scope VisitFuncParameter([NotNull] RedstarParser.FuncParameterContext context)
        {
            _scope.Peek().NewIdent(_symbols, context.ident());
            context.typename().Accept(this);
            return _scope.Peek();
        }

        public override Scope VisitVarDefine([NotNull] RedstarParser.VarDefineContext context)
        {
            _scope.Peek().NewIdent(_symbols, context.ident());
            context.attrList()?.Accept(this);
            context.exp().Accept(this);
            return _scope.Peek();
        }

        public override Scope VisitIdent([NotNull] RedstarParser.IdentContext context)
        {
            var symbol = _scope.Peek().Find(context.GetText());
            if (symbol != null)
            {
                Console.WriteLine($"{context.Start.Location()} Reference SymbolID {symbol.ID}");
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.Red;
                Console.WriteLine($"{context.Start.Location()} Cannot determine symbol reference");
                Console.ForegroundColor = ConsoleColor.White;
            }

            return _scope.Peek();
        }

        private static Scope ValidateScope(Scope scopeRoot)
        {
            IEnumerable<Scope> bottomLevelScopes(Scope scope){
                if (scope.Children.Count == 0){
                    return new[] { scope };
                }
                else
                {
                    return scope.Children.SelectMany(bottomLevelScopes);
                }
            }

            void verifyUp(Scope scope, HashSet<string> allDecl)
            {
                foreach (var decl in scope.DeclaredIdents)
                {
                    if (!allDecl.Add(decl.Key))
                    {
                        Console.ForegroundColor = ConsoleColor.Red;
                        Console.WriteLine($"{decl.Value.Declaration.Location()} Ident defined in outer scope");
                        Console.ForegroundColor = ConsoleColor.White;
                    }
                }
                if (scope.Parent != null) {
                    verifyUp(scope.Parent, allDecl);
                }
            }

            foreach (var scope in bottomLevelScopes(scopeRoot))
            {
                verifyUp(scope, new HashSet<string>());
            }

            return scopeRoot;
        }
    }
}