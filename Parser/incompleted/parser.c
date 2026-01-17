/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 2.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void)
{
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType)
{
  if (lookAhead->tokenType == tokenType)
  {
    // printToken(lookAhead);
    scan();
  }
  else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void)
{
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
}

void compileBlock(void)
{
  if (lookAhead->tokenType == KW_CONST)
  {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  }
  else
    compileBlock2();
}

void compileBlock2(void)
{
  if (lookAhead->tokenType == KW_TYPE)
  {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  }
  else
    compileBlock3();
}

void compileBlock3(void)
{
  if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  }
  else
    compileBlock4();
}

void compileBlock4(void)
{
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void)
{
  while (lookAhead->tokenType == TK_IDENT)
  {
    compileConstDecl();
  }
}

void compileConstDecl(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileConstant();
    eat(SB_SEMICOLON);
  }
}

void compileTypeDecls(void)
{
  // TODO
  while (lookAhead->tokenType == TK_IDENT)
  {
    compileTypeDecl();
  }
}

void compileTypeDecl(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileType(); // type
    eat(SB_SEMICOLON);
  }
}

void compileVarDecls(void)
{
  while (lookAhead->tokenType == TK_IDENT)
  {
    compileVarDecl();
  }
}

void compileVarDecl(void)
{
  eat(TK_IDENT);

  if (lookAhead->tokenType == SB_EQ)
  {
    eat(SB_EQ);
    compileExpression();
  }

  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    eat(TK_IDENT);

    if (lookAhead->tokenType == SB_EQ)
    {
      eat(SB_EQ);
      compileExpression();
    }
  }

  eat(SB_COLON);
  compileType();
  eat(SB_SEMICOLON);
}

void compileSubDecls(void)
{
  while (lookAhead->tokenType == KW_PROCEDURE || lookAhead->tokenType == KW_FUNCTION)
  {
    if (lookAhead->tokenType == KW_PROCEDURE)
      compileProcDecl();
    else
      compileFuncDecl();
  }
}

void compileFuncDecl(void)
{
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);

  if (lookAhead->tokenType == SB_LPAR)
  {
    compileParams();
  }
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
}

void compileProcDecl(void)
{

  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);

  if (lookAhead->tokenType == SB_LPAR)
  {
    compileParams();
  }

  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
}

void compileUnsignedConstant(void)
{
  // TODO
}

void compileConstant(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileConstant2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileConstant2();
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    compileConstant2();
    break;
  }
}

void compileConstant2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileType(void)
{
  switch (lookAhead->tokenType)
  {
  // basic type
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  case KW_ARRAY:
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);
    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    break;
  default:
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileBasicType(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case KW_INTEGER:
    eat(KW_INTEGER);
    break;
  case KW_CHAR:
    eat(KW_CHAR);
    break;
  default:
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileParams(void)
{
  eat(SB_LPAR);
  compileParam();

  while (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileParam();
  }

  eat(SB_RPAR);
}

void compileParams2(void)
{
  // TODO
}

void compileParam(void)
{
  // TODO
  if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);
  }
  eat(TK_IDENT);
  eat(SB_COLON);
  compileBasicType();
}

void compileStatements(void)
{
  // TODO
  compileStatement();
  compileStatements2();
}

void compileStatements2(void)
{
  // TODO
  if (lookAhead->tokenType == SB_SEMICOLON)
  {
    eat(SB_SEMICOLON);
    compileStatement();
    compileStatements2();
  }
}

void compileStatement(void)
{
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void)
{
  // TODO
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL)
  {
    compileIndexes();
  }
  eat(SB_ASSIGN);
  compileExpression();
}

void compileCallSt(void)
{
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  if (lookAhead->tokenType != SB_LPAR)
  {
    return;
  }
  eat(SB_LPAR);
  compileExpression();
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
  }
  eat(SB_RPAR);
}

void compileGroupSt(void)
{ // TODO
  eat(KW_BEGIN);
  if (lookAhead->tokenType != KW_END)
  {
    compileStatement();
    while (lookAhead->tokenType == SB_SEMICOLON)
    {
      eat(SB_SEMICOLON);
      compileStatement();
    }
  }
  eat(KW_END);
}
void compileIfSt(void)
{
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
}

void compileElseSt(void)
{
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void)
{
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
}

void compileForSt(void)
{
  eat(KW_FOR);
  eat(SB_LPAR);

  // n:=10;
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(SB_SEMICOLON);

  // n < 100
  if (lookAhead->tokenType != SB_SEMICOLON)
  {
    compileCondition();
  }
  eat(SB_SEMICOLON);

  // n:=n+1
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();

  eat(SB_RPAR);

  compileStatement();
}

void compileArguments(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    compileArguments2();
    eat(SB_RPAR);
    break;

  case SB_TIMES:
  case SB_SLASH:
  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
  case SB_GT:
  case SB_LT:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_GE:
  case SB_RPAR:
  case SB_RSEL:
  case SB_COMMA:
  case SB_SEMICOLON:
    break;

  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileArguments2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_COMMA:
    eat(SB_COMMA);
    compileExpression();
    compileArguments2();
    break;
  case SB_RPAR:
    break;
  default:
    error(ERR_INVALIDARGUMENTS, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}
void compileCondition(void)
{
  // TODO
  compileExpression();
  switch (lookAhead->tokenType)
  {
  case SB_EQ:
    eat(SB_EQ);
    break;
  case SB_NEQ:
    eat(SB_NEQ);
    break;
  case SB_GT:
    eat(SB_GT);
    break;
  case SB_GE:
    eat(SB_GE);
    break;
  case SB_LT:
    eat(SB_LT);
    break;
  case SB_LE:
    eat(SB_LE);
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
  compileExpression();
}

void compileCondition2(void)
{
  // TODO
}

void compileExpression(void)
{
  // TODO
  if (lookAhead->tokenType == SB_PLUS)
  {
    eat(SB_PLUS);
  }
  else if (lookAhead->tokenType == SB_MINUS)
  {
    eat(SB_MINUS);
  }
  compileTerm();
  compileExpression2();
}

void compileExpression2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_PLUS:
    eat(SB_PLUS);
    compileTerm();
    compileExpression2();
    break;
  case SB_MINUS:
    eat(SB_MINUS);
    compileTerm();
    compileExpression2();
    break;
  case KW_TO:
  case KW_DO:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
  case SB_GT:
  case SB_LT:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_GE:
  case SB_RPAR:
  case SB_RSEL:
  case SB_COMMA:
  case SB_SEMICOLON:
    break;
  default:
    error(ERR_INVALIDEXPRESSION, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileExpression3(void)
{
  // TODO
}

void compileTerm(void)
{
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_TIMES:
    eat(SB_TIMES);
    compileFactor();
    compileTerm2();
    break;
  case SB_SLASH:
    eat(SB_SLASH);
    compileFactor();
    compileTerm2();
    break;

  case SB_PLUS:
  case SB_MINUS:
  case KW_TO:
  case KW_DO:
  case KW_END:
  case KW_ELSE:
  case KW_THEN:
  case SB_GT:
  case SB_LT:
  case SB_EQ:
  case SB_NEQ:
  case SB_LE:
  case SB_GE:
  case SB_RPAR:
  case SB_RSEL:
  case SB_COMMA:
  case SB_SEMICOLON:
    break;

  default:
    error(ERR_INVALIDTERM, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileFactor(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;

  case TK_IDENT:
    eat(TK_IDENT);
    switch (lookAhead->tokenType)
    {
    case SB_LSEL:
      compileIndexes();
      break;
    case SB_LPAR:
      eat(SB_LPAR);
      compileExpression();

      while (lookAhead->tokenType == SB_COMMA)
      {
        eat(SB_COMMA);
        compileExpression();
      }

      eat(SB_RPAR);
      break;
    default:
      break;
    }
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileIndexes(void)
{
  // TODO
  while (lookAhead->tokenType == SB_LSEL)
  {
    eat(SB_LSEL);
    compileExpression();
    eat(SB_RSEL);
  }
}

int compile(char *fileName)
{
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();

  return IO_SUCCESS;
}
