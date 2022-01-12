grammar Redstar;

statement
    : attrDecl
    | bindDecl
    | annoDecl
    | funcDecl
    | ifStmt
    | body

    | varDefine ';'
    | funcCall ';'
    | attrAppl ';'
    | returnStmt ';'
    ;

attrDecl : 'attr' ident ;

annoDecl : 'anno' ('_' | ident | op) bindAnnoConstraint ('=>' bindAnnoLists )? ;
bindDecl : 'bind' ('_' | ident | op) bindAnnoConstraint ('=>' bindAnnoLists )? ;
bindAnnoConstraint
    : bindAnnoLists
    | 'any' bindAnnoAttrList
    | 'all' bindAnnoAttrList
    | 'none' bindAnnoAttrList
    ;
bindAnnoLists : bindAnnoAttrList ( ',' bindAnnoAttrList )* ;
bindAnnoAttrList
    : attrList
    | '_'
    ;

funcDecl : 'func' ident '(' funcParameters? ')' funcReturn? body ;
funcParameters : funcParameter ( ',' funcParameter)* ;
funcParameter : ident ':' typename ;
funcReturn : typename ( ',' typename )* ;
body : '{' statement* '}';

ifStmt : 'if' exp body ( 'else' 'if' exp body )* ( 'else' body)?;

attrAppl : '[' '-'? ident ( ',' '-' ? ident )* ']' ident ( ',' ident)*;

exp
    : '(' varDefine ')'
    | funcCall
    | binaryExp
    | '(' exp ')'
    | ident
    | term
    ;

varDefine : ident ':' ( attrList? typename )? '=' exp ;
attrList : '[' ident ( ',' ident )* ']' ;

funcCall : ident '(' operands? ')' ;
operands : exp ( ',' exp )* ;

returnStmt : 'return' exp ;

binaryExp : term op exp ;
term
    : ident
    | literal
    | '(' exp ')'
    ;

literal
    : literalInt
    | literalDecimal;
literalInt
    : INT ( 'e' '-'? INT )?
    | '(' '-' INT ( 'e' '-'? INT )? ')' ;
literalDecimal
    : INT '.' INT ( 'e' '-'? INT )?
    | '(' '-' INT  '.' INT ( 'e' '-'? INT )? ')' ;

typename
    : 'int'
    | 'string'
    | ident
    ;
ident : IDENT ;
op : OP ;


INT : [0-9]+ ;
OP
    : '<<='
    | '>>='
    | '&&='
    | '||='
    | '??='
    | '??'
    | '=='
    | '!='
    | '<='
    | '>='
    | '>>'
    | '<<'
    | '&&'
    | '||'
    | '&='
    | '|='
    | '%='
    | [+\-*/%<>=&|]
    ;
IDENT
    : [a-zA-Z] [a-zA-Z0-9_] *
    | '_' [a-zA-Z0-9_] + // dis-allows _ as an ident
    ;

WS
    : [ \t\n\r] + -> skip
    ;
COMMENT
    : '//' .*? '\n' -> skip
    ;