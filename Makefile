# ─────────────────────────────────────────────
#  MediLang DSL — Makefile (Flex + Bison)
# ─────────────────────────────────────────────
#  Usage:
#    mingw32-make          — build medilang.exe
#    mingw32-make clean    — remove generated files
#
#  Build pipeline:
#    1. Bison compiles medilang.y → medilang.tab.c + medilang.tab.h
#    2. Flex  compiles medilang.l → lex.yy.c
#    3. GCC   compiles everything → medilang.exe
#
#  Note: lexer.c and parser.c are the hand-coded equivalents
#        kept as backup/reference. They are NOT compiled here.
# ─────────────────────────────────────────────

CC      = gcc
FLEX    = flex
BISON   = bison
CFLAGS  = -Wall -Wno-unused-function -Wno-unused-label

TARGET  = medilang.exe

# Fix GnuWin32 Bison m4 path issue (spaces in "Program Files (x86)")
export BISON_PKGDATADIR = C:/PROGRA~2/GnuWin32/share/bison

# Source files (generated lexer + parser + hand-coded executor & symtab)
SRCS = main.c lex.yy.c medilang.tab.c executor.c symbol_table.c

all: $(TARGET)

# Step 1: Bison generates parser + token header
medilang.tab.c medilang.tab.h: medilang.y
	$(BISON) -d -o medilang.tab.c medilang.y

# Step 2: Flex generates lexer (needs medilang.tab.h for token defines)
lex.yy.c: medilang.l medilang.tab.h
	$(FLEX) -olex.yy.c medilang.l

# Step 3: Compile everything
$(TARGET): $(SRCS) executor.h symbol_table.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	del /Q $(TARGET) lex.yy.c medilang.tab.c medilang.tab.h 2>nul || true

.PHONY: all clean
