%{
/*
 * medilang.y — Bison Parser for MediLang
 *
 * Defines the grammar, invokes the execution engine on each
 * successfully parsed statement, and reports errors with line numbers.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "executor.h"

/* Flex/Bison glue */
extern int  yylex(void);
extern int  yylineno;
extern FILE *yyin;

void yyerror(const char *msg);

/* Counters */
int syntax_errors   = 0;
int semantic_errors  = 0;
%}

/* ── Value types carried by tokens ── */
%union {
    int  ival;
    char sval[256];
}

/* ── Terminal tokens ── */
%token KW_REGISTER KW_PATIENT KW_AGE
%token KW_PRESCRIBE KW_MEDICINE KW_DOSAGE
%token KW_RECORD KW_DIAGNOSIS
%token KW_VALIDATE

%token <ival> INTEGER
%token <sval> STRING
%token <sval> IDENTIFIER

/* ── Start symbol ── */
%start program

%%

program
    : statement_list
    ;

statement_list
    : statement
    | statement_list statement
    ;

statement
    : register_stmt
    | prescribe_stmt
    | record_stmt
    | validate_stmt
    | error ';'  {
        /* On syntax error Bison consumes tokens until ';' and resumes. */
        fprintf(stderr, "  -> Recovering at line %d, skipping to next statement.\n",
                yylineno);
        yyerrok;
      }
    ;

/* ── REGISTER PATIENT <id> <name> AGE <int> ; ── */
register_stmt
    : KW_REGISTER KW_PATIENT IDENTIFIER STRING KW_AGE INTEGER ';'
      {
          exec_register($3, $4, $6);
      }
    ;

/* ── PRESCRIBE <id> MEDICINE <name> DOSAGE <dosage> ; ── */
prescribe_stmt
    : KW_PRESCRIBE IDENTIFIER KW_MEDICINE STRING KW_DOSAGE STRING ';'
      {
          exec_prescribe($2, $4, $6);
      }
    ;

/* ── RECORD <id> DIAGNOSIS <description> ; ── */
record_stmt
    : KW_RECORD IDENTIFIER KW_DIAGNOSIS STRING ';'
      {
          exec_record($2, $4);
      }
    ;

/* ── VALIDATE <id> ; ── */
validate_stmt
    : KW_VALIDATE IDENTIFIER ';'
      {
          exec_validate($2);
      }
    ;

%%

/* ── Error reporter ── */
void yyerror(const char *msg) {
    fprintf(stderr, "Syntax Error at line %d: %s\n", yylineno, msg);
    syntax_errors++;
}
