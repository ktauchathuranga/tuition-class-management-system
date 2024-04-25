#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"

int main() {
    int login_attempts = 0;
    while (login_attempts < 3)
    {
        if (login())
        {
            // authSec();
            break;
        } else {
            printf("login failed\n");
            login_attempts++;
        }
    }
    
    if (login_attempts == 3) {
        printf("Failed to login after 3 attempts. Please try again later.\n");
    }
    
    return 0;
}
