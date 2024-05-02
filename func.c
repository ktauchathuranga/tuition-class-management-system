#define _GNU_SOURCE
#include "func.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/evp.h>
#include <sqlite3.h>

void sha256(const char *str, char outputBuffer[65])
{
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int len = EVP_MAX_MD_SIZE;

    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    EVP_DigestInit_ex(mdctx, EVP_sha256(), NULL);
    EVP_DigestUpdate(mdctx, str, strlen(str));
    EVP_DigestFinal_ex(mdctx, hash, &len);
    EVP_MD_CTX_free(mdctx);

    for (int i = 0; i < len; i++)
    {
        sprintf(outputBuffer + (i * 2), "%02x", hash[i]);
    }
    outputBuffer[64] = 0;
}

int login()
{
    char stored_username_hash[] = "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918"; // admin
    char stored_password_hash[] = "8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918"; // admin

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
        strcmp(stored_password_hash, entered_password_hash) == 0)
    {
        printf("Login successful!\n");
        return 1;
    }
    else
    {
        printf("Login failed. Please check your username and password.\n");
        return 0;
    }
}

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

char **fetchData(const char *query, DataType type)
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

    int numRows = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        numRows++;
    }

    results = malloc(sizeof(char *) * (numRows + 1));

    sqlite3_reset(stmt);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        results[resultCount] = strdup((const char *)sqlite3_column_text(stmt, 0));
        resultCount++;
    }

    results[resultCount] = NULL;

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return results;
}

// For delete and update data
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

void authSec()
{
    int choice;

    do
    {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            stdReg();
            // related function
            break;
        case 2:
            displayStd();
            // related function
            break;
        case 3:
            manageClasses();
            // related function
            break;
        case 4:
            tutReg();
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

void displayMenu()
{
    // these are just place holders, TO BE CHANGED!
    printf("\n=== Tuition Class Management System ===\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Update Students\n");
    printf("4. Delete Students\n");
    printf("5. Manage Classes\n");
    printf("6. Add Tutor\n");
    printf("0. Exit\n");
}

void stdReg()
{
    int stid;
    char firstname[100];
    char lastname[100];
    char dob[20];
    char contnumber[15];
    char email[50];

    printf("Enter st id: ");
    scanf("%d", &stid);

    printf("Enter first name: ");
    scanf("%s", firstname);

    printf("Enter last name: ");
    scanf("%s", lastname);

    printf("Enter dob: "); // add format year-month-date
    scanf("%s", dob);

    printf("Enter contact number: ");
    scanf("%s", contnumber);

    printf("Enter your email: ");
    scanf("%s", email);

    char *data;
    asprintf(&data, "%d, '%s', '%s', '%s', '%s', '%s'", stid, firstname, lastname, dob, contnumber, email);

    // Write user details to a text file
    FILE *file = fopen("user_log.txt", "a"); // Open file in append mode
    if (file != NULL) {
        fputs(data, file); // Write data to the file
        fclose(file); // Close the file
    } else {
        printf("Error: Unable to open file.\n");
        return;
    }

    const char *dataArray[1] = {data};

    insertData("Students", dataArray, 1);

    free(data);


    printf("DONE!");
}

void tutReg()
{
    int tutid;
    char firstname[100];
    char lastname[100];
    char subject[100];
    char contnumber[15];
    char email[50];

    printf("Enter tut id: ");
    scanf("%d", &tutid);
    getchar();

    printf("Enter first name: ");
    fgets(firstname, sizeof(firstname), stdin);
    firstname[strcspn(firstname, "\n")] = '\0';

    printf("Enter last name: ");
    fgets(lastname, sizeof(lastname), stdin);
    lastname[strcspn(lastname, "\n")] = '\0';

    printf("Enter subject: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = '\0';

    printf("Enter contact number: ");
    fgets(contnumber, sizeof(contnumber), stdin);
    contnumber[strcspn(contnumber, "\n")] = '\0';

    printf("Enter your email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    char *data;
    asprintf(&data, "%d, '%s', '%s', '%s', '%s', '%s'", tutid, firstname, lastname, subject, contnumber, email);


    // You should ask to verify the data before inserting

    const char *dataArray[1] = {data};

    insertData("Tutors", dataArray, 1);

    free(data);

    printf("DONE!");
}

void dynamicMenu(char **items)
{
    int choice;
    do
    {
        // printf("\n=== Dynamic Menu ===\n");
        for (int i = 0; items[i] != NULL; i++)
        {
            printf("%d. %s\n", i + 1, items[i]);
        }
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice > 0 && items[choice - 1] != NULL)
        {
            printf("You selected: %s\n", items[choice - 1]);
            // call the function for the selected item

            char buffer[256];
            const char *classname = items[choice - 1];

            sprintf(buffer, "SELECT Students.FirstName, Students.LastName FROM Students JOIN Enrollments ON Students.StudentID = Enrollments.StudentID JOIN Classes ON Enrollments.ClassID = Classes.ClassID WHERE Classes.ClassName = '%s';", classname);

            const char *query = buffer;

            char **result = fetchData(query, TEXT);
            if (result != NULL)
            {
                for (int i = 0; result[i] != NULL; i++)
                {
                    printf("Name: %s\n", result[i]);
                    free(result[i]);
                }
                free(result);
            }
        }
        else if (choice == 0)
        {
            printf("Returning to main menu.\n");
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void displayStd()
{

    printf("Select the class: \n");

    const char *query = "SELECT ClassName FROM Classes;";
    char **result = fetchData(query, TEXT);
    if (result != NULL)
    {
        dynamicMenu(result);

        for (int i = 0; result[i] != NULL; i++)
        {
            free(result[i]);
        }
        free(result);
    }
}

void stdSearch()
{
    int stid;
    printf("Enter student id: ");
    scanf("%d", &stid);

    char buffer[256];
    char *fields[] = {"StudentID", "FirstName", "LastName", "DateOfBirth", "ContactNumber", "Email"};
    int numFields = sizeof(fields) / sizeof(fields[0]);

    for (int i = 0; i < numFields; i++)
    {
        sprintf(buffer, "SELECT %s FROM Students WHERE StudentID = %d;", fields[i], stid);
        const char *query = buffer;

        char **result = fetchData(query, TEXT);
        if (result != NULL && result[0] != NULL)
        {
            printf("%s: %s\n", fields[i], result[0]);
            free(result[0]);
        }
        free(result);
    }
}

void feeMng()
{
}

void status()
{
}
void editClassDetails(int classID){
    sqlite3 *db;
    char *errMsg=0;
    int rc;

    rc=sqlite3_open("test.db", &db);
    
    if(rc){
        fprintf(stderr,"Can't open database: %s\n",sqlite3_errmsg(db) );
        sqlite3_close(db);
        return;
    }

    char className[100], classTime[50], classDays[50];

    printf("Enter new class name: ");
    scanf("%s", className);
    printf("Enter new Class Time: ");
    scanf("%s", classTime);
    printf("Enter new Class Days: ");
    scanf("%s", classDays);

    char query[300];
    sprintf(query, "UPDATE Classes SET ClassName = '%s', ClassTime = '%s', ClassDays = '%s' WHERE ClassID = %d;",
            className, classTime, classDays, classID);
    
        rc = sqlite3_exec(db, query, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("/nClass details updated successfully!\n");
    }

    sqlite3_close(db);
}
void deleteClass(int classID) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char query[256];
    sprintf(query, "DELETE FROM Classes WHERE ClassID = %d;", classID);

    rc = sqlite3_exec(db, query, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("\nClass deleted successfully!\n");
    }

    sqlite3_close(db);
}

void updateStd(){
    int stid;
    char firstname[100];
    char lastname[100];
    char dob[20];
    char contnumber[15];
    char email[50];

    printf("Enter student ID to update: ");
    scanf("%d", &stid);

    printf("Enter first name: ");
    scanf("%s", firstname);

    printf("Enter last name: ");
    scanf("%s", lastname);

    printf("Enter dob: ");
    scanf("%s", dob);

    printf("Enter contact number: ");
    scanf("%s", contnumber);

    printf("Enter your email: ");
    scanf("%s", email);

    char data[512];
    sprintf(data, "FirstName='%s', LastName='%s', DateOfBirth='%s', ContactNumber='%s', Email='%s'", firstname, lastname, dob, contnumber, email);

    char query[1024];
    sprintf(query, "UPDATE Students SET %s WHERE StudentID=%d", data, stid);

    if (updateData(query)) {
        printf("Student details updated successfully!\n");
    } else {
        printf("Failed to update student details.\n");
    }

}

void deleteStd(){
    int stid;

    printf("Enter student ID to delete: ");
    scanf("%d", &stid);

    char query[512];
    sprintf(query, "DELETE FROM Students WHERE StudentID=%d", stid);

    if (updateData(query)) {
        printf("Student deleted successfully!\n");
    } else {
        printf("Failed to delete student.\n");
    }
}

void updateTut(){
    int tutid;
    char firstname[100];
    char lastname[100];
    char subject[100];
    char contact[15];
    char email[50];

    printf("Enter tut id to Update: ");
    scanf("%d", &tutid);

    printf("Enter first name: ");
    scanf("%s", firstname);

    printf("Enter last name:");
    scanf("%s", lastname);

    printf("Enter subject: ");
    scanf("%s",subject);

    printf("Enter contact number: ");
    scanf("%s",contact);

    printf("Enter your email: ");
    scanf("%s",email);

    char data[512];
    sprintf(data, "FirstName='%s', LastName='%s', SubjectSpecialization='%s', ContactNumber='%s', Email='%s'", firstname, lastname, subject, contact, email);
 
    char query[1024];
    sprintf(query, "UPDATE Tutors SET %s WHERE TutorID=%d", data, tutid);

    if (updateData(query)) {
        printf("Teacher details updated successfully!\n");
    } else {
        printf("Failed to update teacher details.\n");
    }

}

void deleteTut(){
    int tutid;

    printf("Enter teacher ID to delete: ");
    scanf("%d", &tutid);

    char query[512];
    sprintf(query, "DELETE FROM Tutors WHERE TutorID=%d", tutid);

    if (updateData(query)) {
        printf("Teacher deleted successfully!\n");
    } else {
        printf("Failed to delete teacher.\n");
    }
}

void editClass(){
        int choice,classID;

         printf("\n===Edit Class===\n\n");
         printf("\t1.Edit Class Details\n");
         printf("\t2.Delete Class\n");
         printf("\t3.Back to Manage Class Menu\n");
         printf("\t0. Back to Main Menu\n");
         printf("\nEnter your choice: ");
         scanf("%d",&choice);

         switch (choice){
            case 1:
                printf("Enter Class ID to edit: ");
            scanf("%d", &classID);
            editClassDetails(classID); 
            break;

            case 2:
                printf("Enter Class ID to delete: ");
                scanf("%d",&classID);
                deleteClass(classID);
                break;
            case 3:
                 manageClasses();
                 break;
            case 0:
                authSec();
                // printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice.\n");
                break;
         }
      }

void addClass(){
        int classID,tutorID;
        char className[100],classTime[50],classDays[50];

        printf("Enter Class ID: ");
        scanf("%d", &classID);
        printf("Enter Class Name: ");
        scanf("%s", className);
        printf("Enter Tutor ID: ");
        scanf("%d", &tutorID);
        printf("Enter Class Time: ");
        scanf("%s", classTime);
        printf("Enter Class Days: ");
        scanf("%s", classDays);

        char data[256];
        sprintf(data,"%d, '%s', %d, '%s', '%s'", classID,className,tutorID,classTime,classDays);

        const char* dataArray[1]={data};
        insertData("Classes", dataArray, 1);

        printf("\nClass added successfully!\n");
        
    }

void displayclasslist(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "SELECT * FROM Classes;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n=== Class Details ===\n\n");
    printf("ID\tClass Name\tTutor ID\tClass Time\tClass Days\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int classID = sqlite3_column_int(stmt, 0);
        const unsigned char *className = sqlite3_column_text(stmt, 1);
        int tutorID = sqlite3_column_int(stmt, 2);
        const unsigned char *classTime = sqlite3_column_text(stmt, 3);
        const unsigned char *classDays = sqlite3_column_text(stmt, 4);

        printf("%d\t%s\t%d\t%s\t%s\n", classID, className, tutorID, classTime, classDays);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void manageClasses() {
    int classChoice;
    do {
        printf("\n=== Manage Classes ===\n\n");
        printf("\t1. Add Class\n");
        printf("\t2. Display Classes\n");
        printf("\t3. Edit Class\n");
        printf("\t0. Back to Main Menu\n\n");
        printf("Enter your choice: ");
        scanf("%d", &classChoice);

        switch (classChoice) {
            case 1:
                addClass();// Add class function
                break;
            case 2:
                displayclasslist();// Display classes function
                break;
            case 3:
                editClass();// Edit class function
                break;
            case 0:
                printf("Back to Main Menu");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (classChoice != 0);
}

