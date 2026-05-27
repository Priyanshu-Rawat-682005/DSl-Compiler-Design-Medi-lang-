# MediLang Compiler

MediLang is a Domain-Specific Language (DSL) specifically designed to streamline healthcare and hospital operations. It provides a simple, readable syntax for patient registration, prescription management, and diagnostic recording, with built-in validation to ensure data integrity.

## Features

- **Patient Registration:** Register new patients with unique IDs, names, and ages.
- **Diagnostic Recording:** Append diagnoses to registered patients.
- **Prescription Management:** Assign medicines and dosages securely.
- **Validation:** Execute rule-based checks on a patient's medical records to ensure logical consistency.

## Architecture

The MediLang compiler is built with a robust pipeline:
1. **Lexical Analysis:** Powered by `Flex`, converting raw source text into validated language tokens.
2. **Syntax Analysis:** Powered by `Bison/Yacc`, evaluating token streams against an LALR(1) Context-Free Grammar.
3. **Semantic Analysis:** A custom C-based Symbol Table manager that enforces semantic rules (e.g., duplicate IDs, unregistered operations).
4. **Execution Engine:** A C-based execution engine that outputs structured hospital reports.

## Getting Started

### Prerequisites

To build and run the MediLang compiler, you will need:
- GCC (GNU Compiler Collection)
- Flex (Fast Lexical Analyzer)
- Bison (GNU Parser Generator)
- Make

### Building the Project

The project includes a `Makefile` for streamlined compilation. Simply run the following command in the root directory:

```bash
make
```

This will generate the `medilang` executable (or `medilang.exe` on Windows).

### Running a Program

You can execute a MediLang source file by passing it to the compiled executable. For example:

```bash
./medilang tests/test_basic.ml
```

### Syntax Overview

MediLang scripts are composed of statements ending with semicolons `;`. Single-line comments are supported using `//`.

#### Example Program

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

## Language Specifications

For a detailed breakdown of the grammar, lexical rules, and semantic checks, please refer to the `MEDILANG_SPEC.md` document included in the repository.

## Contribution

This project was developed collaboratively, demonstrating compiler design principles using standard tools like Flex and Bison.

- **Frontend & Web Integration:** Includes a web-based frontend to execute and test the DSL.
- **Backend & Core:** Core components are written in C for performance and reliability.
