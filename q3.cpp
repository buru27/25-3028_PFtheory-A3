#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    char designation[100];
    double salary;
} Employee;

void inputEmployees(Employee *arr, int n) {
    for (int i = 0; i < n; i++) {
        printf("Employee %d ID: ", i+1);
        scanf("%d", &arr[i].id);
        getchar();
        printf("Employee %d Name: ", i+1);
        fgets(arr[i].name, 100, stdin);
        arr[i].name[strcspn(arr[i].name, "\n")] = '\0';
        printf("Employee %d Designation: ", i+1);
        fgets(arr[i].designation, 100, stdin);
        arr[i].designation[strcspn(arr[i].designation, "\n")] = '\0';
        printf("Employee %d Salary: ", i+1);
        scanf("%lf", &arr[i].salary);
        getchar();
    }
}

void displayEmployees(Employee *arr, int n) {
    printf("ID\tName\tDesignation\tSalary\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t%s\t%.2f\n", arr[i].id, arr[i].name, arr[i].designation, arr[i].salary);
    }
}

void findHighestSalary(Employee *arr, int n) {
    if (n == 0) return;
    int idx = 0;
    for (int i = 1; i < n; i++) {
        if (arr[i].salary > arr[idx].salary) idx = i;
    }
    printf("Highest salary:\n");
    printf("%d\t%s\t%s\t%.2f\n", arr[idx].id, arr[idx].name, arr[idx].designation, arr[idx].salary);
}

void searchEmployee(Employee *arr, int n) {
    int choice;
    printf("Search by 1) ID 2) Name : ");
    scanf("%d", &choice);
    getchar();
    if (choice == 1) {
        int id;
        printf("Enter ID: ");
        scanf("%d", &id);
        for (int i = 0; i < n; i++) {
            if (arr[i].id == id) {
                printf("%d\t%s\t%s\t%.2f\n", arr[i].id, arr[i].name, arr[i].designation, arr[i].salary);
                return;
            }
        }
        printf("Not found\n");
    } else if (choice == 2) {
        char name[100];
        printf("Enter Name: ");
        fgets(name, 100, stdin);
        name[strcspn(name, "\n")] = '\0';
        for (int i = 0; i < n; i++) {
            if (strcmp(arr[i].name, name) == 0) {
                printf("%d\t%s\t%s\t%.2f\n", arr[i].id, arr[i].name, arr[i].designation, arr[i].salary);
                return;
            }
        }
        printf("Not found\n");
    } else {
        printf("Invalid choice\n");
    }
}

void giveBonusBelowThreshold(Employee *arr, int n, double threshold) {
    for (int i = 0; i < n; i++) {
        if (arr[i].salary < threshold) {
            arr[i].salary = arr[i].salary * 1.10;
        }
    }
}

int main() {
    int n;
    printf("How many employees? ");
    scanf("%d", &n);
    getchar();
    Employee *arr = malloc(sizeof(Employee) * n);
    inputEmployees(arr, n);
    printf("\nAll employees:\n");
    displayEmployees(arr, n);
    printf("\n");
    findHighestSalary(arr, n);
    printf("\nSearch an employee:\n");
    searchEmployee(arr, n);
    printf("\nGiving 10%% bonus to those below 50000...\n");
    giveBonusBelowThreshold(arr, n, 50000.0);
    printf("After bonus:\n");
    displayEmployees(arr, n);
    free(arr);
    return 0;
}
