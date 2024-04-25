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
            printf("login failed\n");
            loginAtt++;
        }
    }
    
    if (loginAtt == 3) {
        printf("Failed to login after 3 attempts. Please try again later.\n");
    }
    
    return 0;
}
