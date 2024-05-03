# TCM System
***

### 🤖 To compile:

- If your code uses `func.c`:
```
gcc <your-compiling-file-name> func.c -o main -lcrypto -lsqlite3
```
- If your code **DONT** uses `func.c`:
```
gcc <your-compiling-file-name> -o main -lcrypto -lsqlite3
```

> [!NOTE]  
> - You need, `openssl` library and `sqlite3` library.
> - If you still having trouble, you can open a codespace for the repo by ```Code -> Codespaces -> Create codespace on main```
> - `resources/testDB.c` is for understand how the function sql works, with **examples**.
> - `resources/testMen.c` is for understand how the menu navigation works, with **examples**
> - Default username and the password is `admin`
> - If you want to add test file but do not want to uplaod to the GitHub, just add your file name to `.gitignore` file.

> [!IMPORTANT]  
> - Hence, the repo does **NOT** have db included; create db and add sample data by running,
> - If you are in linux `./RUNTHIS`
> - If you are in windows `RUNTHIS.exe`
> - `MONTHUPDATE` is for update the the `Payments` table for every month.
> - `DAYUPDATE` is for update the `Attendance` table for every day.
### 🏃‍♂️ To run:
- Windows
```
main
```
- Linux
```
./main
```

### 🏢 Structure

- LOGIN
    - Manage Students
      - Add Student
      - Update Student
      - Delete Student
    - Manage Tutotrs
      - Add Tutor
      - Update Tutor
      - Delete Tutor
    - Manage Classes
      - Add Class
      - Update Class
      - Delete Class
    - Browse Students
      - Class List
        - Display All Students
    - Search Student (by ID)
      - Display Details
    - Fee Manage
      - Collect Fee (by ID)
      - Due Fee
    - Attendance
      - Mark Attendance
      - Check Attendance
    - Status (basic summary)
  
### 📜 More details:

`func.c` holds all major functions; if you add a new function, don't forget to add a function prototype to the `func.h` file as well.
In `main.c` we just use basic logic to call the functions so it doesn't get too crowded. And by doing that `main.c` will be much easier to read and understand the basic logic structure behind it.

> [!NOTE]  
> If you see any bugs or things that might be improved, please open an issue.

***

# SQL Table Structure

This repository contains the SQL table structure for a Tution Class Management System. The database consists of six tables: `Students`, `Tutors`, `Classes`, `Enrollments`, `Payments`, and `Attendance`.

## Table Definitions

### Students

The `Students` table stores information about the students.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| StudentID | INT | PRIMARY KEY |
| FirstName | TEXT | NOT NULL |
| LastName | TEXT | NOT NULL |
| DateOfBirth | TEXT | |
| ContactNumber | TEXT | |
| Email | TEXT | |

### Tutors

The `Tutors` table stores information about the tutors.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| TutorID | INT | PRIMARY KEY |
| FirstName | TEXT | NOT NULL |
| LastName | TEXT | NOT NULL |
| SubjectSpecialization | TEXT | |
| ContactNumber | TEXT | |
| Email | TEXT | |

### Classes

The `Classes` table stores information about the classes.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| ClassID | INT | PRIMARY KEY |
| ClassName | TEXT | NOT NULL |
| TutorID | INT | FOREIGN KEY REFERENCES Tutors(TutorID) |
| ClassTime | TEXT | |
| ClassDays | TEXT | |

### Enrollments

The `Enrollments` table stores information about the enrollments.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| EnrollmentID | INT | PRIMARY KEY |
| StudentID | INT | FOREIGN KEY REFERENCES Students(StudentID) |
| ClassID | INT | FOREIGN KEY REFERENCES Classes(ClassID) |
| EnrollmentDate | TEXT | |

### Payments

The `Payments` table stores information about the payments.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| PaymentID | INT | PRIMARY KEY |
| StudentID | INT | FOREIGN KEY REFERENCES Students(StudentID) |
| Paid | INT | |
| PaymentDate | TEXT | |
| DueDate | TEXT | |

### Attendance

The `Attendance` table stores information about the attendance.

| Column Name | Data Type | Constraint |
|-------------|-----------|------------|
| AttendanceID | INT | PRIMARY KEY |
| EnrollmentID | INT | FOREIGN KEY REFERENCES Enrollments(EnrollmentID) |
| AttendanceDate | TEXT | |
| IsPresent | INT | |


### 🔗 Usefull links,

- [sqlite and C](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)
- [CONTRIBUTION GUIDE](.github/CONTRIBUTING.md)

### 📰License

The TCM System is open source and licensed under the [MIT License](.github/LICENSE). Fork, remix, and make it your own! Pull requests and other contributions greatly appreciated.