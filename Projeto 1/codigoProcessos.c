#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

// Estrutura para compartilhar dados entre processos.
typedef struct {
    int last_time;
    int direction;
    int mutex; // Simulação simples de um mutex.
} SharedData;

void use_escalator(SharedData *shared, int arrival_time, int direction) {
    // "Lock" simples, espera até o mutex ser 0.
    while (__sync_lock_test_and_set(&shared->mutex, 1)) {
        usleep(100);
    }

    // Aqui viria a lógica da escada rolante.
    // [...] (similar ao exemplo das threads)

    // "Unlock" do mutex.
    __sync_synchronize(); // Memory barrier.
    shared->mutex = 0;

    exit(0);
}

int main() {
    int N;
    scanf("%d", &N);

    // Criar segmento de memória compartilhada.
    int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    SharedData *shared = (SharedData *)shmat(shmid, NULL, 0);
    shared->last_time = 0;
    shared->direction = -1;
    shared->mutex = 0;

    for (int i = 0; i < N; ++i) {
        int arrival_time, direction;
        scanf("%d %d", &arrival_time, &direction);

        pid_t pid = fork();
        if (pid == 0) { // Processo filho.
            use_escalator(shared, arrival_time, direction);
        } else if (pid < 0) {
            fprintf(stderr, "Fork failed.\n");
            exit(1);
        }
    }

    // Esperar todos os processos filhos terminarem.
    while (wait(NULL) > 0);

    printf("%d\n", shared->last_time);

    // Destruir o segmento de memória compartilhada.
    shmdt(shared);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

