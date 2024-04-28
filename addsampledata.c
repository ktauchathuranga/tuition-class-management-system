#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char *argv[])
{

    const char *studentsData[] = {
        "1, 'John', 'Doe', '2000-01-01', '1234567890', 'john.doe@example.com'",
        "2, 'Jane', 'Doe', '2001-02-02', '0987654321', 'jane.doe@example.com'"};

    for (int i = 0; i < 2; i++)
    {
        insertData("Students", &studentsData[i], 1);
    }

    const char *tutorsData[] = {
        "1, 'Alice', 'Smith', 'Math', '1112223333', 'alice.smith@example.com'",
        "2, 'Bob', 'Johnson', 'Science', '4445556666', 'bob.johnson@example.com'"};

    for (int i = 0; i < 2; i++)
    {
        insertData("Tutors", &tutorsData[i], 1);
    }

    const char *classesData[] = {
        "1, 'Math 101', 1, '09:00', 'Monday,Wednesday,Friday'",
        "2, 'Science 101', 2, '10:00', 'Tuesday,Thursday'"};

    for (int i = 0; i < 2; i++)
    {
        insertData("Classes", &classesData[i], 1);
    }

    const char *enrollmentsData[] = {
        "1, 1, 1, '2024-01-01'",
        "2, 2, 2, '2024-01-02'"};

    for (int i = 0; i < 2; i++)
    {
        insertData("Enrollments", &enrollmentsData[i], 1);
    }

    const char *paymentsData[] = {
        "1, 1, 100.00, '2024-01-01'",
        "2, 2, 200.00, '2024-01-02'"};

    for (int i = 0; i < 2; i++)
    {
        insertData("Payments", &paymentsData[i], 1);
    }

    return 0;
}
