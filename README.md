# TCM System
***

#### 🤖 To compile:
```
gcc "replce-this-with-your-compiling-file-name" func.c -o main -lcrypto -lsqlite3
```

> [!NOTE]  
> - You need, ```openssl``` library and ```sqlite3``` library.
> - If you still having trouble, you can open a codespace for the repo by ```Code -> Codespaces -> Create codespace on main```
> - `createdb.c` is for create the db tables.
> - `addsampledata.c` to add sample data to the tabels.
> - `test.c` is for understand how the function sql works, with **examples**.

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
    - reg std
    - reg tea
    - browse std
      - class
        - display all std
    - search std by id
      - display details
      - update
      - delete
    - fee manage
      - collect fee (by id)
      - due fee std list
    - status (all basic summary)
  
#### 📜 More details:

`func.c` holds all major functions; if you add a new function, don't forget to add a function prototype to the `func.h` file as well.
In `main.c` we just use basic logic to call the functions so it doesn't get too crowded. And by doing that `main.c` will be much easier to read and understand the basic logic structure behind it.

#### 🔗 Usefull links,

- [sqlite and C](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)
- [CONTRIBUTION GUIDE](.github/CONTRIBUTING.md)