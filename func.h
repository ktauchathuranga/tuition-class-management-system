#ifndef FUNC_H
#define FUNC_H

#include <stdbool.h>

// typedef union {
//     int i;
//     double d;
//     char* s;
// } Data;

typedef enum {
    INTEGER,
    REAL,
    TEXT
} DataType;

int login();
void authSec();
int createTable(const char* tableName, const char* columnDefinitions[], int numColumns);
int insertData(const char* tableName, const char* data[], int numData);
void stdReg();
void tutReg();
void displayMenu();
void displayStd();
void stdSearch();
void status();
void updateStd();
void deleteStd();
void updateTut();
void deleteTut();
char **fetchData(const char *query, DataType type);
bool updateData(const char* query);
void manageClass();
void manageTutors();
void manageStudents();
void manageFee();
void manageAttendance();
void collectFee();
void dueFee();
void markAttendance();
void checkAttendance();
void status();
void insertEnrollmentData(int stid);
void insertClassData(int tutorid);
void displayclasslist();
void void clearScreen();

#endif
