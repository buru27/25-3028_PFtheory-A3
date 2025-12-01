#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

typedef struct {
    int id;
    int popularity;
    long lastAccess;
    int used;
} Book;

int findIndexById(Book *shelf, int cap, int id) {
    for (int i = 0; i < cap; i++) if (shelf[i].used && shelf[i].id == id) return i;
    return -1;
}

int findEmptyIndex(Book *shelf, int cap) {
    for (int i = 0; i < cap; i++) if (!shelf[i].used) return i;
    return -1;
}

int findLRUIndex(Book *shelf, int cap) {
    long min = LONG_MAX;
    int idx = -1;
    for (int i = 0; i < cap; i++) {
        if (shelf[i].used && shelf[i].lastAccess < min) {
            min = shelf[i].lastAccess;
            idx = i;
        }
    }
    return idx;
}

int main() {
    int capacity, Q;
    if (scanf("%d %d", &capacity, &Q) != 2) return 0;
    Book *shelf = malloc(sizeof(Book) * capacity);
    for (int i = 0; i < capacity; i++) {
        shelf[i].used = 0;
        shelf[i].lastAccess = 0;
    }
    long timeCounter = 1;
    for (int i = 0; i < Q; i++) {
        char op[10];
        scanf("%s", op);
        if (strcmp(op, "ADD") == 0) {
            int x, y;
            scanf("%d %d", &x, &y);
            int idx = findIndexById(shelf, capacity, x);
            if (idx != -1) {
                shelf[idx].popularity = y;
                shelf[idx].lastAccess = timeCounter++;
            } else {
                int empty = findEmptyIndex(shelf, capacity);
                if (empty != -1) {
                    shelf[empty].used = 1;
                    shelf[empty].id = x;
                    shelf[empty].popularity = y;
                    shelf[empty].lastAccess = timeCounter++;
                } else {
                    int lru = findLRUIndex(shelf, capacity);
                    if (lru != -1) {
                        shelf[lru].id = x;
                        shelf[lru].popularity = y;
                        shelf[lru].lastAccess = timeCounter++;
                        shelf[lru].used = 1;
                    }
                }
            }
        } else if (strcmp(op, "ACCESS") == 0) {
            int x;
            scanf("%d", &x);
            int idx = findIndexById(shelf, capacity, x);
            if (idx == -1) {
                printf("-1\n");
            } else {
                printf("%d\n", shelf[idx].popularity);
                shelf[idx].lastAccess = timeCounter++;
            }
        }
    }
    free(shelf);
    return 0;
}
