#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char* argv[]) {
    int loginAtt = 0;
    char* loadingSymbols = "-\\|/";
    int numSymbols = 4;
    const char* version = "v2.1.1";

    printf("\n");
    printf("========================================\n");
    printf("=       Tution Class Management        =\n");
    printf("=            Welcome Screen            =\n");
    printf("=                              %s  =\n", version); // make it center if character count is increased
    printf("========================================\n");
    printf("\nWelcome to the Tution Class Management System!\n");

    printf("\n========================================\n");

    printf("\nStarting...");
    fflush(stdout);

    for (int i = 0; i < 25; i++) { // 5000 milliseconds / 200 milliseconds = 25 iterations
        printf("\b%c", loadingSymbols[i % numSymbols]);
        fflush(stdout);
        #ifdef _WIN32
            Sleep(200);
        #else
            usleep(200000);
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
