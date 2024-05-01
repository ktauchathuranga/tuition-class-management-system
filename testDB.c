#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int createTable(const char *tableName, const char *columnDefinitions[], int numColumns)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    char sql[1024] = {0};
    sprintf(sql, "CREATE TABLE %s(", tableName);

    for (int i = 0; i < numColumns; i++)
    {
        strcat(sql, columnDefinitions[i]);
        if (i < numColumns - 1)
        {
            strcat(sql, ",");
        }
    }

    strcat(sql, ");");

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
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

int insertData(const char *tableName, const char *data[], int numData)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    char sql[1024] = {0};
    sprintf(sql, "INSERT INTO %s VALUES(", tableName);

    for (int i = 0; i < numData; i++)
    {
        strcat(sql, data[i]);
        if (i < numData - 1)
        {
            strcat(sql, ",");
        }
    }

    strcat(sql, ");");

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
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

typedef union
{
    int i;
    double d;
    char *s;
} Data;

typedef enum
{
    INTEGER,
    REAL,
    TEXT
} DataType;

char **fetchData(const char *query, DataType type, bool useCallback)
{
    sqlite3 *db;
    sqlite3_stmt *stmt;
    char *zErrMsg = 0;
    int rc;
    char **results = NULL;
    int resultCount = 0;

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    // Count the number of rows that the query will return
    int numRows = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        numRows++;
    }

    // Allocate memory for results
    results = malloc(sizeof(char *) * (numRows + 1)); // +1 for the NULL terminator

    // Reset the prepared statement
    sqlite3_reset(stmt);

    // Fetch the data
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        results[resultCount] = strdup((const char *)sqlite3_column_text(stmt, 0));
        resultCount++;
    }

    results[resultCount] = NULL; // Ensure the array is NULL-terminated

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return results;
}


bool updateData(const char *query)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }

    sqlite3_close(db);
    return true;
}

int main()
{
    // Fetching a single string value
    const char *query = "SELECT FirstName FROM Students WHERE StudentID = 1;";
    char **result = fetchData(query, TEXT, false);
    if (result != NULL) {
        printf("Name: %s\n", result[0]);
        free(result[0]);
        free(result);
    }

    // Fetching multiple string values
    query = "SELECT ClassName FROM Classes;";
    result = fetchData(query, TEXT, false);
    if (result != NULL) {
        for (int i = 0; result[i] != NULL; i++) {
            printf("Name: %s\n", result[i]);
            free(result[i]);
        }
        free(result);
    }

    // Fetching a single integer value
    query = "SELECT ContactNumber FROM Students WHERE StudentID = 666;";
    result = fetchData(query, TEXT, false);
    if (result != NULL) {
        int age = atoi(result[0]);
        printf("Age: %d\n", age);
        free(result[0]);
        free(result);
    }

    // Fetching multiple integer values
    query = "SELECT ContactNUmber FROM Students;";
    result = fetchData(query, TEXT, false);
    if (result != NULL) {
        for (int i = 0; result[i] != NULL; i++) {
            int age = atoi(result[i]);
            printf("Age: %d\n", age);
            free(result[i]);
        }
        free(result);
    }

    //---------------------------------------------------------------------------------

    // Update data
    const char *query2 = "UPDATE Students SET StudentID = 6 WHERE FirstName='amal';";
    updateData(query2);

    //---------------------------------------------------------------------------------

    // Delete data
    const char *query3 = "DELETE FROM COMPANY WHERE NAME='Mark';";
    updateData(query3);

    //---------------------------------------------------------------------------------

    // Insert data
    int stid = 404 ;
    char firstname[100] = "nimal";
    char lastname[100] = "perera";
    char dob[20] = "2000-01-01";
    char contnumber[15] = "0712345678";
    char email[50] = "asd@aasd.com";

    char data1[256];
    sprintf(data1, "%d, '%s', '%s', '%s', '%s', '%s'", stid, firstname, lastname, dob, contnumber, email);

    const char *dataArray[1] = {data1};

    insertData("Students", dataArray, 1);

    return 0;
}

// char query[100];
// sprintf(query, "DELETE FROM COMPANY WHERE NAME='%s';", name);

// deleteData(query);

//--------------------------------------------------------------------------------------

// char data[256];
// sprintf(data, "%d, '%s', '%s', '%s', '%s', '%s'", stid, firstname, lastname, dob, contnumber, email);

// const char* dataArray[1] = {data};

// insertData("Students", dataArray, 1);