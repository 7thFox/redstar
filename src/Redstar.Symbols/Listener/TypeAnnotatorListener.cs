using System;
using System.Linq;
using Redstar.Parse;
using System.Diagnostics.CodeAnalysis;
using System.Collections.Generic;
using Antlr4.Runtime;
using Antlr4.Runtime.Tree;
using static Redstar.Symbols.Assertions;

namespace Redstar.Symbols.Listener
{
    internal class TypeAnnotatorListener : RedstarBaseListener
    {
        private readonly SymbolTable _symbols;
        public TypeAnnotatorListener(SymbolTable symbolTable) => _symbols = symbolTable;

        public override void EnterFuncParameter([NotNull] RedstarParser.FuncParameterContext context)
        {
            var typename = context.typename();
            var type = _symbols.FastFind(typename.GetText());
            if (type != null)
            {
                var local = _symbols.FastFind(context.ident().Start.Location()).MustFind<LocalVariableSymbol>();
                local.Type = type.AssertKind<TypeSymbol>(SymbolKind.Type, ErrorCodes.ExpectedTypeName, typename.Start.Location());
            }
        }

        public override void ExitVarDefine([NotNull] RedstarParser.VarDefineContext context)
        {
            TypeSymbol? typeSymbol = null;

            Assert(_expTypes.Count <= 1);

            var typename = context.typename();
            if (typename != null)
            {
                var type = _symbols.FastFind(typename.GetText());
                if (type != null)
                {
                    if (_expTypes.Count > 0 && _expTypes.Peek().ID != type.ID)
                    {
                        Out.Error(ErrorCodes.TypeMismatch, context.Start, "Type mismatch");
                        typeSymbol = null;
                    }
                    else
                    {
                        typeSymbol = type.AssertKind<TypeSymbol>(SymbolKind.Type, ErrorCodes.ExpectedTypeName, typename.Start.Location());
                    }
                }
            }
            else if (_expTypes.Count > 0)
            {
                typeSymbol = _expTypes.Peek();
            }

            if (typeSymbol != null)
            {
                var local = _symbols.FastFind(context.ident().Start.Location()).MustFind<LocalVariableSymbol>();
                local.Type = typeSymbol;
            }
        }


        private readonly Stack<TypeSymbol> _expTypes = new();
        public override void EnterStatement([NotNull] RedstarParser.StatementContext context)
        {
            _expTypes.Clear();
        }

        public override void EnterLiteralDecimal([NotNull] RedstarParser.LiteralDecimalContext context)
        {
            _expTypes.Push(_symbols.FastFind("float").MustFind<TypeSymbol>());
        }

        public override void EnterLiteralInt([NotNull] RedstarParser.LiteralIntContext context)
        {
            _expTypes.Push(_symbols.FastFind("int").MustFind<TypeSymbol>());
        }

        public override void EnterLiteralString([NotNull] RedstarParser.LiteralStringContext context)
        {
            _expTypes.Push(_symbols.FastFind("string").MustFind<TypeSymbol>());
        }

        public override void ExitBinaryExp([NotNull] RedstarParser.BinaryExpContext context)
        {
            // all sub-expressions (including those in sub-binary expressions)
            // get called before this FYI

            // Stack will decrease by one for each binary exp. This might not fail
            // at the right time, but it will fail before the end of the total expression.
            if (_expTypes.Count < 2)
            {
                _expTypes.Clear();// Some sub-expression can't be determined; neither can this one
            }
            else
            {
                var lhs = _expTypes.Pop();
                var rhs = _expTypes.Peek();// Pop + Push
                if (lhs.ID != rhs.ID)
                {
                    Out.Error(ErrorCodes.TypeMismatch, context.Start, "Type mismatch on binary exp");
                }
            }
        }

        public override void EnterIdent([NotNull] RedstarParser.IdentContext context)
        {
            if (context.Parent?.RuleIndex == RedstarParser.RULE_term)
            {
                var type = (_symbols.FastFind(context.Start) as LocalVariableSymbol)?.Type;
                if (type != null)
                {
                    _expTypes.Push(type);
                }
            }
        }
    }
}