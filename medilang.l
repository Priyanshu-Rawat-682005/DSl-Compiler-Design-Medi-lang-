%{
/*
 * medilang.l — Flex Lexer for MediLang
 *
 * Tokenises keywords, identifiers, integers, and quoted strings.
 * Skips whitespace and comments (lines starting with //).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "medilang.tab.h"

extern int yylineno;
%}

%option noyywrap
%option yylineno

%%

 /* ── Keywords ── */
"REGISTER"      { return KW_REGISTER;  }
"PATIENT"       { return KW_PATIENT;   }
"AGE"           { return KW_AGE;       }
"PRESCRIBE"     { return KW_PRESCRIBE; }
"MEDICINE"      { return KW_MEDICINE;  }
"DOSAGE"        { return KW_DOSAGE;    }
"RECORD"        { return KW_RECORD;    }
"DIAGNOSIS"     { return KW_DIAGNOSIS; }
"VALIDATE"      { return KW_VALIDATE;  }

 /* ── Integer literal ── */
[0-9]+          {
                    yylval.ival = atoi(yytext);
                    return INTEGER;
                }

 /* ── Quoted string literal (content without quotes) ── */
\"([^\"\n])*\"  {
                    /* strip surrounding quotes */
                    yytext[yyleng - 1] = '\0';
                    strncpy(yylval.sval, yytext + 1, 255);
                    yylval.sval[255] = '\0';
                    return STRING;
                }

 /* ── Identifier (patient ID, etc.) ── */
[A-Za-z][A-Za-z0-9_]*  {
                    strncpy(yylval.sval, yytext, 255);
                    yylval.sval[255] = '\0';
                    return IDENTIFIER;
                }

 /* ── Semicolon ── */
";"             { return ';'; }

 /* ── Skip whitespace and newlines ── */
[ \t\r\n]+      { /* skip */ }

 /* ── Single-line comments ── */
\/\/.*          { /* skip */ }

 /* ── Anything else is an error ── */
.               {
                    fprintf(stderr,
                        "Lexical Error at line %d: unexpected character '%s'\n",
                        yylineno, yytext);
                }

%%
