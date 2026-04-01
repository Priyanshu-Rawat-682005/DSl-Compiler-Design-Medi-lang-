/*
 * symbol_table.c — MediLang Symbol Table Implementation
 *
 * Uses open-addressing hash map (linear probing) keyed by patient ID.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/* ── Hash table ── */
static Patient table[MAX_PATIENTS];

/* djb2 hash */
static unsigned int hash(const char *str) {
    unsigned int h = 5381;
    int c;
    while ((c = *str++))
        h = ((h << 5) + h) + (unsigned int)c;
    return h % MAX_PATIENTS;
}

/* ── Initialise all slots to empty ── */
void symtab_init(void) {
    memset(table, 0, sizeof(table));
}

/* ── Look up a patient by ID (returns pointer or NULL) ── */
Patient *find_patient(const char *id) {
    unsigned int idx = hash(id);
    unsigned int start = idx;
    do {
        if (table[idx].occupied && strcmp(table[idx].id, id) == 0)
            return &table[idx];
        idx = (idx + 1) % MAX_PATIENTS;
    } while (idx != start);
    return NULL;
}

/* ── Check whether a patient ID is already registered ── */
int patient_exists(const char *id) {
    return find_patient(id) != NULL;
}

/* ── Register a new patient. Returns 0 on success, -1 on error. ── */
int register_patient(const char *id, const char *name, int age) {
    if (patient_exists(id)) {
        fprintf(stderr, "Semantic Error: Patient '%s' is already registered.\n", id);
        return -1;
    }

    unsigned int idx = hash(id);
    unsigned int start = idx;
    do {
        if (!table[idx].occupied) {
            table[idx].occupied = 1;
            strncpy(table[idx].id,   id,   MAX_ID_LEN   - 1);
            strncpy(table[idx].name, name, MAX_NAME_LEN - 1);
            table[idx].age = age;
            table[idx].prescriptions = NULL;
            table[idx].diagnoses     = NULL;
            return 0;
        }
        idx = (idx + 1) % MAX_PATIENTS;
    } while (idx != start);

    fprintf(stderr, "Error: Symbol table is full.\n");
    return -1;
}

/* ── Add a prescription to an existing patient ── */
int add_prescription(const char *id, const char *medicine, const char *dosage) {
    Patient *p = find_patient(id);
    if (!p) {
        fprintf(stderr, "Semantic Error: Patient '%s' is not registered. "
                        "Register the patient before prescribing.\n", id);
        return -1;
    }

    Prescription *rx = (Prescription *)malloc(sizeof(Prescription));
    if (!rx) { perror("malloc"); return -1; }

    strncpy(rx->medicine, medicine, MAX_STR_LEN - 1);
    rx->medicine[MAX_STR_LEN - 1] = '\0';
    strncpy(rx->dosage,   dosage,   MAX_STR_LEN - 1);
    rx->dosage[MAX_STR_LEN - 1] = '\0';

    rx->next = p->prescriptions;
    p->prescriptions = rx;
    return 0;
}

/* ── Add a diagnosis to an existing patient ── */
int add_diagnosis(const char *id, const char *description) {
    Patient *p = find_patient(id);
    if (!p) {
        fprintf(stderr, "Semantic Error: Patient '%s' is not registered. "
                        "Register the patient before recording diagnosis.\n", id);
        return -1;
    }

    Diagnosis *dx = (Diagnosis *)malloc(sizeof(Diagnosis));
    if (!dx) { perror("malloc"); return -1; }

    strncpy(dx->description, description, MAX_STR_LEN - 1);
    dx->description[MAX_STR_LEN - 1] = '\0';

    dx->next = p->diagnoses;
    p->diagnoses = dx;
    return 0;
}

/* ── Print a full patient summary ── */
void print_patient_summary(const char *id) {
    Patient *p = find_patient(id);
    if (!p) {
        fprintf(stderr, "Semantic Error: Patient '%s' is not registered.\n", id);
        return;
    }

    printf("========================================\n");
    printf("  Patient Summary\n");
    printf("========================================\n");
    printf("  ID   : %s\n", p->id);
    printf("  Name : %s\n", p->name);
    printf("  Age  : %d\n", p->age);

    printf("  ---- Prescriptions -------------------\n");
    if (!p->prescriptions) {
        printf("    (none)\n");
    } else {
        int i = 1;
        for (Prescription *rx = p->prescriptions; rx; rx = rx->next, i++)
            printf("    %d. %s — %s\n", i, rx->medicine, rx->dosage);
    }

    printf("  ---- Diagnoses -----------------------\n");
    if (!p->diagnoses) {
        printf("    (none)\n");
    } else {
        int i = 1;
        for (Diagnosis *dx = p->diagnoses; dx; dx = dx->next, i++)
            printf("    %d. %s\n", i, dx->description);
    }
    printf("========================================\n");
}

/* ── Free all dynamic memory ── */
void symtab_free(void) {
    for (int i = 0; i < MAX_PATIENTS; i++) {
        if (!table[i].occupied) continue;

        Prescription *rx = table[i].prescriptions;
        while (rx) {
            Prescription *tmp = rx;
            rx = rx->next;
            free(tmp);
        }

        Diagnosis *dx = table[i].diagnoses;
        while (dx) {
            Diagnosis *tmp = dx;
            dx = dx->next;
            free(tmp);
        }

        table[i].occupied = 0;
    }
}
