#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char **lines = NULL;
int countLines = 0;
int capacity = 0;

int ensureCapacity(int need) {
    if (capacity >= need) return 1;
    int newcap = capacity == 0 ? 4 : capacity * 2;
    while (newcap < need) newcap *= 2;
    char **tmp = realloc(lines, newcap * sizeof(char*));
    if (!tmp) return 0;
    lines = tmp;
    capacity = newcap;
    return 1;
}

int insertLine(int index, const char *text) {
    if (index < 0 || index > countLines) return 0;
    if (!ensureCapacity(countLines + 1)) return 0;
    memmove(&lines[index+1], &lines[index], (countLines - index) * sizeof(char*));
    lines[index] = malloc(strlen(text) + 1);
    if (!lines[index]) return 0;
    strcpy(lines[index], text);
    countLines++;
    return 1;
}

int deleteLine(int index) {
    if (index < 0 || index >= countLines) return 0;
    free(lines[index]);
    memmove(&lines[index], &lines[index+1], (countLines - index - 1) * sizeof(char*));
    countLines--;
    return 1;
}

void printAllLines() {
    for (int i = 0; i < countLines; i++) {
        printf("%d: %s\n", i+1, lines[i]);
    }
}

void freeAll() {
    for (int i = 0; i < countLines; i++) free(lines[i]);
    free(lines);
    lines = NULL;
    countLines = 0;
    capacity = 0;
}

int shrinkToFit() {
    if (countLines == capacity) return 1;
    char **tmp = realloc(lines, countLines * sizeof(char*));
    if (!tmp && countLines>0) return 0;
    lines = tmp;
    capacity = countLines;
    return 1;
}

int saveToFile(const char *filename) {
    FILE *f = fopen(filename, "w");
    if (!f) return 0;
    for (int i = 0; i < countLines; i++) {
        fprintf(f, "%s\n", lines[i]);
    }
    fclose(f);
    return 1;
}

int loadFromFile(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    freeAll();
    char *line = NULL;
    size_t len = 0;
    ssize_t n;
    while ((n = getline(&line, &len, f)) != -1) {
        if (n>0 && line[n-1]=='\n') line[n-1]=0;
        insertLine(countLines, line);
    }
    free(line);
    fclose(f);
    return 1;
}

int main() {
    char *buf = NULL;
    size_t bufsize = 0;
    while (1) {
        printf("editor> ");
        if (getline(&buf, &bufsize, stdin) == -1) break;
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, "quit") == 0) break;
        if (strncmp(buf, "insert ", 7) == 0) {
            int idx;
            char text[10000];
            if (sscanf(buf+7, "%d %[^\n]", &idx, text) >= 1) {
                insertLine(idx-1, text);
            }
        } else if (strncmp(buf, "delete ", 7) == 0) {
            int idx;
            if (sscanf(buf+7, "%d", &idx) == 1) deleteLine(idx-1);
        } else if (strcmp(buf, "print") == 0) {
            printAllLines();
        } else if (strncmp(buf, "save ", 5) == 0) {
            char fname[256];
            if (sscanf(buf+5, "%255s", fname) == 1) saveToFile(fname);
        } else if (strncmp(buf, "load ", 5) == 0) {
            char fname[256];
            if (sscanf(buf+5, "%255s", fname) == 1) loadFromFile(fname);
        } else if (strcmp(buf, "shrink") == 0) {
            shrinkToFit();
        } else {
            insertLine(countLines, buf);
        }
    }
    free(buf);
    freeAll();
    return 0;
}
