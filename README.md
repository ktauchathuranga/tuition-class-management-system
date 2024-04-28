# TCM System
***

#### To compile:
```
gcc main.c func.c -o main -lcrypto -lsqlite3
```

> [!NOTE]  
> - You need, ```openssl``` library and ```sqlite3``` library.
> - If you still having trouble, you can open a codespace for the repo by ```Code -> Codespaces -> Create codespace on "branch-name"```

#### To run:
- Windows
```
main
```
- Linux
```
./main
```

#### Structure

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

#### Usefull links,

- [sqlite and C](https://www.tutorialspoint.com/sqlite/sqlite_c_cpp.htm)
- [CONTRIBUTION GUIDE](.github/CONTRIBUTING.md)