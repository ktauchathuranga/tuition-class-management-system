#ifndef FUNC_H
#define FUNC_H

#include <stdbool.h>

int login();
void authSec();
int createTable(const char* tableName, const char* columnDefinitions[], int numColumns);
int insertData(const char* tableName, const char* data[], int numData);
void stdReg();
void teaReg();
void displayMenu();
void browseStd();
void stdSearch();
void feeMng();
void status();
bool fetchData(const char* query, DataType type, Data* data);
bool updateData(const char* query);

#endif
