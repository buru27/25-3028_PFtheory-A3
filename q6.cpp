#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int id;
    char name[100];
    char batch[50];
    char membershipType[10];
    char regDate[11];
    char dob[11];
    char interest[10];
} Student;

Student *db = NULL;
int dbCount = 0;
int dbCap = 0;

int ensureDB(int need) {
    if (dbCap >= need) return 1;
    int ncap = dbCap == 0 ? 8 : dbCap * 2;
    while (ncap < need) ncap *= 2;
    Student *tmp = realloc(db, ncap * sizeof(Student));
    if (!tmp) return 0;
    db = tmp;
    dbCap = ncap;
    return 1;
}

void loadDatabase(const char *filename) {
    FILE *f = fopen(filename, "rb");
    if (!f) return;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    int count = size / sizeof(Student);
    ensureDB(count);
    fread(db, sizeof(Student), count, f);
    dbCount = count;
    fclose(f);
    printf("Loaded %d records\n", dbCount);
}

void saveDatabase(const char *filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) { printf("Cannot open file for writing\n"); return; }
    fwrite(db, sizeof(Student), dbCount, f);
    fclose(f);
    printf("Saved %d records\n", dbCount);
}

int findById(int id) {
    for (int i = 0; i < dbCount; i++) if (db[i].id == id) return i;
    return -1;
}

void addStudent(Student s, const char *filename) {
    if (findById(s.id) != -1) { printf("ID exists\n"); return; }
    ensureDB(dbCount + 1);
    db[dbCount++] = s;
    saveDatabase(filename);
}

void updateStudent(int studentID, const char *filename) {
    int idx = findById(studentID);
    if (idx == -1) { printf("Not found\n"); return; }
    printf("Enter new batch: ");
    fgets(db[idx].batch, 50, stdin);
    db[idx].batch[strcspn(db[idx].batch, "\n")] = 0;
    printf("Enter new membership type: ");
    fgets(db[idx].membershipType, 10, stdin);
    db[idx].membershipType[strcspn(db[idx].membershipType, "\n")] = 0;
    saveDatabase(filename);
}

void deleteStudent(int studentID, const char *filename) {
    int idx = findById(studentID);
    if (idx == -1) { printf("Not found\n"); return; }
    for (int i = idx; i < dbCount - 1; i++) db[i] = db[i+1];
    dbCount--;
    saveDatabase(filename);
}

void displayAll() {
    for (int i = 0; i < dbCount; i++) {
        printf("%d %s %s %s %s %s %s\n", db[i].id, db[i].name, db[i].batch, db[i].membershipType, db[i].regDate, db[i].dob, db[i].interest);
    }
}

void generateBatchReport(const char *batch, const char *membership) {
    for (int i = 0; i < dbCount; i++) {
        if (strcmp(db[i].batch, batch) == 0) {
            if (strcmp(membership, "Both") == 0 || strcmp(db[i].interest, membership) == 0 || strcmp(db[i].membershipType, membership) == 0) {
                printf("%d %s %s %s %s %s %s\n", db[i].id, db[i].name, db[i].batch, db[i].membershipType, db[i].regDate, db[i].dob, db[i].interest);
            }
        }
    }
}

int main() {
    const char *filename = "members.dat";
    loadDatabase(filename);
    while (1) {
        printf("1 add 2 update 3 delete 4 view 5 report 6 exit: ");
        int choice;
        if (scanf("%d", &choice) != 1) break;
        getchar();
        if (choice == 1) {
            Student s;
            printf("ID: "); scanf("%d", &s.id); getchar();
            printf("Name: "); fgets(s.name, 100, stdin); s.name[strcspn(s.name, "\n")] = 0;
            printf("Batch (CS/SE/Cyber/AI): "); fgets(s.batch, 50, stdin); s.batch[strcspn(s.batch, "\n")] = 0;
            printf("Membership Type (IEEE/ACM): "); fgets(s.membershipType, 10, stdin); s.membershipType[strcspn(s.membershipType, "\n")] = 0;
            printf("Reg Date YYYY-MM-DD: "); fgets(s.regDate, 11, stdin); s.regDate[strcspn(s.regDate, "\n")] = 0;
            printf("DOB YYYY-MM-DD: "); fgets(s.dob, 11, stdin); s.dob[strcspn(s.dob, "\n")] = 0;
            printf("Interest (IEEE/ACM/Both): "); fgets(s.interest, 10, stdin); s.interest[strcspn(s.interest, "\n")] = 0;
            addStudent(s, filename);
        } else if (choice == 2) {
            int id; printf("Enter ID to update: "); scanf("%d", &id); getchar();
            updateStudent(id, filename);
        } else if (choice == 3) {
            int id; printf("Enter ID to delete: "); scanf("%d", &id); getchar();
            deleteStudent(id, filename);
        } else if (choice == 4) {
            displayAll();
        } else if (choice == 5) {
            char batch[50], membership[10];
            printf("Enter batch: "); fgets(batch, 50, stdin); batch[strcspn(batch, "\n")] = 0;
            printf("Enter membership interest (IEEE/ACM/Both): "); fgets(membership, 10, stdin); membership[strcspn(membership, "\n")] = 0;
            generateBatchReport(batch, membership);
        } else if (choice == 6) {
            saveDatabase(filename);
            break;
        } else {
            printf("Invalid\n");
        }
    }
    free(db);
    return 0;
}
