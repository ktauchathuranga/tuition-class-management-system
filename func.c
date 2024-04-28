#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <sqlite3.h> 

void sha256(const char *str, char outputBuffer[65]) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int len = EVP_MAX_MD_SIZE;
    
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, str, strlen(str));
    EVP_DigestFinal_ex(mdctx, hash, &len);
    EVP_MD_CTX_free(mdctx);
    
    for(int i = 0; i < len; i++) {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int login() {
    char stored_username_hash[] = "ca978112ca1bbdcafac231b39a23dc4da786eff8147c4e72b9807785afee48bb"; // a
    char stored_password_hash[] = "3e23e8160039594a33894f6564e1b1348bbd7a0088d42c4acb73eeaed59c009d"; // b

    char username[100];
    char password[100];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    char entered_username_hash[65];
    char entered_password_hash[65];
    sha256(username, entered_username_hash);
    sha256(password, entered_password_hash);

    if (strcmp(stored_username_hash, entered_username_hash) == 0 &&
        strcmp(stored_password_hash, entered_password_hash) == 0) {
        printf("Login successful!\n");
        return 1;
    } else {
        printf("Login failed. Please check your username and password.\n");
        return 0;
    }

}

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

void authSec() {
    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // related function
                break;
            case 2:
                // related function
                break;
            case 3:
                // related function
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

}

void displayMenu() {
    // these are just place holders, TO BE CHANGED!
    printf("\n=== Tuition Class Management System ===\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Manage Classes\n");
    printf("0. Exit\n");
}

void stdReg() {

}

void teaReg() {

}

void browseStd() {

}

void stdSearch() {
    // by ID (look at README.md)
}

void feeMng() {
    
}

void status() {

}

int fetchData(const char* query) {
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

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);
   
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        fprintf(stdout, "Data fetched successfully\n");
    }
    sqlite3_close(db);
    return 1;
}

// char query[] = "SELECT * FROM COMPANY;";
// fetchData(query);

