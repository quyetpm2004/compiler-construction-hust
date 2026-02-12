/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <ctype.h>
#include "token.h"

struct
{
  char string[MAX_IDENT_LEN + 1];
  TokenType tokenType;
} keywords[KEYWORDS_COUNT] = {
    {"program", KW_PROGRAM},
    {"const", KW_CONST},
    {"type", KW_TYPE},
    {"var", KW_VAR},
    {"integer", KW_INTEGER},
    {"char", KW_CHAR},
    {"array", KW_ARRAY},
    {"of", KW_OF},
    {"function", KW_FUNCTION},
    {"procedure", KW_PROCEDURE},
    {"begin", KW_BEGIN},
    {"end", KW_END},
    {"call", KW_CALL},
    {"if", KW_IF},
    {"then", KW_THEN},
    {"else", KW_ELSE},
    {"while", KW_WHILE},
    {"do", KW_DO},
    {"for", KW_FOR},
    {"to", KW_TO},
    {"break", KW_BREAK},
    {"continue", KW_CONTINUE},
};

int keywordEq(char *kw, char *string)
{
  while ((*kw != '\0') && (*string != '\0'))
  {
    if (*kw != *string)
      break;
    kw++;
    string++;
  }
  return ((*kw == '\0') && (*string == '\0'));
}

TokenType checkKeyword(char *string)
{
  int i;
  for (i = 0; i < KEYWORDS_COUNT; i++)
    if (keywordEq(keywords[i].string, string))
      return keywords[i].tokenType;
  return TK_NONE;
}

Token *makeToken(TokenType tokenType, int lineNo, int colNo)
{
  Token *token = (Token *)malloc(sizeof(Token));
  token->tokenType = tokenType;
  token->lineNo = lineNo;
  token->colNo = colNo;
  return token;
}

char *tokenToString(TokenType tokenType)
{
  switch (tokenType)
  {
  case TK_NONE:
    return "None";
  case TK_IDENT:
    return "an identification";
  case TK_NUMBER:
    return "a number";
  case TK_CHAR:
    return "a constant char";
  case TK_EOF:
    return "end of file";

  case KW_PROGRAM:
    return "keyword PROGRAM";
  case KW_CONST:
    return "keyword CONST";
  case KW_TYPE:
    return "keyword TYPE";
  case KW_VAR:
    return "keyword VAR";
  case KW_INTEGER:
    return "keyword INTEGER";
  case KW_CHAR:
    return "keyword CHAR";
  case KW_ARRAY:
    return "keyword ARRAY";
  case KW_OF:
    return "keyword OF";
  case KW_FUNCTION:
    return "keyword FUNCTION";
  case KW_PROCEDURE:
    return "keyword PROCEDURE";
  case KW_BEGIN:
    return "keyword BEGIN";
  case KW_END:
    return "keyword END";
  case KW_CALL:
    return "keyword CALL";
  case KW_IF:
    return "keyword IF";
  case KW_THEN:
    return "keyword THEN";
  case KW_ELSE:
    return "keyword ELSE";
  case KW_WHILE:
    return "keyword WHILE";
  case KW_DO:
    return "keyword DO";
  case KW_FOR:
    return "keyword FOR";
  case KW_TO:
    return "keyword TO";
  case KW_BREAK:
    return "keyword BREAK";
  case KW_CONTINUE:
    return "keyword CONTINUE";

  case SB_SEMICOLON:
    return "\';\'";
  case SB_COLON:
    return "\':\'";
  case SB_PERIOD:
    return "\'.\'";
  case SB_COMMA:
    return "\',\'";
  case SB_ASSIGN:
    return "\':=\'";
  case SB_EQ:
    return "\'=\'";
  case SB_NEQ:
    return "\'!=\'";
  case SB_LT:
    return "\'<\'";
  case SB_LE:
    return "\'<=\'";
  case SB_GT:
    return "\'>\'";
  case SB_GE:
    return "\'>=\'";
  case SB_PLUS:
    return "\'+\'";
  case SB_MINUS:
    return "\'-\'";
  case SB_TIMES:
    return "\'*\'";
  case SB_SLASH:
    return "\'/\'";
  case SB_LPAR:
    return "\'(\'";
  case SB_RPAR:
    return "\')\'";
  case SB_LSEL:
    return "\'(.\'";
  case SB_RSEL:
    return "\'.)\'";
  default:
    return "";
  }
}
