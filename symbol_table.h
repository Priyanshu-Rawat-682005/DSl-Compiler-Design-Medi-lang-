/*
 * symbol_table.h — MediLang Symbol Table
 *
 * Stores patient records, prescriptions, and diagnoses
 * using a hash-map keyed by patient ID.
 */

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define MAX_PATIENTS 256
#define MAX_ID_LEN   64
#define MAX_NAME_LEN 128
#define MAX_STR_LEN  256

/* ── Linked-list node for prescriptions ── */
typedef struct Prescription {
    char medicine[MAX_STR_LEN];
    char dosage[MAX_STR_LEN];
    struct Prescription *next;
} Prescription;

/* ── Linked-list node for diagnoses ── */
typedef struct Diagnosis {
    char description[MAX_STR_LEN];
    struct Diagnosis *next;
} Diagnosis;

/* ── Patient record ── */
typedef struct Patient {
    char id[MAX_ID_LEN];
    char name[MAX_NAME_LEN];
    int  age;
    int  occupied;              /* 0 = empty slot, 1 = used */
    Prescription *prescriptions;
    Diagnosis    *diagnoses;
} Patient;

/* ── Public API ── */
void     symtab_init(void);
int      patient_exists(const char *id);
Patient *find_patient(const char *id);
int      register_patient(const char *id, const char *name, int age);
int      add_prescription(const char *id, const char *medicine, const char *dosage);
int      add_diagnosis(const char *id, const char *description);
void     print_patient_summary(const char *id);
void     symtab_free(void);

#endif /* SYMBOL_TABLE_H */
