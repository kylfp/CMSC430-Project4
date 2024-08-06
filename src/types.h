// CMSC 430 Compiler Theory and Design
// Project 4 Skeleton
// UMGC CITE
// Summer 2023

// This file contains type definitions and the function
// prototypes for the type checking functions

typedef char* CharPtr;

enum Types {MISMATCH, INT_TYPE, REAL_TYPE, CHAR_TYPE, NONE};

void checkAssignment(Types lValue, Types rValue, string message);
void checkListAssignment(Types listType, Types list, string message);
void checkType(Types expression, Types desired, string message);
Types checkNumericType(Types expression, string message);
void checkRelTypes(Types left, Types right);
Types checkWhen(Types true_, Types false_);
Types checkSwitch(Types case_, Types when, Types other);
Types checkCases(Types left, Types right);
Types checkIfElse(Types ifType, Types elsifType, Types elseType);
Types checkElsif(Types right, Types left);
Types checkFold(Types listType);
Types checkList(Types left, Types right);
Types checkArithmetic(Types left, Types right);
Types checkRemop(Types left, Types right);
