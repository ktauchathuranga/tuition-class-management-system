#ifndef FUNC_H
#define FUNC_H

int login();
void authSec();
int createTable(const char* tableName, const char* columnDefinitions[], int numColumns);
int insertData(const char* tableName, const char* data[], int numData);
void stdReg();
void teaReg();


#endif
