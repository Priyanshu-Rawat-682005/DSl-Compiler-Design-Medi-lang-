/*
 * executor.h — MediLang Execution Engine
 *
 * Provides functions called by the parser to execute
 * each DSL statement after it has been validated.
 */

#ifndef EXECUTOR_H
#define EXECUTOR_H

void exec_register (const char *id, const char *name, int age);
void exec_prescribe(const char *id, const char *medicine, const char *dosage);
void exec_record   (const char *id, const char *diagnosis);
void exec_validate (const char *id);

#endif /* EXECUTOR_H */
