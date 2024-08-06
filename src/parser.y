/* CMSC 430 Compiler Theory and Design
   Project 4 Skeleton
   UMGC CITE
   Summer 2023

   Project 4 Parser with semantic actions for static semantic errors */

%{
#include <string>
#include <vector>
#include <map>

using namespace std;

#include "types.h"
#include "listing.h"
#include "symbols.h"

int yylex();
Types find(Symbols<Types>& table, CharPtr identifier, string tableName);
void yyerror(const char* message);

Symbols<Types> scalars;
Symbols<Types> lists;

%}

%define parse.error verbose

%union {
	CharPtr iden;
	Types type;
}

%token <iden> IDENTIFIER

%token <type> INT_LITERAL REAL_LITERAL CHAR_LITERAL

%token ADDOP MULOP REMOP EXPOP NEGOP RELOP ANDOP OROP NOTOP ARROW

%token BEGIN_ CASE CHARACTER ELSE ELSIF END ENDIF ENDFOLD ENDSWITCH FOLD
  FUNCTION IF INTEGER IS LEFT LIST OF OTHERS REAL RETURNS RIGHT
  SWITCH THEN WHEN

%type <type> list list_choice expressions body type statement_ statement cases case_ case expression
	term primary factor arithmetic_negation elsifs elsif function function_header_ function_header

%%

function:
	function_header_ variables body {checkAssignment($1, $3, "of function return");} ;

function_header_:
  function_header ';' {$$ = $1;} |
  error ';' ;

function_header:
	FUNCTION IDENTIFIER optional_parameters RETURNS type {$$ = $5;} ;

optional_parameters:
  parameters |
  %empty ;

parameters:
  parameter ',' parameters |
  parameter ;

parameter:
  IDENTIFIER ':' type ;

type:
	INTEGER {$$ = INT_TYPE;} |
	REAL {$$ = REAL_TYPE;} |
	CHARACTER {$$ = CHAR_TYPE; };

variables:
	variable_ variables |
	%empty ;

variable_:
  variable ';' |
  error ';' ;

variable:
	IDENTIFIER ':' type IS statement {checkAssignment($3, $5, "Variable Initialization"); scalars.insert($1, $3);} |
	IDENTIFIER ':' LIST OF type IS list {checkListAssignment($5, $7, "List Initialization"); lists.insert($1, $5);} ;

list:
	'(' expressions ')' {$$ = $2;} ;

expressions:
	expressions ',' expression {$$ = checkList($1, $3);} |
	expression ;

body:
	BEGIN_ statement_ END ';' {$$ = $2;} ;

statement_:
	statement ';' |
	error ';' {$$ = MISMATCH;} ;

statement:
	expression |
	WHEN condition ',' expression ':' expression {$$ = checkWhen($4, $6);} |
	SWITCH expression IS cases OTHERS ARROW statement ';' ENDSWITCH {$$ = checkSwitch($2, $4, $7);} |
  IF condition THEN statement_ elsifs ELSE statement_ ENDIF {$$ = checkIfElse($4, $5, $7);} |
  FOLD direction operator list_choice ENDFOLD {$$ = checkFold($4);} ;

elsifs:
  elsifs elsif {$$ = checkElsif($1, $2);}|
  %empty {$$ = NONE;};

elsif:
     ELSIF condition THEN statement_ {$$ = $4;} ;

cases:
	cases case_ {$$ = checkCases($1, $2);} |
	%empty {$$ = NONE;} ;

case_:
  case ';' |
  error ';' ;

case:
	CASE INT_LITERAL ARROW statement {$$ = $4;} ;

direction:
LEFT |
RIGHT ;

operator:
  ADDOP |
  MULOP |
  EXPOP ;

list_choice:
  list {$$ = $1;}|
  IDENTIFIER ;

condition:
	condition OROP conjunct |
	conjunct ;

conjunct:
  conjunct ANDOP logical_negation |
  logical_negation ;

logical_negation:
  NOTOP logical_negation |
  relation ;

relation:
	'(' condition')' |
	expression RELOP expression {checkRelTypes($1, $3);} ;

expression:
	expression ADDOP term {$$ = checkArithmetic($1, $3);} |
	term ;

term:
	term MULOP factor {$$ = checkArithmetic($1, $3);} |
  term REMOP factor {$$ = checkRemop($1, $3);} |
	factor ;

factor:
  arithmetic_negation EXPOP factor {checkNumericType($3, "Exponent ");} |
  arithmetic_negation ;

arithmetic_negation:
  NEGOP arithmetic_negation {$$ = checkNumericType($2, "Negation ");} |
  primary ;

primary:
	'(' expression ')' {$$ = $2;} |
	INT_LITERAL |
  REAL_LITERAL |
	CHAR_LITERAL |
	IDENTIFIER '(' expression ')' {checkType($3, INT_TYPE, "List subscript must be integer"); $$ = find(lists, $1, "List");} |
	IDENTIFIER  {$$ = find(scalars, $1, "Scalar");} ;

%%

Types find(Symbols<Types>& table, CharPtr identifier, string tableName) {
	Types type;
	if (!table.find(identifier, type)) {
		appendError(UNDECLARED, tableName + " " + identifier);
		return MISMATCH;
	}
	return type;
}

void yyerror(const char* message) {
	appendError(SYNTAX, message);
}

int main(int argc, char *argv[]) {
	firstLine();
	yyparse();
	lastLine();
	return 0;
}
