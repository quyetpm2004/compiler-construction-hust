/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.1
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symtab.h"

void freeObject(Object *obj);
void freeScope(Scope *scope);
void freeObjectList(ObjectNode *objList);
void freeReferenceList(ObjectNode *objList);

SymTab *symtab;
Type *intType;
Type *charType;

/******************* Type utilities ******************************/

Type *makeIntType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_INT;
  return type;
}

Type *makeCharType(void)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_CHAR;
  return type;
}

Type *makeArrayType(int arraySize, Type *elementType)
{
  Type *type = (Type *)malloc(sizeof(Type));
  type->typeClass = TP_ARRAY;
  type->arraySize = arraySize;
  type->elementType = elementType;
  return type;
}

Type *duplicateType(Type *type)
{
  // TODO
  if (type == NULL)
    return NULL;
  Type *newType = (Type *)malloc(sizeof(Type));
  newType->typeClass = type->typeClass;
  if (type->typeClass == TP_ARRAY)
  {
    newType->arraySize = type->arraySize;
    newType->elementType = duplicateType(type->elementType); // Đệ quy tạo bản sao của elementType
  }
  return newType;
}

int compareType(Type *type1, Type *type2)
{
  // TODO
  if (type1 == type2)
    return 1;
  if (type1 == NULL || type2 == NULL)
    return 0;
  if (type1->typeClass != type2->typeClass)
    return 0;

  switch (type1->typeClass)
  {
  case TP_INT:
  case TP_CHAR:
    return 1;
  case TP_ARRAY:
    return (type1->arraySize == type2->arraySize) && compareType(type1->elementType, type2->elementType);
  default:
    return 0;
  }
}

void freeType(Type *type)
{
  // TODO
  if (type != NULL)
  {
    if (type->typeClass == TP_ARRAY)
    {
      freeType(type->elementType);
    }
    free(type);
  }
}

/******************* Constant utility ******************************/

ConstantValue *makeIntConstant(int i)
{
  // TODO
  ConstantValue *v = (ConstantValue *)malloc(sizeof(ConstantValue));
  v->type = TP_INT;
  v->intValue = i;
  return v;
}

ConstantValue *makeCharConstant(char ch)
{
  // TODO
  ConstantValue *v = (ConstantValue *)malloc(sizeof(ConstantValue));
  v->type = TP_CHAR;
  v->charValue = ch;
  return v;
}

ConstantValue *duplicateConstantValue(ConstantValue *v)
{
  // TODO
  ConstantValue *newV = (ConstantValue *)malloc(sizeof(ConstantValue));
  *newV = *v;
  return newV;
}

/******************* Object utilities ******************************/

Scope *CreateScope(Object *owner, Scope *outer)
{
  Scope *scope = (Scope *)malloc(sizeof(Scope));
  scope->objList = NULL;
  scope->owner = owner;
  scope->outer = outer;
  return scope;
}

Object *CreateProgramObject(char *programName)
{
  Object *program = (Object *)malloc(sizeof(Object));
  strcpy(program->name, programName);
  program->kind = OBJ_PROGRAM;
  program->progAttrs = (ProgramAttributes *)malloc(sizeof(ProgramAttributes));
  program->progAttrs->scope = CreateScope(program, NULL);
  symtab->program = program;

  return program;
}

Object *CreateConstantObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_CONSTANT;
  obj->constAttrs = (ConstantAttributes *)malloc(sizeof(ConstantAttributes));
  obj->constAttrs->value = NULL;
  return obj;
}

Object *CreateTypeObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_TYPE;
  obj->typeAttrs = (TypeAttributes *)malloc(sizeof(TypeAttributes));
  obj->typeAttrs->actualType = NULL;
  return obj;
}

Object *CreateVariableObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_VARIABLE;
  obj->varAttrs = (VariableAttributes *)malloc(sizeof(VariableAttributes));
  obj->varAttrs->type = NULL;
  // obj->varAttrs->scope = symtab->currentScope; // Scope có thể được suy ra từ objList
  return obj;
}

Object *CreateFunctionObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_FUNCTION;
  obj->funcAttrs = (FunctionAttributes *)malloc(sizeof(FunctionAttributes));
  obj->funcAttrs->paramList = NULL;
  obj->funcAttrs->returnType = NULL;
  // Tạo Scope mới, Scope ngoài là Scope hiện tại
  obj->funcAttrs->scope = CreateScope(obj, symtab->currentScope);
  return obj;
}

Object *CreateProcedureObject(char *name)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PROCEDURE;
  obj->procAttrs = (ProcedureAttributes *)malloc(sizeof(ProcedureAttributes));
  obj->procAttrs->paramList = NULL;
  // Tạo Scope mới, Scope ngoài là Scope hiện tại
  obj->procAttrs->scope = CreateScope(obj, symtab->currentScope);
  return obj;
}

Object *CreateParameterObject(char *name, enum ParamKind kind, Object *owner)
{
  // TODO
  Object *obj = (Object *)malloc(sizeof(Object));
  strcpy(obj->name, name);
  obj->kind = OBJ_PARAMETER;
  obj->paramAttrs = (ParameterAttributes *)malloc(sizeof(ParameterAttributes));
  obj->paramAttrs->kind = kind;
  obj->paramAttrs->type = NULL;      // Kiểu sẽ được gán sau
  obj->paramAttrs->function = owner; // Thủ tục/Hàm sở hữu tham số
  return obj;
}

void freeObject(Object *obj)
{
  // TODO
  if (obj != NULL)
  {
    switch (obj->kind)
    {
    case OBJ_PROGRAM:
      freeScope(obj->progAttrs->scope);
      free(obj->progAttrs);
      break;
    case OBJ_CONSTANT:
      free(obj->constAttrs->value);
      free(obj->constAttrs);
      break;
    case OBJ_TYPE:
      freeType(obj->typeAttrs->actualType);
      free(obj->typeAttrs);
      break;
    case OBJ_VARIABLE:
      free(obj->varAttrs);
      break;
    case OBJ_FUNCTION:
      freeReferenceList(obj->funcAttrs->paramList);
      freeType(obj->funcAttrs->returnType);
      freeScope(obj->funcAttrs->scope);
      free(obj->funcAttrs);
      break;
    case OBJ_PROCEDURE:
      freeReferenceList(obj->procAttrs->paramList);
      freeScope(obj->procAttrs->scope);
      free(obj->procAttrs);
      break;
    case OBJ_PARAMETER:
      free(obj->paramAttrs);
      break;
    }
    free(obj);
  }
}

void freeScope(Scope *scope)
{
  // TODO
  if (scope != NULL)
  {
    freeObjectList(scope->objList); // Giải phóng tất cả Object trong Scope
    free(scope);
  }
}

void freeObjectList(ObjectNode *objList)
{
  // TODO
  ObjectNode *current = objList;
  while (current != NULL)
  {
    ObjectNode *next = current->next;
    freeObject(current->object); // Giải phóng cả Object
    free(current);               // Giải phóng Node
    current = next;
  }
}

void freeReferenceList(ObjectNode *objList)
{
  // TODO
  ObjectNode *current = objList;
  while (current != NULL)
  {
    ObjectNode *next = current->next;
    // KHÔNG gọi freeObject(current->object);
    free(current); // Chỉ giải phóng Node
    current = next;
  }
}

void AddObject(ObjectNode **objList, Object *obj)
{
  ObjectNode *node = (ObjectNode *)malloc(sizeof(ObjectNode));
  node->object = obj;
  node->next = NULL;
  if ((*objList) == NULL)
    *objList = node;
  else
  {
    ObjectNode *n = *objList;
    while (n->next != NULL)
      n = n->next;
    n->next = node;
  }
}

Object *findObject(ObjectNode *objList, char *name)
{
  // TODO
  ObjectNode *current = objList;
  while (current != NULL)
  {
    if (strcmp(current->object->name, name) == 0)
    {
      return current->object; // Tìm thấy
    }
    current = current->next;
  }
  return NULL; // Không tìm thấy
}

/******************* others ******************************/

void initSymTab(void)
{
  Object *obj;
  Object *param;

  symtab = (SymTab *)malloc(sizeof(SymTab));
  symtab->globalObjectList = NULL;

  obj = CreateFunctionObject("READC");
  obj->funcAttrs->returnType = makeCharType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateFunctionObject("READI");
  obj->funcAttrs->returnType = makeIntType();
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEI");
  param = CreateParameterObject("i", PARAM_VALUE, obj);
  param->paramAttrs->type = makeIntType();
  AddObject(&(obj->procAttrs->paramList), param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITEC");
  param = CreateParameterObject("ch", PARAM_VALUE, obj);
  param->paramAttrs->type = makeCharType();
  AddObject(&(obj->procAttrs->paramList), param);
  AddObject(&(symtab->globalObjectList), obj);

  obj = CreateProcedureObject("WRITELN");
  AddObject(&(symtab->globalObjectList), obj);

  intType = makeIntType();
  charType = makeCharType();
}

void cleanSymTab(void)
{
  freeObject(symtab->program);
  freeObjectList(symtab->globalObjectList);
  free(symtab);
  freeType(intType);
  freeType(charType);
}

void enterBlock(Scope *scope)
{
  symtab->currentScope = scope;
}

void exitBlock(void)
{
  symtab->currentScope = symtab->currentScope->outer;
}

void declareObject(Object *obj)
{
  if (obj->kind == OBJ_PARAMETER)
  {
    Object *owner = symtab->currentScope->owner;
    switch (owner->kind)
    {
    case OBJ_FUNCTION:
      AddObject(&(owner->funcAttrs->paramList), obj);
      break;
    case OBJ_PROCEDURE:
      AddObject(&(owner->procAttrs->paramList), obj);
      break;
    default:
      break;
    }
  }

  AddObject(&(symtab->currentScope->objList), obj);
}
