// CMSC 430 Compiler Theory and Design
// Project 4 Skeleton
// UMGC CITE
// Summer 2023

// This file contains the bodies of the type checking functions

#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>

using namespace std;

#include "types.h"
#include "listing.h"

/* void checkAssignment(Types lValue, Types rValue, string message) { */
/* 	if (lValue != MISMATCH && rValue != MISMATCH && lValue != rValue) */
/* 		appendError(GENERAL_SEMANTIC, "Type Mismatch on " + message); */
/* } */

void checkAssignment(Types lValue, Types rValue, string message) {
  if (lValue == MISMATCH || rValue == MISMATCH) {
    return;
  }
  if (lValue == rValue) {
    return;
  }
  if (lValue == INT_TYPE && rValue == REAL_TYPE) {
    appendError(GENERAL_SEMANTIC, "Illegal narrowing " + message);
  }
  else {
    appendError(GENERAL_SEMANTIC, "Type mismatch on " + message);
  }
}

void checkListAssignment(Types listType, Types list, string message) {
	if (listType != MISMATCH && list != MISMATCH && listType != list)
		appendError(GENERAL_SEMANTIC, "Type Mismatch on " + message);
}

void checkType(Types expression, Types desired, string message) {
  if (expression != desired) {
    appendError(GENERAL_SEMANTIC, message);
  }
}

Types checkNumericType(Types expression, string message) {
  if (expression == CHAR_TYPE) {
    appendError(GENERAL_SEMANTIC, message + "requires numeric types");
    return MISMATCH;
  }
  return expression;
}

void checkRelTypes(Types left, Types right) {
  if ((left == CHAR_TYPE && right != CHAR_TYPE) || (left != CHAR_TYPE && right == CHAR_TYPE)) {
    appendError(GENERAL_SEMANTIC, "Character Literals cannot be compared to numeric expressions");
  }
}

// When Statements
Types checkWhen(Types true_, Types false_) {
	if (true_ == MISMATCH || false_ == MISMATCH)
		return MISMATCH;
	if (true_ != false_)
		appendError(GENERAL_SEMANTIC, "When Types Mismatch ");
	return true_;
}

// Switch Statements
Types checkSwitch(Types case_, Types when, Types other) {
	if (case_ != INT_TYPE)
		appendError(GENERAL_SEMANTIC, "Switch Expression Not Integer");
	return checkCases(when, other);
}

Types checkCases(Types left, Types right) {
	if (left == MISMATCH || right == MISMATCH)
		return MISMATCH;
	if (left == NONE || left == right)
		return right;
	appendError(GENERAL_SEMANTIC, "Case Types Mismatch");
	return MISMATCH;
}

// If Statements
Types checkIfElse(Types ifType, Types elsifType, Types elseType) {
  if((ifType == elsifType && ifType == elseType) || (ifType == elseType && elsifType == NONE)) {
    return ifType;
  }
  appendError(GENERAL_SEMANTIC, "If-Elsif-Else type mismatch");
  return MISMATCH;

}

Types checkElsif(Types left, Types right) {
  if (left == MISMATCH || right == MISMATCH) {
    return MISMATCH;
  }
  if (left == NONE || left == right) {
    return right;
  }
  appendError(GENERAL_SEMANTIC, "If-Elsif-Else type mismatch");
  return MISMATCH;
}

// Fold statements
Types checkFold(Types listType) {
  if (listType == INT_TYPE || listType == REAL_TYPE) {
    return listType;
  }
  if (listType == MISMATCH) {
    return MISMATCH;
  }
  appendError(GENERAL_SEMANTIC, "Fold requires a numeric list");
  return MISMATCH;
}

// Lists
Types checkList(Types left, Types right) {
  if (left == MISMATCH || right == MISMATCH) {
    return MISMATCH;
  }
  if (left == NONE || left == right) {
    return right;
  }
  appendError(GENERAL_SEMANTIC, "List Types Mismatch");
  return MISMATCH;
}

// Expressions
Types checkArithmetic(Types left, Types right) {
	if (left == MISMATCH || right == MISMATCH) {
		return MISMATCH;
  }
	if (left == INT_TYPE && right == INT_TYPE) {
		return INT_TYPE;
  }
  if (( left == INT_TYPE && right == REAL_TYPE ) || ( left == REAL_TYPE && right == INT_TYPE ) || ( left == REAL_TYPE && right == REAL_TYPE)) {
    return REAL_TYPE;
  }
	appendError(GENERAL_SEMANTIC, "Integer Type Required");
	return MISMATCH;
}

Types checkRemop(Types left, Types right) {
  if (left == MISMATCH && right == MISMATCH) {
    return MISMATCH;
  }
  if (left == INT_TYPE && right == INT_TYPE) {
    return INT_TYPE;
  }
  appendError(GENERAL_SEMANTIC, "Remainder operator requires integer operands");
  return MISMATCH;
}
