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

    printf("\n");
    printf("========================================\n");
    printf("=             Login Screen             =\n");
    printf("========================================\n");
    printf("[*] Enter Username: ");
    scanf("%s", username);
    printf("[*] Enter Password: ");
    scanf("%s", password);

    char entered_username_hash[65];
    char entered_password_hash[65];
    sha256(username, entered_username_hash);
    sha256(password, entered_password_hash);

    if (strcmp(stored_username_hash, entered_username_hash) == 0 &&
        strcmp(stored_password_hash, entered_password_hash) == 0)
    {
        printf("[+] Login Successful!\n");
        return 1;
    }
    else
    {
        printf("[!] Login failed. Please Check Your Username and Password Correctly... :(\n");
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
        fprintf(stderr, "[!] Can't open Database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        // fprintf(stdout, "Opened database successfully\n");
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
        fprintf(stderr, "[!] SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "[+] Table created successfully\n");
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
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        // fprintf(stdout, "Opened database successfully\n");
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
        fprintf(stderr, "[!] SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        // fprintf(stdout, "Records created successfully\n");
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
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        return NULL;
    }
    else
    {
        // fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] SQL error: %s\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        return false;
    }
    else
    {
        // fprintf(stdout, "Opened database successfully\n");
    }

    rc = sqlite3_exec(db, query, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "[+] Operation done Successfully\n");
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
        printf("[-] Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            manageStudents();
            // related function
            break;
        case 2:
            manageTutors();
            // related function
            break;
        case 3:
            manageClass();
            // related function
            break;
        case 4:
            displayStd();
            // related function
            break;
        case 5:
            stdSearch();
            // related function
            break;
        case 6:
            manageFee();
            // related function
            break;
        case 7:
            manageAttendance();
            // related function
            break;
        case 8:
            status();
            // related function
            break;
        case 0:
            printf("Exiting program.\n");
            break;
        default:
            printf("[*] Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void displayMenu()
{
    // these are just place holders, TO BE CHANGED!
    printf("\n");
    printf("======================================\n");
    printf("=       Tuition Class Management     =\n");
    printf("======================================\n");
    printf("1. Manage Students\n");
    printf("2. Manage Tutors\n");
    printf("3. Manage Classes\n");
    printf("4. Browse Students (by class)\n");
    printf("5. Search Studnet (by ID)\n");
    printf("6. Fee Manage\n");
    printf("7. Attendance\n");
    printf("8. Status\n");
    printf("0. Exit\n");
    printf("=======================================\n");
}

void stdReg()
{
    int stid;
    char firstname[100];
    char lastname[100];
    char dob[20];
    char contnumber[15];
    char email[50];

    printf("[-] Enter Student ID: ");
    scanf("%d", &stid);

    printf("[-] Enter First Name: ");
    scanf("%s", firstname);

    printf("[-] Enter Last Name: ");
    scanf("%s", lastname);

    printf("[-] Enter Date of Birth (YYYY-MM-DD): "); // add format year-month-date
    scanf("%s", dob);

    printf("[-] Enter Contact Number: ");
    scanf("%s", contnumber);

    printf("[-] Enter Your Email: ");
    scanf("%s", email);

    char *data;
    asprintf(&data, "%d, '%s', '%s', '%s', '%s', '%s'", stid, firstname, lastname, dob, contnumber, email);

    // Write user details to a text file
    FILE *file = fopen("user_log.txt", "a"); // Open file in append mode
    if (file != NULL) {
        fputs(data, file); // Write data to the file
        fclose(file); // Close the file
    } else {
        printf("[!] Error: Unable to open file.\n");
        return;
    }

    const char *dataArray[1] = {data};

    insertData("Students", dataArray, 1);

    free(data);

    insertEnrollmentData(stid); // To update enrollment table


    // printf("DONE!");
}

void tutReg()
{
    int tutid;
    char firstname[100];
    char lastname[100];
    char subject[100];
    char contnumber[15];
    char email[50];

    printf("[-] Enter Tutor ID: ");
    scanf("%d", &tutid);
    getchar();

    printf("[-] Enter First Name: ");
    fgets(firstname, sizeof(firstname), stdin);
    firstname[strcspn(firstname, "\n")] = '\0';

    printf("[-] Enter Last Name: ");
    fgets(lastname, sizeof(lastname), stdin);
    lastname[strcspn(lastname, "\n")] = '\0';

    printf("[-] Enter Subject Specialization: ");
    fgets(subject, sizeof(subject), stdin);
    subject[strcspn(subject, "\n")] = '\0';

    printf("[-] Enter Contact Number: ");
    fgets(contnumber, sizeof(contnumber), stdin);
    contnumber[strcspn(contnumber, "\n")] = '\0';

    printf("[-] Enter Your Email: ");
    fgets(email, sizeof(email), stdin);
    email[strcspn(email, "\n")] = '\0';

    char *data;
    asprintf(&data, "%d, '%s', '%s', '%s', '%s', '%s'", tutid, firstname, lastname, subject, contnumber, email);


    // You should ask to verify the data before inserting

    const char *dataArray[1] = {data};

    insertData("Tutors", dataArray, 1);

    free(data);

    insertClassData(tutid); // To update class table

    // printf("DONE!");
}

void dynamicMenu(char **items)
{
    int choice;
    do
    {
        printf("\n============= Class  List =============\n");
        for (int i = 0; items[i] != NULL; i++)
        {
            printf("%d. %s\n", i + 1, items[i]);
        }
        printf("0. Back to Main Menu\n");
        printf("=======================================\n");

        printf("[-] Enter your choice: ");
        scanf("%d", &choice);
        printf("\n");

        if (choice > 0 && items[choice - 1] != NULL)
        {
            // printf("You selected: %s\n", items[choice - 1]);
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
                    printf("%d: %s\n", (i + 1), result[i]);
                    free(result[i]);
                }
                free(result);
            }
        }
        else if (choice == 0)
        {
            printf("[|] Returning to main menu.\n");
        }
        else
        {
            printf("[!] Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}

void displayStd()
{

    printf("[-] Select the class: \n");

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
    printf("[-] Enter Student ID: ");
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
            printf("%d. %s: %s\n", (i + 1), fields[i], result[0]);
            free(result[0]);
        }
        free(result);
    }
}

void status()
{
    const char *studentQuery = "SELECT COUNT(*) FROM Students;";
    char **studentResult = fetchData(studentQuery, TEXT);
    int numStudents = 0;
    if (studentResult != NULL && studentResult[0] != NULL)
    {
        numStudents = atoi(studentResult[0]);
        free(studentResult[0]);
        free(studentResult);
    }

    const char *tutorQuery = "SELECT COUNT(*) FROM Tutors;";
    char **tutorResult = fetchData(tutorQuery, TEXT);
    int numTutors = 0;
    if (tutorResult != NULL && tutorResult[0] != NULL)
    {
        numTutors = atoi(tutorResult[0]);
        free(tutorResult[0]);
        free(tutorResult);
    }

    const char *classQuery = "SELECT COUNT(*) FROM Classes;";
    char **classResult = fetchData(classQuery, TEXT);
    int numClasses = 0;
    if (classResult != NULL && classResult[0] != NULL)
    {
        numClasses = atoi(classResult[0]);
        free(classResult[0]);
        free(classResult);
    }

    printf("\n");
    printf("=======================================\n");
    printf("=               Summary               =\n");
    printf("=======================================\n");
    printf("[~] Number of students: %d\n", numStudents);
    printf("[~] Number of tutors:   %d\n", numTutors);
    printf("[~] Number of classes:  %d\n", numClasses);
    printf("=======================================\n");

}

void editClassDetails(int classID){
    sqlite3 *db;
    char *errMsg=0;
    int rc;

    rc=sqlite3_open("test.db", &db);
    
    if(rc){
        fprintf(stderr,"[!] Can't open database: %s\n",sqlite3_errmsg(db) );
        sqlite3_close(db);
        return;
    }

    char className[100], classTime[50], classDays[50];

    printf("[-] Enter New Class Name: ");
    scanf("%s", className);
    printf("[-] Enter New Class Time (00:00): ");
    scanf("%s", classTime);
    printf("[-] Enter New Class Days (day,day,day): ");
    scanf("%s", classDays);

    char query[300];
    sprintf(query, "UPDATE Classes SET ClassName = '%s', ClassTime = '%s', ClassDays = '%s' WHERE ClassID = %d;",
            className, classTime, classDays, classID);
    
        rc = sqlite3_exec(db, query, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("/n[+] Class details updated successfully!\n");
    }

    sqlite3_close(db);
}
void deleteClass(int classID) {
    sqlite3 *db;
    char *errMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);
    if (rc) {
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    char query[256];
    sprintf(query, "DELETE FROM Classes WHERE ClassID = %d;", classID);

    rc = sqlite3_exec(db, query, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "[!] SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    } else {
        printf("\n[+] Class deleted successfully!\n");
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

    printf("[-] Enter Student ID to Update: ");
    scanf("%d", &stid);

    printf("[-] Enter First Name: ");
    scanf("%s", firstname);

    printf("[-] Enter Last Name: ");
    scanf("%s", lastname);

    printf("[-] Enter Date of Birth (YYYY-MM-DD): ");
    scanf("%s", dob);

    printf("[-] Enter Contact Number: ");
    scanf("%s", contnumber);

    printf("[-] Enter Your Email: ");
    scanf("%s", email);

    char data[512];
    sprintf(data, "FirstName='%s', LastName='%s', DateOfBirth='%s', ContactNumber='%s', Email='%s'", firstname, lastname, dob, contnumber, email);

    char query[1024];
    sprintf(query, "UPDATE Students SET %s WHERE StudentID=%d", data, stid);

    if (updateData(query)) {
        printf("[+] Student Details Updated Successfully!\n");
    } else {
        printf("[!] Failed to Update Student Details.\n");
    }

}

void deleteStd(){
    int stid;

    printf("[-] Enter Student ID to Delete: ");
    scanf("%d", &stid);

    char query[512];
    sprintf(query, "DELETE FROM Students WHERE StudentID=%d", stid);

    if (updateData(query)) {
        printf("[+] Student Deleted Successfully!\n");
    } else {
        printf("[!] Failed to Delete Student.\n");
    }
}

void updateTut(){
    int tutid;
    char firstname[100];
    char lastname[100];
    char subject[100];
    char contact[15];
    char email[50];

    printf("[-] Enter Tutor ID to Update: ");
    scanf("%d", &tutid);

    printf("[-] Enter First Name: ");
    scanf("%s", firstname);

    printf("[-] Enter Last Name:");
    scanf("%s", lastname);

    printf("[-] Enter Subject Specialization: ");
    scanf("%s",subject);

    printf("[-] Enter Contact Number: ");
    scanf("%s",contact);

    printf("[-] Enter Your Email: ");
    scanf("%s",email);

    char data[512];
    sprintf(data, "FirstName='%s', LastName='%s', SubjectSpecialization='%s', ContactNumber='%s', Email='%s'", firstname, lastname, subject, contact, email);
 
    char query[1024];
    sprintf(query, "UPDATE Tutors SET %s WHERE TutorID=%d", data, tutid);

    if (updateData(query)) {
        printf("[+] Tutor Details Updated Successfully!\n");
    } else {
        printf("[!] Failed to Update Tutor Details.\n");
    }

}

void deleteTut(){
    int tutid;

    printf("[-] Enter Tutor ID to Delete: ");
    scanf("%d", &tutid);

    char query[512];
    sprintf(query, "DELETE FROM Tutors WHERE TutorID=%d", tutid);

    if (updateData(query)) {
        printf("[+] Teacher Deleted Successfully!\n");
    } else {
        printf("[!] Failed to Delete Teacher.\n");
    }
}

void editClass(){
        int choice,classID;

        //  printf("\n===Edit Class===\n\n");
         printf("\t1. Edit Class Details\n");
         printf("\t2. Display Class\n");
         printf("\t3. Back to Manage Class Menu\n");
         printf("\t0. Back to Main Menu\n");
         printf("======================================\n");
         printf("\n[-] Enter your choice: ");
         scanf("%d",&choice);

         switch (choice){
            case 1:
                printf("[-] Enter Class ID to Edit: ");
            scanf("%d", &classID);
            editClassDetails(classID); 
            break;

            case 2:
                displayclasslist();
                break;
            case 3:
                 manageClass();
                 break;
            case 0:
                authSec();
                // printf("Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice.\n");
                break;
         }
      }

void addClass(){
        int classID,tutorID;
        char className[100],classTime[50],classDays[50];

        printf("[-] Enter Class ID: ");
        scanf("%d", &classID);
        printf("[-] Enter Class Name: ");
        scanf("%s", className);
        printf("[-] Enter Tutor ID: ");
        scanf("%d", &tutorID);
        printf("[-] Enter Class Time (00:00): ");
        scanf("%s", classTime);
        printf("[-] Enter Class Days (day,day,day): ");
        scanf("%s", classDays);

        char data[256];
        sprintf(data,"%d, '%s', %d, '%s', '%s'", classID,className,tutorID,classTime,classDays);

        const char* dataArray[1]={data};
        insertData("Classes", dataArray, 1);

        printf("\n[+] Class Added Successfully!\n");
        
    }

void displayclasslist(){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("test.db", &db);

    if (rc) {
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    const char *sql = "SELECT * FROM Classes;";
    sqlite3_stmt *stmt;

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "[!] SQL error: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    printf("\n");
    printf("=======================================\n");
    printf("=          Class Details              =\n");
    printf("=======================================\n");
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

void manageStudents() {
    int studentChoice;
    do {
        printf("=========== Manage Student ============\n");
        printf("1. Add Student\n");
        printf("2. Update Student\n");
        printf("3. Delete Student\n");
        printf("0. Back to Main Menu\n");
        printf("[-] Enter your choice: ");
        scanf("%d", &studentChoice);

        switch (studentChoice) {
            case 1:
                // Add related function
                stdReg();
                break;
            case 2:
                // Add related function
                updateStd();
                break;
            case 3:
                // Add related function
                deleteStd();
                break;
            case 0:
                printf("[|] Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    } while (studentChoice != 0);
}

void manageTutors() {
    int tutorChoice;
    do {
        printf("=========== Manage   Tutors ===========\n");
        printf("1. Add Tutor\n");
        printf("2. Update Tutor\n");
        printf("3. Delete Tutor\n");
        printf("0. Back to Main Menu\n");
        printf("[-] Enter your choice: ");
        scanf("%d", &tutorChoice);

        switch (tutorChoice) {
            case 1:
                // Add related function
                tutReg();
                break;
            case 2:
                // Add related function
                updateTut();
                break;
            case 3:
                // Add related function
                deleteTut();
                break;
            case 0:
                printf("[|] Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    } while (tutorChoice != 0);
}

void manageClass() {
    int classChoice;
    int classID;
    do {
        printf("============ Manage  Class ============\n");
        printf("1. Add Class\n");
        printf("2. Update Class\n");
        printf("3. Delete Class\n");
        printf("0. Back to Main Menu\n");
        printf("[-] Enter your choice: ");
        scanf("%d", &classChoice);

        switch (classChoice) {
            case 1:
                // Add related function
                addClass();
                break;
            case 2:
                // Add related function
                editClass();
                break;
            case 3:
                // Add related function
                printf("[-] Enter Class ID to Delete: ");
                scanf("%d",&classID);
                deleteClass(classID);
                break;
            case 0:
                printf("[|] Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    } while (classChoice != 0);
}

void manageFee() {
    int feeChoice;
    do {
        printf("============= Manage  Fee =============\n");
        printf("1. Collect Fee\n");
        printf("2. Due Fee\n");
        printf("0. Back to Main Menu\n");
        printf("[-] Enter your choice: ");
        scanf("%d", &feeChoice);

        switch (feeChoice) {
            case 1:
                // Add related function
                collectFee();
                break;
            case 2:
                // Add related function
                dueFee();
                break;
            case 0:
                printf("[|] Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    } while (feeChoice != 0);
}

void manageAttendance() {
    int attendanceChoice;
    do {
        printf("========== Manage  Attendance =========\n"); // not centerd
        printf("==================== ==================\n");
        printf("1. Mark Attendance\n");
        printf("2. Check Attendance\n");
        printf("0. Back to Main Menu\n");
        printf("[-] Enter your choice: ");
        scanf("%d", &attendanceChoice);

        switch (attendanceChoice) {
            case 1:
                // Add related function
                markAttendance();
                break;
            case 2:
                // Add related function
                checkAttendance();
                break;
            case 0:
                printf("[-] Returning to main menu.\n");
                break;
            default:
                printf("[!] Invalid choice. Please try again.\n");
        }
    } while (attendanceChoice != 0);
}

void collectFee() {
    int stid;
    char date[20];
    char dueDate[20];

    printf("[-] Enter Due Date (YYYY-MM-DD): ");
    scanf("%s", dueDate);

    printf("[-] Enter Student ID: ");
    scanf("%d", &stid);

    printf("[-] Enter Payment Date (YYYY-MM-DD): ");
    scanf("%s", date);

    char *query;
    asprintf(&query, "UPDATE Payments SET PaymentDate='%s', Paid='1' WHERE StudentID=%d AND DueDate='%s';", date, stid, dueDate);

    const char *query2 = query;

    if (updateData(query)) {
        printf("\n[+] Fee collected successfully!\n");
    } else {
        printf("\n[!] Failed to collect fee. Please try again.\n");
    }
}

int executeSQL(sqlite3 *db, const char *sql)
{
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

void dueFee()
{
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        // printf("Opened database successfully.\n");
    }

    char dueDate[20];
    printf("[-] Enter Due Date (YYYY-MM-DD): ");
    scanf("%s", dueDate);

    const char *sql = "SELECT Students.StudentID, Students.FirstName FROM Students LEFT JOIN Payments ON Students.StudentID = Payments.StudentID WHERE (Payments.Paid IS NULL OR Payments.Paid = 0) AND Payments.DueDate = ?;";
    
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, dueDate, -1, SQLITE_STATIC);

    printf("[+] Students Who Have Not Paid by %s:\n", dueDate);
    int resultCount = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        resultCount++;
        int studentID = sqlite3_column_int(stmt, 0);
        const unsigned char *firstName = sqlite3_column_text(stmt, 1);
        printf("[*] ID: %d -> First Name: %s\n", studentID, firstName);
    }

    if (resultCount == 0)
    {
        printf("[+] No Students Found Who Have Not Paid by %s.\n", dueDate);
    }

    sqlite3_finalize(stmt);

    sqlite3_close(db);
}

void markAttendance() {
    int enrollmentID;
    char date[20];

    printf("[-] Enter Enrollment ID: ");
    scanf("%d", &enrollmentID);

    printf("[-] Enter date (YYYY-MM-DD): ");
    scanf("%s", date);

    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "[!] Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }
    else
    {
        // printf("Opened database successfully.\n");
    }

    char query[100];
    sprintf(query, "UPDATE Attendance SET IsPresent=1 WHERE EnrollmentID=%d AND AttendanceDate='%s';", enrollmentID, date);

    if (executeSQL(db, query) == SQLITE_OK)
    {
        printf("\n[+] Attendance Marked Successfully!\n");
    }
    else
    {
        printf("\n[!] Failed to Mark Attendance. Please Try Again.\n");
    }

    sqlite3_close(db);
}

void checkAttendance() {
    int enrollmentID;
    char date[20];

    printf("[-] Enter Enrollment ID: ");
    scanf("%d", &enrollmentID);

    printf("[-] Enter Date (YYYY-MM-DD): ");
    scanf("%s", date);

    int isPresent = -1;

    char query1[200];
    sprintf(query1, "SELECT IsPresent FROM Attendance WHERE EnrollmentID=%d AND AttendanceDate='%s';", enrollmentID, date);

    const char *query = query1;

    char **result = fetchData(query, TEXT);
    if (result != NULL) {
        isPresent = atoi(result[0]);
        free(result[0]);
        free(result);
    }

    if (isPresent == 1)
    {
        printf("[+] Student With Enrollment ID %d was Present on %s.\n", enrollmentID, date);
    }
    else if (isPresent == 0)
    {
        printf("[+] Student With Enrollment ID %d was Absent on %s.\n", enrollmentID, date);
    }
    else
    {
        printf("[!] No Attendance Data Found for Student With Enrollment ID %d on %s.\n", enrollmentID, date);
    }
}

void insertEnrollmentData(int stid)
{
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "[!] Can't Open Database: %s\n", sqlite3_errmsg(db));
        return;
    }

    int lastEnrollmentID = 0;
    int classID;
    char enrollmentDay[20];

    const char *getLastEnrollmentIDQuery = "SELECT MAX(EnrollmentID) FROM Enrollments;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, getLastEnrollmentIDQuery, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastEnrollmentID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    printf("[-] Enter Class ID: ");
    scanf("%d", &classID);
    printf("[-] Enter Enrollment Day (YYYY-MM-DD): ");
    scanf("%s", enrollmentDay);

    char insertSQL[200];
    sprintf(insertSQL, "INSERT INTO Enrollments (EnrollmentID, StudentID, ClassID, EnrollmentDate) VALUES (%d, %d, %d, '%s');", lastEnrollmentID + 1, stid, classID, enrollmentDay);
    if (executeSQL(db, insertSQL) == SQLITE_OK)
    {
        printf("=======================================\n");
        printf("[+] Enrollment Data Inserted Successfully.\n");
    }
    else
    {
        fprintf(stderr, "[!] Failed to Insert Enrollment Data.\n");
    }

    sqlite3_close(db);
}

void insertClassData(int tutorid)
{
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "[!] Can't Open Database: %s\n", sqlite3_errmsg(db));
        return;
    }

    int lastClassID = 0;
    char className[100];
    char classTime[20];
    char classDays[40];

    const char *getLastClassIDQuery = "SELECT MAX(ClassID) FROM Classes;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, getLastClassIDQuery, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "[!] Failed to Prepare Statement: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastClassID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    printf("[-] Enter Class Name: ");
    scanf("%s", className);

    printf("[-] Enter Class Time (00:00): ");
    scanf("%s", classTime);

    printf("[-] Enter Class Days (day,day,day): ");
    scanf("%s", classDays);

    char insertSQL[300];
    sprintf(insertSQL, "INSERT INTO Classes (ClassID, ClassName, TutorID, ClassTime, ClassDays) VALUES (%d, '%s', %d, '%s', '%s');", lastClassID + 1, className, tutorid, classTime, classDays);
    if (executeSQL(db, insertSQL) == SQLITE_OK)
    {
        printf("=======================================\n");
        printf("[+] Class Data Inserted Successfully.\n");
    }
    else
    {
        fprintf(stderr, "[!] Failed to Insert Class Data.\n");
    }

    sqlite3_close(db);
}