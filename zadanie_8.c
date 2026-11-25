
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>
#include <queue>
#include <chrono>

using namespace std;

int** createMatrix(int n) {
    int** a = new int* [n];

    for (int i = 0; i < n; i++) {
        a[i] = new int[n]();
    }

    return a;
}

void deleteMatrix(int** a, int n) {
    for (int i = 0; i < n; i++) {
        delete[] a[i];
    }

    delete[] a;
}

void generateMatrix(int** a, int n, int p) {
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (rand() % 100 < p) {
                a[i][j] = 1;
                a[j][i] = 1;
            }
        }
    }
}

void printMatrix(int** a, int n) {
    printf("\nМатрица смежности:\n");

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", a[i][j]);
        }

        printf("\n");
    }
}

int bfsMatrix(int** a, int n, int start, bool show) {
    bool* visited = new bool[n]();

    queue<int> q;

    int count = 0;

    if (show) {
        printf("\nОбход в ширину по матрице: ");
    }

    for (int offset = 0; offset < n; offset++) {
        int source = (start + offset) % n;

        if (visited[source]) {
            continue;
        }

        visited[source] = true;
        q.push(source);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            count++;

            if (show) {
                printf("%d ", v + 1);
            }

            for (int i = 0; i < n; i++) {
                if (a[v][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                }
            }
        }
    }

    if (show) {
        printf("\n");
    }

    delete[] visited;

    return count;
}

struct Node {
    int vertex;
    Node* next;
};

Node** createLists(int** a, int n) {
    Node** list = new Node * [n];

    for (int i = 0; i < n; i++) {
        list[i] = NULL;

        for (int j = n - 1; j >= 0; j--) {
            if (a[i][j] == 1) {
                Node* temp = new Node;

                temp->vertex = j;
                temp->next = list[i];

                list[i] = temp;
            }
        }
    }

    return list;
}

void printLists(Node** list, int n) {
    printf("\nСписки смежности:\n");

    for (int i = 0; i < n; i++) {
        printf("%d -> ", i + 1);

        Node* temp = list[i];

        while (temp != NULL) {
            printf("%d ", temp->vertex + 1);
            temp = temp->next;
        }

        printf("\n");
    }
}

void deleteLists(Node** list, int n) {
    for (int i = 0; i < n; i++) {
        Node* temp = list[i];

        while (temp != NULL) {
            Node* next = temp->next;

            delete temp;

            temp = next;
        }
    }

    delete[] list;
}

int bfsLists(Node** list, int n, int start, bool show) {
    bool* visited = new bool[n]();

    queue<int> q;

    int count = 0;

    if (show) {
        printf("\nОбход в ширину по спискам смежности: ");
    }

    for (int offset = 0; offset < n; offset++) {
        int source = (start + offset) % n;

        if (visited[source]) {
            continue;
        }

        visited[source] = true;
        q.push(source);

        while (!q.empty()) {
            int v = q.front();
            q.pop();

            count++;

            if (show) {
                printf("%d ", v + 1);
            }

            Node* temp = list[v];

            while (temp != NULL) {
                int u = temp->vertex;

                if (!visited[u]) {
                    visited[u] = true;
                    q.push(u);
                }

                temp = temp->next;
            }
        }
    }

    if (show) {
        printf("\n");
    }

    delete[] visited;

    return count;
}

struct QueueNode {
    int data;
    QueueNode* next;
};

struct MyQueue {
    QueueNode* head;
    QueueNode* tail;
};

void push(MyQueue& q, int value) {
    QueueNode* temp = new QueueNode;

    temp->data = value;
    temp->next = NULL;

    if (q.tail == NULL) {
        q.head = temp;
        q.tail = temp;
    }
    else {
        q.tail->next = temp;
        q.tail = temp;
    }
}

int pop(MyQueue& q) {
    QueueNode* temp = q.head;

    int value = temp->data;

    q.head = q.head->next;

    if (q.head == NULL) {
        q.tail = NULL;
    }

    delete temp;

    return value;
}

bool queueEmpty(MyQueue& q) {
    return q.head == NULL;
}

int bfsMyQueue(int** a, int n, int start, bool show) {
    bool* visited = new bool[n]();

    MyQueue q;

    q.head = NULL;
    q.tail = NULL;

    int count = 0;

    if (show) {
        printf("\nОбход в ширину с собственной очередью: ");
    }

    for (int offset = 0; offset < n; offset++) {
        int source = (start + offset) % n;

        if (visited[source]) {
            continue;
        }

        visited[source] = true;
        push(q, source);

        while (!queueEmpty(q)) {
            int v = pop(q);

            count++;

            if (show) {
                printf("%d ", v + 1);
            }

            for (int i = 0; i < n; i++) {
                if (a[v][i] == 1 && !visited[i]) {
                    visited[i] = true;
                    push(q, i);
                }
            }
        }
    }

    if (show) {
        printf("\n");
    }

    delete[] visited;

    return count;
}

double timeStandard(int** a, int n, int start) {
    auto begin = chrono::steady_clock::now();

    bfsMatrix(a, n, start, false);

    auto end = chrono::steady_clock::now();

    chrono::duration<double, milli> elapsed = end - begin;

    return elapsed.count();
}

double timeMyQueue(int** a, int n, int start) {
    auto begin = chrono::steady_clock::now();

    bfsMyQueue(a, n, start, false);

    auto end = chrono::steady_clock::now();

    chrono::duration<double, milli> elapsed = end - begin;

    return elapsed.count();
}

double timeSpisokCmezh(Node** list, int n, int start) {
    auto begin = chrono::steady_clock::now();

    bfsLists(list, n, start, false);

    auto end = chrono::steady_clock::now();

    chrono::duration<double, milli> elapsed = end - begin;

    return elapsed.count();
}

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    setlocale(LC_ALL, "Russian");

    srand((unsigned)time(NULL));

    int n, p, start;

    printf("Введите количество вершин: ");

    if (scanf("%d", &n) != 1 || n < 1) {
        printf("Неверное количество вершин\n");
        return 1;
    }

    printf("Введите вероятность появления ребра (от 0 до 100): ");

    if (scanf("%d", &p) != 1 || p < 0 || p > 100) {
        printf("Неверная вероятность\n");
        return 1;
    }

    int** a = createMatrix(n);

    generateMatrix(a, n, p);

    printMatrix(a, n);

    printf("\nВведите начальную вершину (от 1 до %d): ", n);

    if (scanf("%d", &start) != 1 || start < 1 || start > n) {
        printf("Неверная начальная вершина\n");

        deleteMatrix(a, n);

        return 1;
    }

    start--;

    Node** list = createLists(a, n);

    printLists(list, n);

    bfsMatrix(a, n, start, true);

    bfsLists(list, n, start, true);

    bfsMyQueue(a, n, start, true);

    printf("\n\nСРАВНЕНИЕ ВРЕМЕНИ BFS\n");

    double spisokTime =
        timeSpisokCmezh(list, n, start);

    double standardTime =
        timeStandard(a, n, start);

    double myTime =
        timeMyQueue(a, n, start);

    printf("\n%-25s %-20s\n",
        "Способ обхода",
        "Время (мс)");

    printf("%-25s %-20.6f\n",
        "Списки смежности",
        spisokTime);

    printf("%-25s %-20.6f\n",
        "Стандартная очередь",
        standardTime);

    printf("%-25s %-20.6f\n",
        "Собственная очередь",
        myTime);

    deleteLists(list, n);

    deleteMatrix(a, n);

    return 0;
}
