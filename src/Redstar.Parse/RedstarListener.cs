//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     ANTLR Version: 4.9.3
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

// Generated from src/Redstar.Parse/Redstar.g4 by ANTLR 4.9.3

// Unreachable code detected
#pragma warning disable 0162
// The variable '...' is assigned but its value is never used
#pragma warning disable 0219
// Missing XML comment for publicly visible type or member '...'
#pragma warning disable 1591
// Ambiguous reference in cref attribute
#pragma warning disable 419

namespace Redstar.Parse {
using Antlr4.Runtime.Misc;
using IParseTreeListener = Antlr4.Runtime.Tree.IParseTreeListener;
using IToken = Antlr4.Runtime.IToken;

/// <summary>
/// This interface defines a complete listener for a parse tree produced by
/// <see cref="RedstarParser"/>.
/// </summary>
[System.CodeDom.Compiler.GeneratedCode("ANTLR", "4.9.3")]
[System.CLSCompliant(false)]
public interface IRedstarListener : IParseTreeListener {
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.start"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterStart([NotNull] RedstarParser.StartContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.start"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitStart([NotNull] RedstarParser.StartContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.statement"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterStatement([NotNull] RedstarParser.StatementContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.statement"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitStatement([NotNull] RedstarParser.StatementContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.attrDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterAttrDecl([NotNull] RedstarParser.AttrDeclContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.attrDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitAttrDecl([NotNull] RedstarParser.AttrDeclContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.annoDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterAnnoDecl([NotNull] RedstarParser.AnnoDeclContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.annoDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitAnnoDecl([NotNull] RedstarParser.AnnoDeclContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.bindDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBindDecl([NotNull] RedstarParser.BindDeclContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.bindDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBindDecl([NotNull] RedstarParser.BindDeclContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.bindAnnoConstraint"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBindAnnoConstraint([NotNull] RedstarParser.BindAnnoConstraintContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.bindAnnoConstraint"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBindAnnoConstraint([NotNull] RedstarParser.BindAnnoConstraintContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.bindAnnoLists"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBindAnnoLists([NotNull] RedstarParser.BindAnnoListsContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.bindAnnoLists"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBindAnnoLists([NotNull] RedstarParser.BindAnnoListsContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.bindAnnoAttrList"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBindAnnoAttrList([NotNull] RedstarParser.BindAnnoAttrListContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.bindAnnoAttrList"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBindAnnoAttrList([NotNull] RedstarParser.BindAnnoAttrListContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.funcDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFuncDecl([NotNull] RedstarParser.FuncDeclContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.funcDecl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFuncDecl([NotNull] RedstarParser.FuncDeclContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.funcParameters"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFuncParameters([NotNull] RedstarParser.FuncParametersContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.funcParameters"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFuncParameters([NotNull] RedstarParser.FuncParametersContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.funcParameter"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFuncParameter([NotNull] RedstarParser.FuncParameterContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.funcParameter"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFuncParameter([NotNull] RedstarParser.FuncParameterContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.funcReturn"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFuncReturn([NotNull] RedstarParser.FuncReturnContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.funcReturn"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFuncReturn([NotNull] RedstarParser.FuncReturnContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.body"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBody([NotNull] RedstarParser.BodyContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.body"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBody([NotNull] RedstarParser.BodyContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.ifStmt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterIfStmt([NotNull] RedstarParser.IfStmtContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.ifStmt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitIfStmt([NotNull] RedstarParser.IfStmtContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.attrAppl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterAttrAppl([NotNull] RedstarParser.AttrApplContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.attrAppl"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitAttrAppl([NotNull] RedstarParser.AttrApplContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.exp"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterExp([NotNull] RedstarParser.ExpContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.exp"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitExp([NotNull] RedstarParser.ExpContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.binaryExp"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterBinaryExp([NotNull] RedstarParser.BinaryExpContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.binaryExp"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitBinaryExp([NotNull] RedstarParser.BinaryExpContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.term"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterTerm([NotNull] RedstarParser.TermContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.term"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitTerm([NotNull] RedstarParser.TermContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.varDefine"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterVarDefine([NotNull] RedstarParser.VarDefineContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.varDefine"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitVarDefine([NotNull] RedstarParser.VarDefineContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.attrList"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterAttrList([NotNull] RedstarParser.AttrListContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.attrList"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitAttrList([NotNull] RedstarParser.AttrListContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.funcCall"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterFuncCall([NotNull] RedstarParser.FuncCallContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.funcCall"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitFuncCall([NotNull] RedstarParser.FuncCallContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.operands"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterOperands([NotNull] RedstarParser.OperandsContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.operands"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitOperands([NotNull] RedstarParser.OperandsContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.returnStmt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterReturnStmt([NotNull] RedstarParser.ReturnStmtContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.returnStmt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitReturnStmt([NotNull] RedstarParser.ReturnStmtContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.literal"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterLiteral([NotNull] RedstarParser.LiteralContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.literal"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitLiteral([NotNull] RedstarParser.LiteralContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.literalString"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterLiteralString([NotNull] RedstarParser.LiteralStringContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.literalString"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitLiteralString([NotNull] RedstarParser.LiteralStringContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.literalInt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterLiteralInt([NotNull] RedstarParser.LiteralIntContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.literalInt"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitLiteralInt([NotNull] RedstarParser.LiteralIntContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.literalDecimal"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterLiteralDecimal([NotNull] RedstarParser.LiteralDecimalContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.literalDecimal"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitLiteralDecimal([NotNull] RedstarParser.LiteralDecimalContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.typename"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterTypename([NotNull] RedstarParser.TypenameContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.typename"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitTypename([NotNull] RedstarParser.TypenameContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.ident"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterIdent([NotNull] RedstarParser.IdentContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.ident"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitIdent([NotNull] RedstarParser.IdentContext context);
	/// <summary>
	/// Enter a parse tree produced by <see cref="RedstarParser.op"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void EnterOp([NotNull] RedstarParser.OpContext context);
	/// <summary>
	/// Exit a parse tree produced by <see cref="RedstarParser.op"/>.
	/// </summary>
	/// <param name="context">The parse tree.</param>
	void ExitOp([NotNull] RedstarParser.OpContext context);
}
} // namespace Redstar.Parse
