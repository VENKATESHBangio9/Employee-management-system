#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Employee {
    int id;
    char name[50];
    int age;
    float salary;
};

void addEmployee();
void listEmployees();
void searchEmployee();
void updateEmployee();
void deleteEmployee();

int main() {
    int choice;
    while(1) {
        printf("\n--- Employee Management System ---\n");
        printf("1. Add Employee\n");
        printf("2. List Employees\n");
        printf("3. Search Employee\n");
        printf("4. Update Employee\n");
        printf("5. Delete Employee\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addEmployee(); break;
            case 2: listEmployees(); break;
            case 3: searchEmployee(); break;
            case 4: updateEmployee(); break;
            case 5: deleteEmployee(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addEmployee() {
    FILE *fp = fopen("employees.dat", "ab");
    struct Employee emp;
    
    printf("Enter Employee ID: ");
    scanf("%d", &emp.id);
    printf("Enter Name: ");
    scanf(" %[^\n]s", emp.name); // to read string with spaces
    printf("Enter Age: ");
    scanf("%d", &emp.age);
    printf("Enter Salary: ");
    scanf("%f", &emp.salary);
    
    fwrite(&emp, sizeof(emp), 1, fp);
    fclose(fp);
    printf("Employee added successfully!\n");
}

void listEmployees() {
    FILE *fp = fopen("employees.dat", "rb");
    struct Employee emp;
    
    if(fp == NULL) {
        printf("No employee records found.\n");
        return;
    }
    
    printf("\nList of Employees:\n");
    printf("ID\tName\t\tAge\tSalary\n");
    printf("-------------------------------------------------\n");
    
    while(fread(&emp, sizeof(emp), 1, fp)) {
        printf("%d\t%s\t\t%d\t%.2f\n", emp.id, emp.name, emp.age, emp.salary);
    }
    
    fclose(fp);
}

void searchEmployee() {
    int id;
    FILE *fp = fopen("employees.dat", "rb");
    struct Employee emp;
    int found = 0;
    
    if(fp == NULL) {
        printf("No employee records found.\n");
        return;
    }
    
    printf("Enter Employee ID to search: ");
    scanf("%d", &id);
    
    while(fread(&emp, sizeof(emp), 1, fp)) {
        if(emp.id == id) {
            printf("Employee Found:\n");
            printf("ID: %d\nName: %s\nAge: %d\nSalary: %.2f\n", emp.id, emp.name, emp.age, emp.salary);
            found = 1;
            break;
        }
    }
    
    if(!found)
        printf("Employee with ID %d not found.\n", id);
    
    fclose(fp);
}

void updateEmployee() {
    int id;
    FILE *fp = fopen("employees.dat", "rb+");
    struct Employee emp;
    int found = 0;
    
    if(fp == NULL) {
        printf("No employee records found.\n");
        return;
    }
    
    printf("Enter Employee ID to update: ");
    scanf("%d", &id);
    
    while(fread(&emp, sizeof(emp), 1, fp)) {
        if(emp.id == id) {
            printf("Current Details:\n");
            printf("Name: %s\nAge: %d\nSalary: %.2f\n", emp.name, emp.age, emp.salary);
            
            printf("Enter new Name: ");
            scanf(" %[^\n]s", emp.name);
            printf("Enter new Age: ");
            scanf("%d", &emp.age);
            printf("Enter new Salary: ");
            scanf("%f", &emp.salary);
            
            fseek(fp, -sizeof(emp), SEEK_CUR);
            fwrite(&emp, sizeof(emp), 1, fp);
            found = 1;
            printf("Employee updated successfully.\n");
            break;
        }
    }
    
    if(!found)
        printf("Employee with ID %d not found.\n", id);
    
    fclose(fp);
}

void deleteEmployee() {
    int id;
    FILE *fp = fopen("employees.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Employee emp;
    int found = 0;
    
    if(fp == NULL) {
        printf("No employee records found.\n");
        return;
    }
    
    printf("Enter Employee ID to delete: ");
    scanf("%d", &id);
    
    while(fread(&emp, sizeof(emp), 1, fp)) {
        if(emp.id == id) {
            found = 1;
            continue; // skip writing this employee to temp file
        }
        fwrite(&emp, sizeof(emp), 1, temp);
    }
    
    fclose(fp);
    fclose(temp);
    
    if(found) {
        remove("employees.dat");
        rename("temp.dat", "employees.dat");
        printf("Employee deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Employee with ID %d not found.\n", id);
    }
}
