#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

// Function to execute SQL queries
int executeSQL(sqlite3 *db, const char *sql)
{
    char *errMsg = NULL;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }
    return rc;
}

int main()
{
    sqlite3 *db;
    int rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        printf("Opened database successfully.\n");
    }

    // Retrieve last payment ID from Payments table
    sqlite3_stmt *stmt;
    const char *sql = "SELECT MAX(PaymentID) FROM Payments";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    int lastPaymentID = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastPaymentID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // Prompt user for due date input
    char dueDate[20];
    printf("Enter due date (YYYY-MM-DD): ");
    scanf("%s", dueDate);

    // Retrieve all students from Students table
    sql = "SELECT StudentID FROM Students";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Insert payment data for each student starting from lastPaymentID + 1
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int studentID = sqlite3_column_int(stmt, 0);
        lastPaymentID++;
        char insertSQL[200]; // Increased buffer size to accommodate the due date
        sprintf(insertSQL, "INSERT INTO Payments (PaymentID, StudentID, Paid, PaymentDate, DueDate) VALUES (%d, %d, NULL, NULL, '%s');", lastPaymentID, studentID, dueDate);
        executeSQL(db, insertSQL);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
