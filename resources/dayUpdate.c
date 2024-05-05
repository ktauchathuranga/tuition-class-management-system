#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

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
    int rc = sqlite3_open("../tcms.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        printf("Opened database successfully.\n");
    }

    // Ask for the date
    char date[20];
    printf("Enter date (YYYY-MM-DD): ");
    scanf("%s", date);

    // Get the last attendance ID
    sqlite3_stmt *stmt;
    const char *sql = "SELECT MAX(AttendanceID) FROM Attendance";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    int lastAttendanceID = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        lastAttendanceID = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);

    // Get enrollment IDs from the Enrollment table
    sql = "SELECT EnrollmentID FROM Enrollments";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return 1;
    }

    // Insert attendance data
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int enrollmentID = sqlite3_column_int(stmt, 0);
        lastAttendanceID++;
        char insertSQL[200];
        sprintf(insertSQL, "INSERT INTO Attendance (AttendanceID, EnrollmentID, AttendanceDate, IsPresent) VALUES (%d, %d, '%s', 0);", lastAttendanceID, enrollmentID, date);
        executeSQL(db, insertSQL);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
