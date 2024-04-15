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
    // Se a escada estiver parada ou se movendo na mesma direção.
    if (esc_direction == -1 || esc_direction == p->direction) {
        // Se a escada estiver parada ou se a próxima pessoa chegar após o último uso.
        if (last_time <= p->arrival_time) {
            last_time = p->arrival_time + 10; // Começa e atravessa
        } else {
            last_time += 10; // Já estava em movimento, apenas atravessa
        }
    } else {
        // Escada está se movendo na direção oposta.
        // Se a próxima pessoa chegar após a escada ter parado e mudado de direção.
        if (last_time < p->arrival_time) {
            last_time = p->arrival_time; // A escada já estaria parada quando a pessoa chega
        }
        // Adiciona tempo para a escada parar se necessário e para atravessar.
        last_time += 20;
    }
    esc_direction = p->direction; // Atualiza a direção da escada
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    // argc é a contagem de argumentos, argv é o array de argumentos
    if (argc != 2) {
        fprintf(stderr, "input\n", argv[0]);
        return 1;
    }

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    int N;
    fscanf(file, "%d", &N);

    pthread_t threads[N];
    Person people[N];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < N; i++) {
        fscanf(file, "%d %d", &people[i].arrival_time, &people[i].direction);
    }

    fclose(file); // Fechar o arquivo após ler os dados

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

