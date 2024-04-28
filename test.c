#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <sqlite3.h> 

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

int createTable(const char* tableName, const char* columnDefinitions[], int numColumns) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    char sql[1024] = {0};
    sprintf(sql, "CREATE TABLE %s(", tableName);

    for(int i = 0; i < numColumns; i++) {
        strcat(sql, columnDefinitions[i]);
        if(i < numColumns - 1) {
            strcat(sql, ",");
        }
    }

    strcat(sql, ");");

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
    return 1;
}

// const char* columnDefinitions[] = {
//     "ID INT PRIMARY KEY     NOT NULL",
//     "NAME           TEXT    NOT NULL",
//     "AGE            INT     NOT NULL",
//     "ADDRESS        CHAR(50)",
//     "SALARY         REAL"
// };

// createTable("COMPANY", columnDefinitions, 5);

int insertData(const char* tableName, const char* data[], int numData) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO %s VALUES(", tableName);

    for(int i = 0; i < numData; i++) {
        strcat(sql, data[i]);
        if(i < numData - 1) {
            strcat(sql, ",");
        }
    }

    strcat(sql, ");");

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
    return 1;
}

// const char* data[] = {
//     "1, 'Paul', 32, 'California', 20000.00",
//     "2, 'Allen', 25, 'Texas', 15000.00",
//     "3, 'Teddy', 23, 'Norway', 20000.00",
//     "4, 'Mark', 25, 'Rich-Mond ', 65000.00"
// };

// for(int i = 0; i < 4; i++) {
//     insertData("COMPANY", &data[i], 1);
// }

#include <stdbool.h>

typedef union {
    int i;
    double d;
    char* s;
} Data;

typedef enum {
    INTEGER,
    REAL,
    TEXT
} DataType;

bool fetchData(const char* query, DataType type, Data* data, bool useCallback) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return false;
    } 

    if (useCallback) {
        rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            fprintf(stderr, "SQL error: %s\n", zErrMsg);
            sqlite3_free(zErrMsg);
        } else {
            fprintf(stdout, "Operation done successfully\n");
        }
    } else {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_ROW) {
            switch (type) {
                case INTEGER:
                    data->i = sqlite3_column_int(stmt, 0);
                    break;
                case REAL:
                    data->d = sqlite3_column_double(stmt, 0);
                    break;
                case TEXT:
                    data->s = strdup((const char*)sqlite3_column_text(stmt, 0));
                    break;
            }
        } else if (rc != SQLITE_DONE) {
            fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        }
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}


bool updateData(const char* query) {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
   
    if( rc ) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    } else {
        fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(db);
    return true;
}


int main() {
    const char* query;
    Data data;
    bool useCallback;

    // Example 1: Fetch a single integer value
    query = "SELECT age FROM COMPANY WHERE name='Paul';";
    fetchData(query, INTEGER, &data, false);
    printf("Paul's age is %d\n", data.i);

    // Example 2: Fetch a single real value
    query = "SELECT salary FROM COMPANY WHERE name='Allen';";
    fetchData(query, REAL, &data, false);
    printf("Allen's salary is %.2f\n", data.d);

    // Example 3: Fetch a single text value
    query = "SELECT address FROM COMPANY WHERE name='Teddy';";
    fetchData(query, TEXT, &data, false);
    printf("Teddy's address is %s\n", data.s);
    free(data.s);  // Don't forget to free the string when you're done with it

    // Example 4: Fetch and print multiple records using the callback function
    query = "SELECT * FROM COMPANY;";
    fetchData(query, INTEGER, NULL, true);  // The second and third arguments are ignored when useCallback is true

    //---------------------------------------------------------------------------------

    // Update data
    const char* query2 = "UPDATE COMPANY SET AGE = 33 WHERE NAME='Paul';";
    updateData(query2);

    //---------------------------------------------------------------------------------

    // Delete data
    const char* query3 = "DELETE FROM COMPANY WHERE NAME='Mark';";
    updateData(query3);

    return 0;
}
