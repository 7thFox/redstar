using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using Redstar.Symbols.Internal;

namespace Redstar.Symbols.Internal.Listener
{
    internal class TypeCheckerListener : RedstarBaseListener
    {
        // private List<(string, Scope)> _deferredFinds = new List<(string, Scope)>();
        private SymbolTable _symbols;
        public TypeCheckerListener(SymbolTable symbolTable) => _symbols = symbolTable;

        private Stack<ISymbol[]> _returnTypes = new Stack<ISymbol[]>();
        private Stack<ISymbol> _statementTypes = new Stack<ISymbol>();
        private Dictionary<long, long> _symbolToType = new Dictionary<long, long>();

        // public override void EnterStatement([NotNull] RedstarParser.StatementContext context)
        // {

        // }

        public override void EnterLiteralInt([NotNull] RedstarParser.LiteralIntContext context)
        {
            Out.Debug(DebugCategory.Type, context.Start, "int");
            _statementTypes.Push(_symbols.ImplicitScope.FindInThisScope("int").MustFind());
        }

        public override void EnterLiteralString([NotNull] RedstarParser.LiteralStringContext context)
        {
            Out.Debug(DebugCategory.Type, context.Start, "string");
            _statementTypes.Push(_symbols.ImplicitScope.FindInThisScope("string").MustFind());
        }

        public override void ExitBinaryExp([NotNull] RedstarParser.BinaryExpContext context)
        {
            var right = _statementTypes.Pop();
            var left = _statementTypes.Peek();// Pop then re-push
            if (!left.SymbolEqual(right))
            {
                Out.Error(3, context.Start, $"Cannot perform operation with {left.Name} and {right.Name}");
            }
            Out.Debug(DebugCategory.Type, context.Start, left.Name);
        }

        public override void ExitVarDefine([NotNull] RedstarParser.VarDefineContext context)
        {
            var symbol = _symbols.Find(context.ident().GetText()).MustFind();
            _symbolToType[symbol.ID] = _statementTypes.Peek().ID;
            Out.Debug(DebugCategory.Type, context.Start, _statementTypes.Peek().Name);
        }

        public override void EnterFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            var funcSymbol = _symbols.Find(context.ident().GetText()).MustFind();

            var ret = context.funcReturn();
            if (ret != null)
            {
                _returnTypes.Push(ret.typename()
                    .Select(t =>
                    {
                        var r = _symbols.Find(t.GetText());
                        if (r == null)
                        {
                            Out.Error(5, t.Start, $"Cannot resolve type name");
                            return _symbols.FastFind("void").MustFind();
                        }
                        return r;
                    })
                    .ToArray());
            }
            else
            {
                _returnTypes.Push(new[] { _symbols.FastFind("void").MustFind() });
            }

            Out.Debug(DebugCategory.Type, funcSymbol.Declaration,
                $"Return Type(s): [ {string.Join(", ", _returnTypes.Peek().Select(x => x.Name))} ]");
            if (_returnTypes.Peek().Length > 1)
            {
                Out.Error(9999, funcSymbol.Declaration,
                    "Yeah, I sorta don't actually support multiple return right now");
            }
            else
            {
                _symbolToType[funcSymbol.ID] = _returnTypes.Peek().Single().ID;
            }
        }

        public override void ExitReturnStmt([NotNull] RedstarParser.ReturnStmtContext context)
        {
            if (!_returnTypes.Peek().Single().SymbolEqual(_statementTypes.Peek()))
            {
                Out.Error(6, context.Start, "Returned value does not match function return type");
            }
        }

        public override void ExitFuncDecl([NotNull] RedstarParser.FuncDeclContext context)
        {
            _returnTypes.Pop();
        }

        public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        {
            // Includes func calls
            var parent = context.Parent?.GetType();
            if (parent != typeof(RedstarParser.VarDefineContext) &&
                parent != typeof(RedstarParser.FuncDeclContext) &&
                parent != typeof(RedstarParser.FuncParameterContext))
            {
                var symbol = _symbols.Find(context.GetText());
                if (symbol != null)
                {
                    Out.Debug(DebugCategory.Type, context.Start, $"Reference SymbolID {symbol.ID}");

                    _statementTypes.Push(_symbols.GetSymbol(_symbolToType[symbol.ID]));
                }
                else
                {
                    Out.Error(4, context.Start, "Cannot determine symbol reference");
                    _statementTypes.Push(_symbols.FastFind("void").MustFind());
                }
            }
        }

        public override void EnterFuncParameter([NotNull] RedstarParser.FuncParameterContext context)
        {
            var paramSymbol = _symbols.Find(context.ident().GetText()).MustFind();
            var typeName = context.typename();
            var typeSymbol = _symbols.Find(typeName.GetText());
            if (typeSymbol == null)
            {
                Out.Error(5, typeName.Start, $"Cannot resolve type name");
                typeSymbol = _symbols.FastFind("void").MustFind();
            }
            _symbolToType[paramSymbol.ID] = typeSymbol.ID;

            Out.Debug(DebugCategory.Type, paramSymbol.Declaration, typeSymbol.Name);
        }

        // private void ResolveReferences()
        // {
        //     foreach ((var ident, var scope) in _toResolve)
        //     {
        //         var symbol = scope.Find(ident.GetText());
        //         if (symbol != null)
        //         {
        //         }
        //         else
        //         {
        //             Console.ForegroundColor = ConsoleColor.Red;
        //             Console.WriteLine($"{ident.Start.Location()} Cannot determine symbol reference");
        //             Console.ForegroundColor = ConsoleColor.White;
        //         }
        //     }
        // }
    }
}