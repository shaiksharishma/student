#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CREDENTIAL_FILE "credentials.txt"
#define STUDENT_FILE "students.txt"

struct student {
    int roll;
    char name[50];
    float marks;
};

char currentUser[20];
char currentRole[20];

void createCredentials();
int loginSystem();
void mainMenu();
void adminMenu();
void userMenu();
void addStudent();
void displayStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    createCredentials();

    if (loginSystem())
        mainMenu();
    else
        printf("Login failed!\n");

    return 0;
}

/* ---------------- CREATE CREDENTIALS ---------------- */
void createCredentials() {
    char choice, user[20], pass[20], role[10];

    printf("Create login credentials? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Enter username: ");
        scanf("%s", user);

        printf("Enter password: ");
        scanf("%s", pass);

        printf("Enter role (admin/user): ");
        scanf("%s", role);

        FILE *fp = fopen(CREDENTIAL_FILE, "w");
        fprintf(fp, "%s %s %s\n", user, pass, role);
        fclose(fp);

        printf("Credentials created!\n");
    }

    FILE *f = fopen(STUDENT_FILE, "r");
    if (!f) {
        f = fopen(STUDENT_FILE, "w");
        fclose(f);
    } else {
        fclose(f);
    }
}

/* ---------------- LOGIN SYSTEM ---------------- */
int loginSystem() {
    char username[20], password[20], fuser[20], fpass[20], frole[20];

    printf("\n===== LOGIN =====\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    FILE *fp = fopen(CREDENTIAL_FILE, "r");
    if (!fp) {
        printf("Credentials missing!\n");
        return 0;
    }

    while (fscanf(fp, "%s %s %s", fuser, fpass, frole) != EOF) {
        if (strcmp(username, fuser) == 0 && strcmp(password, fpass) == 0) {
            strcpy(currentUser, fuser);
            strcpy(currentRole, frole);
            fclose(fp);
            return 1;
        }
    }
    fclose(fp);
    return 0;
}

/* ---------------- MAIN MENU ---------------- */
void mainMenu() {
    if (strcmp(currentRole, "admin") == 0)
        adminMenu();
    else
        userMenu();
}

/* ---------------- ADMIN MENU ---------------- */
void adminMenu() {
    int ch;

    do {
        printf("\n===== ADMIN MENU =====\n");
        printf("1. Add Student\n");
        printf("2. Display Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: addStudent(); break;
            case 2: displayStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* ---------------- USER MENU ---------------- */
void userMenu() {
    int ch;

    do {
        printf("\n===== USER MENU =====\n");
        printf("1. Display Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &ch);

        switch (ch) {
            case 1: displayStudents(); break;
            case 2: searchStudent(); break;
            case 3: return;
            default: printf("Invalid choice!\n");
        }
    } while (1);
}

/* ---------------- ADD STUDENT ---------------- */
void addStudent() {
    struct student st;

    printf("Enter roll number: ");
    scanf("%d", &st.roll);

    printf("Enter full name: ");
    getchar();
    fgets(st.name, sizeof(st.name), stdin);
    st.name[strcspn(st.name, "\n")] = 0;

    printf("Enter marks: ");
    scanf("%f", &st.marks);

    FILE *fp = fopen(STUDENT_FILE, "a");
    fprintf(fp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    fclose(fp);

    printf("Student added!\n");
}

/* ---------------- DISPLAY STUDENTS ---------------- */
void displayStudents() {
    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");

    printf("\nRoll\tName\t\tMarks\n");
    printf("--------------------------------------\n");

    while (fscanf(fp, "%d|%[^|]|%f", &st.roll, st.name, &st.marks) != EOF)
        printf("%d\t%-15s %.2f\n", st.roll, st.name, st.marks);

    fclose(fp);
}

/* ---------------- SEARCH STUDENT ---------------- */
void searchStudent() {
    int roll, found = 0;
    printf("Enter roll: ");
    scanf("%d", &roll);

    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");

    while (fscanf(fp, "%d|%[^|]|%f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            printf("Found: %d  %s  %.2f\n", st.roll, st.name, st.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf("Not found!\n");

    fclose(fp);
}

/* ---------------- UPDATE STUDENT ---------------- */
void updateStudent() {
    int roll, found = 0;
    printf("Enter roll to update: ");
    scanf("%d", &roll);

    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d|%[^|]|%f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            found = 1;

            printf("New name: ");
            getchar();
            fgets(st.name, sizeof(st.name), stdin);
            st.name[strcspn(st.name, "\n")] = 0;

            printf("New marks: ");
            scanf("%f", &st.marks);
        }
        fprintf(tmp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Updated!\n");
    } else {
        remove("temp.txt");
        printf("Roll not found!\n");
    }
}

/* ---------------- DELETE STUDENT ---------------- */
void deleteStudent() {
    int roll, found = 0;
    printf("Enter roll to delete: ");
    scanf("%d", &roll);

    struct student st;
    FILE *fp = fopen(STUDENT_FILE, "r");
    FILE *tmp = fopen("temp.txt", "w");

    while (fscanf(fp, "%d|%[^|]|%f", &st.roll, st.name, &st.marks) != EOF) {
        if (st.roll == roll) {
            found = 1;
            continue;
        }
        fprintf(tmp, "%d|%s|%.2f\n", st.roll, st.name, st.marks);
    }

    fclose(fp);
    fclose(tmp);

    if (found) {
        remove(STUDENT_FILE);
        rename("temp.txt", STUDENT_FILE);
        printf("Deleted!\n");
    } else {
        remove("temp.txt");
        printf("Roll not found!\n");
    }
}
