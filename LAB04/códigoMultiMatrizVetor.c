#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 6
#define N 3

int A[M][N]; 
int x[N];
int y[M];

typedef struct {
    int row;
} thread_data;

void *multiply(void *arg) {
    thread_data *data = (thread_data *)arg;
    int row = data->row;

    y[row] = 0;
    for (int j = 0; j < N; j++) {
        y[row] += A[row][j] * x[j];
    }

    pthread_exit(0);
}

int main() {
    pthread_t threads[M];
    thread_data data[M];

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = 2;
        }
    }

    for (int i = 0; i < N; i++) {
        x[i] = 3;
    }

    for (int i = 0; i < M; i++) {
        data[i].row = i;
        if (pthread_create(&threads[i], NULL, multiply, (void *)&data[i])) {
            fprintf(stderr, "Erro ao criar a thread\n");
            return 1;
        }
    }

    for (int i = 0; i < M; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < M; i++) {
        printf("%d ", y[i]);
    }
    printf("\n");

    return 0;
}

