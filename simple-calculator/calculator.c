#include <stdio.h>

int main()
{
    int num1, num2, choice;
    int result;

    printf("This is a Simple Calculator:\n");

    do {
        printf("\n1) Add\n");
        printf("2) Subtract\n");
        printf("3) Multiply\n");
        printf("4) Divide\n");
        printf("0) Quit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 0) {
            printf("Bye!\n");
        }
        else if (choice >= 1 && choice <= 4) {
            printf("Enter Two Numbers: ");
            scanf("%d %d", &num1, &num2);

            if (choice == 1) {
                result = num1 + num2;
                printf("The Result of Add: %d\n", result);
            }
            else if (choice == 2) {
                result = num1 - num2;
                printf("The Result of Subtract: %d\n", result);
            }
            else if (choice == 3) {
                result = num1 * num2;
                printf("The Result of Multiply: %d\n", result);
            }
            else if (choice == 4) {
                if (num2 == 0) {
                    printf("Can't Divide\n");
                }
                else {
                    result = num1 / num2;
                    printf("The Result of Divide: %d\n", result);
                }
            }
        }
        else {
            printf("Invalid choice\n");
        }

    } while (choice != 0);

    return 0;
}
