#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main() {
    int pipefds[2];
    char buffer[30];
    
    if (pipe(pipefds) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        // Processo filho
        char *message = "Olá do Filho!\n";
        write(pipefds[1], message, strlen(message) + 1); // +1 para incluir o caractere nulo
        close(pipefds[1]);
        exit(0); // É uma boa prática o filho chamar exit após terminar
    } else {
        // Processo pai
        wait(NULL); // Espera o processo filho terminar
        read(pipefds[0], buffer, sizeof(buffer));
        printf("Pai recebeu: %s", buffer);
        close(pipefds[0]);
    }

    return 0;
}
