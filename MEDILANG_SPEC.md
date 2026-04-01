# MediLang Language Specification & Design Document

## 1. Introduction
MediLang is a Domain-Specific Language (DSL) created to streamline hospital operations, specifically focusing on patient registration, prescription management, and diagnostic recording. This document defines the lexical elements, syntax rules, and architectural backbone of the compiler.

## 2. Token Definitions (Lexical Rules)

### 2.1 Keywords
The language reserves the following uppercase keywords: 
- `REGISTER`, `PATIENT`, `AGE`
- `PRESCRIBE`, `MEDICINE`, `DOSAGE`
- `RECORD`, `DIAGNOSIS`
- `VALIDATE`

### 2.2 Data Types
- **IDENTIFIER**: A character sequence starting with a letter, optionally followed by alphanumeric characters or underscores (e.g., `Patient1`, `ravi_123`).
- **STRING**: Any literal text enclosed in double quotes (e.g., `"Metformin"`, `"Ravi"`). 
- **INTEGER**: A sequence of contiguous numerical digits (e.g., `45`, `30`).

### 2.3 Other Tokens
- **STATEMENT TERMINATOR**: Every statement must end with a semicolon `;`
- **COMMENTS**: Single-line comments are supported using double forward slashes `//`

---

## 3. Formal Grammar (BNF)
The syntax of MediLang is defined by the following Context-Free Grammar:

```ebnf
program         ::= statement_list

statement_list  ::= statement
                  | statement_list statement

statement       ::= register_stmt
                  | prescribe_stmt
                  | record_stmt
                  | validate_stmt

register_stmt   ::= "REGISTER" "PATIENT" IDENTIFIER STRING "AGE" INTEGER ";"

prescribe_stmt  ::= "PRESCRIBE" IDENTIFIER "MEDICINE" STRING "DOSAGE" STRING ";"

record_stmt     ::= "RECORD" IDENTIFIER "DIAGNOSIS" STRING ";"

validate_stmt   ::= "VALIDATE" IDENTIFIER ";"
```

---

## 4. Language Features & Semantic Rules

### 4.1 Patient Registration
Registers a new patient with their unique ID, full name, and age.
**Syntax:** `REGISTER PATIENT <id> "<name>" AGE <int>;`
**Rule:** Age must be a positive integer. If the `<id>` already exists, it should flag a duplicate symbol error.

### 4.2 Diagnostic Recording
Appends a diagnosis to a registered patient's record.
**Syntax:** `RECORD <id> DIAGNOSIS "<description>";`
**Rule:** The `<id>` must belong to an already registered patient. 

### 4.3 Prescription Generation
Assigns medicine and dosage to a patient.
**Syntax:** `PRESCRIBE <id> MEDICINE "<medicine_name>" DOSAGE "<dosage_amount>";`
**Rule:** The `<id>` must refer to a registered patient. The DOSAGE string must include a unit (e.g., "500mg"). Duplicate medicines should be handled gracefully.

### 4.4 Validation
Executes all checks for a specific patient to verify their record makes medical & logical sense.
**Syntax:** `VALIDATE <id>;`

---

## 5. Architectural Design
The Compiler pipeline is composed of the following sequential stages:

1. **Lexical Analysis (Tokenizer)**
   - Tool: `Flex` (`medilang.l`)
   - Description: Reads raw text and converts valid character sequences into language tokens, skipping whitespace and comments. Unrecognized tokens emit lexical errors.
2. **Syntax Analysis (Parser)**
   - Tool: `Bison/Yacc` (`medilang.y`)
   - Description: Validates token chains against the formal BNF rules. Builds the conceptual abstract syntax tree (AST). Syntax errors attempt smart recovery (skipping to the next `;`).
3. **Semantic Analysis (Symbol Table Manager)**
   - Logic: `C` (`symbol_table.c`, `symbol_table.h`)
   - Description: Maintains the memory mappings, enforcing the logical rules of the DSL (e.g., variables must be initialized/`REGISTER`ed before use).
4. **Execution Engine**
   - Logic: `C` (`executor.c`, `executor.h`)
   - Description: Emits the expected output—either a JSON medical report or a formatted hospital print-out.

---

## 6. Sample Program
```medilang
// Register a new patient
REGISTER PATIENT P001 "Ravi" AGE 45;

// Record their diagnosis
RECORD P001 DIAGNOSIS "Diabetes";

// Prescribe medication
PRESCRIBE P001 MEDICINE "Metformin" DOSAGE "500mg";

// Validate final record
VALIDATE P001;
```
