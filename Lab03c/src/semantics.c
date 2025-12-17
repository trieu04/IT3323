/* 
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>
#include <string.h>
#include "semantics.h"
#include "error.h"

extern SymTab* symtab;
extern Token* currentToken;

Object* lookupObject(char *name) {
  Scope* scope = symtab->currentScope;
  Object* obj;

  // Search in current scope and outer scopes
  while (scope != NULL) {
    obj = findObject(scope->objList, name);
    if (obj != NULL) return obj;
    scope = scope->outer;
  }

  // Search in global object list
  obj = findObject(symtab->globalObjectList, name);
  return obj;
}

void checkFreshIdent(char *name) {
  // Check if the identifier already exists in the current scope only
  if (findObject(symtab->currentScope->objList, name) != NULL) {
    error(ERR_DUPLICATE_IDENT, currentToken->lineNo, currentToken->colNo);
  }
}

Object* checkDeclaredIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredConstant(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_CONSTANT) {
    error(ERR_INVALID_CONSTANT, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredType(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_TYPE) {
    error(ERR_INVALID_TYPE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredVariable(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_VARIABLE) {
    error(ERR_INVALID_VARIABLE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredFunction(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_FUNCTION) {
    error(ERR_INVALID_FUNCTION, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredProcedure(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  if (obj->kind != OBJ_PROCEDURE) {
    error(ERR_INVALID_PROCEDURE, currentToken->lineNo, currentToken->colNo);
  }
  return obj;
}

Object* checkDeclaredLValueIdent(char* name) {
  Object* obj = lookupObject(name);
  if (obj == NULL) {
    error(ERR_UNDECLARED_IDENT, currentToken->lineNo, currentToken->colNo);
  }
  
  // L-Value can be: variable, parameter, or current function name
  switch (obj->kind) {
  case OBJ_VARIABLE:
  case OBJ_PARAMETER:
    break;
  case OBJ_FUNCTION:
    // Check if this is the current function (for return value assignment)
    if (obj != symtab->currentScope->owner) {
      error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
    }
    break;
  default:
    error(ERR_INVALID_LVALUE, currentToken->lineNo, currentToken->colNo);
  }
  
  return obj;
}

