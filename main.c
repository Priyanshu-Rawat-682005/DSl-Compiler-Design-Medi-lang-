/*
 * main.c — MediLang Compiler Entry Point
 *
 * Reads a .ml source file (or stdin) and runs the
 * Bison-generated parser (which calls the Flex-generated lexer).
 */

#include <stdio.h>
#include <stdlib.h>
#include "symbol_table.h"

/* Bison / Flex externals */
extern FILE *yyin;
extern int   yyparse(void);
extern int   syntax_errors;
extern int   semantic_errors;

int main(int argc, char *argv[]) {
    FILE *input;

    printf("============================================\n");
    printf("  MediLang Compiler v1.0\n");
    printf("  Healthcare Domain-Specific Language\n");
    printf("============================================\n\n");

    /* Open source file or read from stdin */
    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            perror(argv[1]);
            return EXIT_FAILURE;
        }
        printf("[INFO] Compiling: %s\n\n", argv[1]);
    } else {
        printf("[INFO] Reading from standard input (Ctrl+Z to end on Windows).\n\n");
        input = stdin;
    }

    /* Initialise symbol table */
    symtab_init();

    /* Set Flex input and run Bison parser */
    yyin = input;
    yyparse();

    /* Summary */
    printf("\n============================================\n");
    printf("  Compilation Summary\n");
    printf("============================================\n");
    if (syntax_errors == 0 && semantic_errors == 0) {
        printf("  Result : SUCCESS - no errors detected.\n");
    } else {
        if (syntax_errors > 0)
            printf("  Syntax errors   : %d\n", syntax_errors);
        if (semantic_errors > 0)
            printf("  Semantic errors  : %d\n", semantic_errors);
        printf("  Result : FAILED\n");
    }
    printf("============================================\n");

    /* Clean up */
    symtab_free();
    if (input != stdin) fclose(input);

    return (syntax_errors || semantic_errors) ? EXIT_FAILURE : EXIT_SUCCESS;
}
