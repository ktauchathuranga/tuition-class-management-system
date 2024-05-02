#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main(int argc, char* argv[]) {
    int loginAtt = 0;
    while (loginAtt < 3)
    {
        if (login())
        {
            authSec();
            break;
        } else {
            printf("Login Failed\n");
            loginAtt++;
        }
    }
    
    if (loginAtt == 3) {
        printf("Failed To Login After 3 Attempts. Please Try Again Later.\n");
    }
    
    return 0;
}
