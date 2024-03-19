#define _GNU_SOURCE
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <sched.h>
#include <stdio.h>
#include <string.h>

#define FIBER_STACK 1024*64

struct context_info {
    char message[128]; 
};

int threadFunction(void* argument) {
    struct context_info* context = (struct context_info*) argument;
    printf("Thread: lendo mensagem: '%s'\n", context->message);
    strcpy(context->message, "Mensagem modificada pela thread");
    printf("Thread: modificando mensagem e saindo\n");
    return 0;
}

int main() {
    void* stack;
    pid_t pid;
    struct context_info* context;

    stack = malloc(FIBER_STACK);
    if (stack == 0) {
        perror("malloc: could not allocate stack");
        exit(1);
    }

    context = malloc(sizeof(struct context_info));
    if (context == 0) {
        perror("malloc: could not allocate context");
        free(stack);
        exit(1);
    }
    strcpy(context->message, "Mensagem original do pai");

    printf("Criando thread\n");
    
    pid = clone(&threadFunction, (char*)stack + FIBER_STACK, SIGCHLD | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_VM, context);
    if (pid == -1) {
        perror("clone");
        free(stack);
        free(context);
        exit(2);
    }

    pid = waitpid(pid, NULL, 0);
    if (pid == -1) {
        perror("waitpid");
        free(stack);
        free(context);
        exit(3);
    }

    printf("Thread retornou e pilha liberada. Mensagem modificada: '%s'\n", context->message);

    free(stack);
    free(context);

    return 0;
}

