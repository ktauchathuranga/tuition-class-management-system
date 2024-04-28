#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"


int main(int argc, char *argv[])
{

    const char *studentsColumnDefinitions[] = {
        "StudentID INT PRIMARY KEY",
        "FirstName TEXT NOT NULL",
        "LastName TEXT NOT NULL",
        "DateOfBirth TEXT",
        "ContactNumber TEXT",
        "Email TEXT"};
    createTable("Students", studentsColumnDefinitions, 6);

    const char *tutorsColumnDefinitions[] = {
        "TutorID INT PRIMARY KEY",
        "FirstName TEXT NOT NULL",
        "LastName TEXT NOT NULL",
        "SubjectSpecialization TEXT",
        "ContactNumber TEXT",
        "Email TEXT"};
    createTable("Tutors", tutorsColumnDefinitions, 6);

    const char *classesColumnDefinitions[] = {
        "ClassID INT PRIMARY KEY",
        "ClassName TEXT NOT NULL",
        "TutorID INT",
        "ClassTime TEXT",
        "ClassDays TEXT",
        "FOREIGN KEY(TutorID) REFERENCES Tutors(TutorID)"};
    createTable("Classes", classesColumnDefinitions, 6);

    const char *enrollmentsColumnDefinitions[] = {
        "EnrollmentID INT PRIMARY KEY",
        "StudentID INT",
        "ClassID INT",
        "EnrollmentDate TEXT",
        "FOREIGN KEY(StudentID) REFERENCES Students(StudentID)",
        "FOREIGN KEY(ClassID) REFERENCES Classes(ClassID)"};
    createTable("Enrollments", enrollmentsColumnDefinitions, 6);

    const char *paymentsColumnDefinitions[] = {
        "PaymentID INT PRIMARY KEY",
        "StudentID INT",
        "Amount REAL",
        "PaymentDate TEXT",
        "FOREIGN KEY(StudentID) REFERENCES Students(StudentID)"};
    createTable("Payments", paymentsColumnDefinitions, 5);

    return 0;
}
