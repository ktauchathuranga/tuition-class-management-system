#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../func.h"

int main(int argc, char *argv[])
{

    // Add sample data----------------------------------------------------------

    const char *studentsData[] = {
        "1, 'John', 'Doe', '2000-01-01', '1234567890', 'john.doe@example.com'",
        "2, 'Jane', 'Doe', '2001-02-02', '0987654321', 'jane.doe@example.com'",
        "3, 'Sam', 'Smith', '2002-03-03', '1122334455', 'sam.smith@example.com'",
        "4, 'Sara', 'Johnson', '2003-04-04', '2233445566', 'sara.johnson@example.com'",
        "5, 'Mike', 'Brown', '2004-05-05', '3344556677', 'mike.brown@example.com'",
        "6, 'Emily', 'Davis', '2005-06-06', '4455667788', 'emily.davis@example.com'",
        "7, 'Jake', 'Miller', '2006-07-07', '5566778899', 'jake.miller@example.com'",
        "8, 'Linda', 'Wilson', '2007-08-08', '6677889900', 'linda.wilson@example.com'",
        "9, 'Harry', 'Moore', '2008-09-09', '7788990011', 'harry.moore@example.com'",
        "10, 'Nancy', 'Taylor', '2009-10-10', '8899001122', 'nancy.taylor@example.com'"};

    for (int i = 0; i < 10; i++)
    {
        insertData("Students", &studentsData[i], 1);
    }

    const char *tutorsData[] = {
        "1, 'Alice', 'Smith', 'Math', '1112223333', 'alice.smith@example.com'",
        "2, 'Bob', 'Johnson', 'Science', '4445556666', 'bob.johnson@example.com'",
        "3, 'Charlie', 'Brown', 'English', '3334445555', 'charlie.brown@example.com'",
        "4, 'Lucy', 'Van Pelt', 'History', '5556667777', 'lucy.vanpelt@example.com'",
        "5, 'Tom', 'Anderson', 'Art', '6667778889', 'tom.anderson@example.com'"};

    for (int i = 0; i < 5; i++)
    {
        insertData("Tutors", &tutorsData[i], 1);
    }

    const char *classesData[] = {
        "1, 'Math 101', 1, '09:00', 'Monday,Wednesday,Friday'",
        "2, 'Science 101', 2, '10:00', 'Tuesday,Thursday'",
        "3, 'English 101', 3, '11:00', 'Monday,Wednesday,Friday'",
        "4, 'History 101', 4, '12:00', 'Tuesday,Thursday'",
        "5, 'Art 101', 5, '13:00', 'Monday,Wednesday,Friday'"};

    for (int i = 0; i < 5; i++)
    {
        insertData("Classes", &classesData[i], 1);
    }

    const char *enrollmentsData[] = {
        "1, 1, 1, '2024-01-01'",
        "2, 2, 2, '2024-01-01'",
        "3, 3, 3, '2024-01-01'",
        "4, 4, 4, '2024-01-01'",
        "5, 5, 1, '2024-01-01'",
        "6, 6, 2, '2024-01-01'",
        "7, 7, 3, '2024-01-01'",
        "8, 8, 4, '2024-01-01'",
        "9, 9, 5, '2024-01-01'",
        "10, 10, 1, '2024-01-01'"};

    for (int i = 0; i < 10; i++)
    {
        insertData("Enrollments", &enrollmentsData[i], 1);
    }

    const char *paymentsData[] = {
        "1, 1, 1, '2024-01-01', '2024-01-10'",
        "2, 2, 0, '', '2024-01-10'",
        "3, 3, 1, '2024-02-01', '2024-01-10'",
        "4, 4, 1, '2024-02-02', '2024-01-10'",
        "5, 5, 1, '2024-03-01', '2024-01-10'",
        "6, 6, 0, '', '2024-01-10'",
        "7, 7, 1, '2024-04-01', '2024-01-10'",
        "8, 8, 0, '', '2024-01-10'",
        "9, 9, 1, '2024-05-01', '2024-01-10'",
        "10, 10, 1, '2024-05-02', '2024-01-10'"};

    for (int i = 0; i < 10; i++)
    {
        insertData("Payments", &paymentsData[i], 1);
    }

    const char *attendanceData[] = {
        "1, 1, '2024-01-01', 1",
        "2, 2, '2024-01-02', 1",
        "3, 3, '2024-02-01', 1",
        "4, 4, '2024-02-02', 0",
        "5, 5, '2024-03-01', 1",
        "6, 6, '2024-03-02', 1",
        "7, 7, '2024-04-01', 1",
        "8, 8, '2024-04-02', 1",
        "9, 9, '2024-05-01', 0",
        "10, 10, '2024-05-02', 1"};

    for (int i = 0; i < 10; i++)
    {
        insertData("Attendance", &attendanceData[i], 1);
    }

    return 0;
}
