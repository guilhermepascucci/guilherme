#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// Definindo a estrutura para armazenar os dados de cada pessoa
typedef struct {
    int arrival_time;
    int direction;
} Person;

int last_time = 0;
int esc_direction = -1; // Inicialmente a escada está parada.
pthread_mutex_t lock; // Mutex para controle de acesso à variável last_time

// Função que cada thread vai executar
void *use_escalator(void *arg) {
    Person *p = (Person *)arg;

    pthread_mutex_lock(&lock);
    if (esc_direction == -1 || esc_direction == p->direction) {
        if (last_time < p->arrival_time) {
            last_time = p->arrival_time;
        }
        last_time += 10;
        esc_direction = p->direction;
    } else {
        if (last_time < p->arrival_time) {
            last_time = p->arrival_time;
        } else {
            last_time += 10; // Espera a escada parar
        }
        last_time += 10; // Tempo para atravessar
        esc_direction = p->direction;
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main() {
    int N;
    scanf("%d", &N);

    pthread_t threads[N];
    Person people[N];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < N; i++) {
        scanf("%d %d", &people[i].arrival_time, &people[i].direction);
    }

    // Criar uma thread para cada pessoa
    for (int i = 0; i < N; i++) {
        pthread_create(&threads[i], NULL, use_escalator, (void *)&people[i]);
    }

    // Esperar todas as threads terminarem
    for (int i = 0; i < N; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", last_time);

    pthread_mutex_destroy(&lock);
    return 0;
}

