# TCM System
***

#### 🤖 To compile:
```
gcc "replace-this-with-your-compiling-file-name" func.c -o main -lcrypto -lsqlite3
```

> [!NOTE]  
> - You need, `openssl` library and `sqlite3` library.
> - If you still having trouble, you can open a codespace for the repo by ```Code -> Codespaces -> Create codespace on main```
> - `createdb.c` is for create the db tables.
> - `addsampledata.c` to add sample data to the tabels.
> - `testDB.c` is for understand how the function sql works, with **examples**.
> - `testMen.c` is for understand how the menu navigation works, with **examples**
> - Default username and the password is `admin`
> - If you want to add test file but do not want to uplaod to the GitHub, just add your file name to `.gitignore` file.

> [!IMPORTANT]  
> - Hence, the repo does not have db included; create db first by running `createdb.c`, and then add data to the tables by running `addsampledata.c`

#### 🏃‍♂️ To run:
- Windows
```
main
```
- Linux
```
./main
```

#### 🏢 Structure

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
      - Due Fee (optional)
    - Status (basic summary)
  
#### 📜 More details:

`func.c` holds all major functions; if you add a new function, don't forget to add a function prototype to the `func.h` file as well.
In `main.c` we just use basic logic to call the functions so it doesn't get too crowded. And by doing that `main.c` will be much easier to read and understand the basic logic structure behind it.

> [!NOTE]  
> If you see any bugs or things that might be improved, please open an issue.

#### 🔗 Usefull links,

- [sqlite and C](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)
- [CONTRIBUTION GUIDE](.github/CONTRIBUTING.md)