// This assignment will show my knowledge on Dynamic Memory Allocation and Common Bugs.
// Olme Matias
// CSC362-02

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    int n = 0; // Size of the array
    int *arr = (int *)calloc(n, sizeof(int)); // Allocates memory for 'n' integers

    if (arr == NULL) {
        printf("Memory allocation failed\n");
        return 1;  // Exit the program if allocation failed
    }

    char buffer[50]; // Buffer to hold user input
    printf("Enter desired number or type 'done' to stop!\n");
    while (1) {
        printf("Enter a number: ");
        scanf("%49s", buffer); // Read user input into the buffer

        if (strcmp(buffer, "done") == 0) {
            break; // Exit the loop if the user enters 0
        }

        n++; // Increment the size for the next number
        arr = (int *)realloc(arr, n * sizeof(int)); // Reallocate memory for 'n' integers
        int num = atoi(buffer); // Convert the input string to an integer
        arr[n - 1] = num; // Store the number in the array
    }

    // Check if the memory reallocation was successful
    if (arr == NULL) {
        printf("Memory reallocation failed\n");
        return 1;  // Exit the program if reallocation failed
    }

    // Check if any numbers were entered
    if (n == 0) {
        printf("No numbers were entered.\n");
        free(arr);
        return 0;
    }

    // Printing the array in reverse order
    printf("Array (input numbers) in Reverse Order:\n");
    for (int i = (n - 1); 0 <= i; i--) {
        printf("arr[%d] = %d\n", i, arr[i]);
    }

    // Freeing the dynamically allocated memory
    free(arr);
    return 0;
}
