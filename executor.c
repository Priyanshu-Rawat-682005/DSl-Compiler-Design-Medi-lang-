/*
 * executor.c — MediLang Execution Engine
 *
 * Each exec_* function performs semantic checks via the symbol table
 * and prints user-friendly output.
 */

#include <stdio.h>
#include "symbol_table.h"
#include "executor.h"
/* semantic_errors is defined in medilang.y (Bison parser) */
extern int semantic_errors;

/* ── REGISTER PATIENT ── */
void exec_register(const char *id, const char *name, int age) {
    if (age <= 0) {
        fprintf(stderr, "Semantic Error: Age must be a positive integer (got %d).\n", age);
        semantic_errors++;
        return;
    }
    if (register_patient(id, name, age) == 0) {
        printf("[OK] Patient Registered Successfully: %s (%s, age %d)\n", id, name, age);
    } else {
        semantic_errors++;
    }
}

/* ── PRESCRIBE ── */
void exec_prescribe(const char *id, const char *medicine, const char *dosage) {
    if (add_prescription(id, medicine, dosage) == 0) {
        printf("[OK] Prescription Added for %s: %s — %s\n", id, medicine, dosage);
    } else {
        semantic_errors++;
    }
}

/* ── RECORD DIAGNOSIS ── */
void exec_record(const char *id, const char *diagnosis) {
    if (add_diagnosis(id, diagnosis) == 0) {
        printf("[OK] Diagnosis Recorded for %s: %s\n", id, diagnosis);
    } else {
        semantic_errors++;
    }
}

/* ── VALIDATE (print full patient summary) ── */
void exec_validate(const char *id) {
    if (!patient_exists(id)) {
        fprintf(stderr, "Semantic Error: Cannot validate — Patient '%s' is not registered.\n", id);
        semantic_errors++;
        return;
    }
    printf("[OK] Validation for Patient %s:\n", id);
    print_patient_summary(id);
}
