#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char* argv[]) {
    int loginAtt = 0;
    const char* version = "v1.0.0";

    printf("\n");
    printf("========================================\n");
    printf("=       Tution Class Management        =\n");
    printf("=            Welcome Screen            =\n");
    printf("=                              %s  =\n", version); // make it center if character count is increased
    printf("========================================\n");
    printf("Welcome to the Tution Class Management System!\n");

    printf("Starting in 5...");
    fflush(stdout);

    for (int i = 5; i > 0; i--) {
        printf("\rStarting in %d... ", i);
        fflush(stdout);
        #ifdef _WIN32
            Sleep(1000);
        #else
            sleep(1);
        #endif
    }

    clearScreen();

    while (loginAtt < 3)
    {
        if (login())
        {
            authSec();
            break;
        } else {
            printf("[!] Login Failed\n");
            loginAtt++;
        }
    }
    
    if (loginAtt == 3) {
        printf("[!] Failed To Login After 3 Attempts. Please Try Again Later.\n");
    }
    
    return 0;
}
