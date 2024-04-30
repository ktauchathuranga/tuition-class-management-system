void manageClasses() {
    int classChoice;
    do {
        printf("\n=== Manage Classes ===\n");
        printf("1. Add Class\n");
        printf("2. Display Classes\n");
        printf("3. Edit Class\n");
        printf("0. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &classChoice);

        switch (classChoice) {
            case 1:
                // Add class function
                break;
            case 2:
                // Display classes function
                break;
            case 3:
                // Edit class function
                break;
            case 0:
                printf("Returning to main menu.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (classChoice != 0);
}

void displayMenu() {
    // these are just place holders, TO BE CHANGED!
    printf("\n=== Tuition Class Management System ===\n");
    printf("1. Add Student\n");
    printf("2. Display Students\n");
    printf("3. Manage Classes\n");
    printf("0. Exit\n");
}

void authSec() {
    int choice;

    do {
        displayMenu();
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                stdReg();
                // related function
                break;
            case 2:
                // related function
                break;
            case 3:
                manageClasses();  // Call the new function here
                break;
            case 0:
                printf("Exiting program.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);
}
