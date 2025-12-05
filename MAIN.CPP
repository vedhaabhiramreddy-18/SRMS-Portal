#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STUDENT_FILE "student.txt"
#define CREDENTIALS_FILE  "credentials.txt"

struct Student {
    int roll;
    char name[50];
    float marks;
};

char currentRole[10];
char currentUser[50];

/* Prototypes */
int loginSystem(void);
void mainMenu(void);

void adminMenu(void);
void staffMenu(void);
void userMenu(void);
void guestMenu(void);

void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);
int  isDuplicateRoll(int);

/* ================= MAIN ================= */
int main(void) {

    printf("\n=========================================\n");
    printf("                SRMS PORTAL                \n");
    printf("=========================================\n");
    printf("   Developed by : SURA VEDHAABHIRAM REDDY\n");
    printf("=========================================\n\n");

    if (loginSystem())
        mainMenu();
    else
        printf("\nLogin Failed! Program Exit\n");

    return 0;
}

/* ================= LOGIN SYSTEM ================= */
int loginSystem(void) {
    char u[20], p[20];
    char fu[20], fp[20], fr[10];

    printf("\n===== LOGIN =====\n");
    printf("Username: ");
    scanf("%19s", u);
    printf("Password: ");
    scanf("%19s", p);

    FILE *f = fopen(CREDENTIALS_FILE, "r");
    if (!f) {
        printf("ERROR: credentials.txt missing!\n");
        return 0;
    }

    while (fscanf(f, "%19s %19s %9s", fu, fp, fr) == 3) {
        if (strcmp(u, fu) == 0 && strcmp(p, fp) == 0) {
            strcpy(currentUser, fu);
            strcpy(currentRole, fr);
            fclose(f);

            printf("\nLogin Successful!\n");
            printf("Welcome %s [%s]\n", currentUser, currentRole);
            return 1;
        }
    }

    fclose(f);
    printf("\nInvalid Username or Password\n");
    return 0;
}

/* ================= MAIN MENU ================= */
void mainMenu(void) {

    if (strcmp(currentRole, "ADMIN") == 0) {
        adminMenu();
    }
    else if (strcmp(currentRole, "STAFF") == 0) {
        staffMenu();
    }
    else if (strcmp(currentRole, "USER") == 0) {
        userMenu();
    }
    else if (strcmp(currentRole, "GUEST") == 0) {
        guestMenu();
    }
    else {
        printf("\nERROR: Invalid role detected!\n");
    }
}

/* ================= ADMIN ================= */
void adminMenu(void) {
    int ch;
    do {
        printf("\n--- ADMIN MENU ---\n");
        printf("1. Add Student\n");
        printf("2. Display All\n");
        printf("3. Search\n");
        printf("4. Update\n");
        printf("5. Delete\n");
        printf("6. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: printf("Logout Successful!\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (ch != 6);
}

/* ================= STAFF ================= */
void staffMenu(void) {
    int ch;
    do {
        printf("\n--- STAFF MENU ---\n");
        printf("1. Display All\n");
        printf("2. Search\n");
        printf("3. Update\n");
        printf("4. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: updateStudent(); break;
            case 4: printf("Logout Successful!\n"); break;
            default: printf("Invalid choice!\n");
        }

    } while (ch != 4);
}

/* ================= USER ================= */
void userMenu(void) {
    int ch;
    struct Student st;
    FILE *fp;
    int roll, found;

    do {
        printf("\n--- USER MENU ---\n");
        printf("1. View My Record\n");
        printf("2. Logout\n");
        printf("Choice: ");
        scanf("%d", &ch);

        if (ch == 1) {

            printf("Enter Your Roll Number: ");
            scanf("%d", &roll);

            fp = fopen(STUDENT_FILE, "r");
            if (!fp) {
                printf("No student records found!\n");
                continue;
            }

            found = 0;
            while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {

                if (st.roll == roll) {
                    printf("\n--- YOUR DETAILS ---\n");
                    printf("Roll   : %d\n", st.roll);
                    printf("Name   : %s\n", st.name);
                    printf("Marks  : %.2f\n", st.marks);
                    found = 1;
                    break;
                }
            }

            if (!found)
                printf("Record not found!\n");

            fclose(fp);
        }

    } while (ch != 2);
}

/* ================= GUEST ================= */
void guestMenu(void) {

    printf("\n--- GUEST MODE ---\n");
    printf("View only access\n");
    displayStudents();
    printf("\nGuest session completed.\n");
}

/* ================= DUPLICATE CHECK ================= */
int isDuplicateRoll(int roll) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) return 0;

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {
        if (st.roll == roll) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* ================= ADD STUDENT ================= */
void addStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "a");

    if (!fp) {
        printf("ERROR: Cannot open student file!\n");
        return;
    }

    printf("Enter Roll: ");
    scanf("%d", &st.roll);

    if (isDuplicateRoll(st.roll)) {
        printf("ERROR: Roll number already exists!\n");
        fclose(fp);
        return;
    }

    printf("Enter Name: ");
    scanf("%49s", st.name);

    printf("Enter Marks: ");
    scanf("%f", &st.marks);

    fprintf(fp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student added successfully!\n");
}

/* ================= DISPLAY ================= */
void displayStudents(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");

    if (!fp) {
        printf("No student data found!\n");
        return;
    }

    printf("\nRoll\tName\tMarks\n");
    printf("---------------------------\n");

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3)
        printf("%d\t%s\t%.2f\n", st.roll, st.name, st.marks);

    fclose(fp);
}

/* ================= SEARCH ================= */
void searchStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    int roll, found = 0;

    if (!fp) {
        printf("No student file available!\n");
        return;
    }

    printf("Enter Roll Number: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {

        if (st.roll == roll) {
            printf("\nRecord Found!\n");
            printf("Roll  : %d\n", st.roll);
            printf("Name  : %s\n", st.name);
            printf("Marks : %.2f\n", st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Record not found!\n");

    fclose(fp);
}

/* ================= UPDATE ================= */
void updateStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) {
        printf("ERROR: File processing error!\n");
        return;
    }

    printf("Enter Roll Number to Update: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {

        if (st.roll == roll) {
            printf("Enter New Name: ");
            scanf("%49s", st.name);
            printf("Enter New Marks: ");
            scanf("%f", &st.marks);
            found = 1;
        }

        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Updated Successfully!\n");
    else
        printf("Record not found!\n");
}

/* ================= DELETE ================= */
void deleteStudent(void) {
    struct Student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");
    int roll, found = 0;

    if (!fp || !tmp) {
        printf("ERROR: File processing error!\n");
        return;
    }

    printf("Enter Roll Number to Delete: ");
    scanf("%d", &roll);

    while (fscanf(fp, "%d %49s %f", &st.roll, st.name, &st.marks) == 3) {

        if (st.roll == roll) {
            found = 1;
            continue;
        }

        fprintf(tmp, "%d %s %.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    remove(STUDENT_FILE);
    rename("temp.txt", STUDENT_FILE);

    if (found)
        printf("Record Deleted Successfully!\n");
    else
        printf("Record not found!\n");
}

