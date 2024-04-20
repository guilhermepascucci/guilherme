#include <stdio.h>
#include <pthread.h>
#define MAX_PESSOAS 10000

typedef struct {
    int horarioChegada;
    int direcao;
} Passageiro;

typedef struct {
    Passageiro filaPrimaria[MAX_PESSOAS];
    int tamanhoFilaPrimaria;
    Passageiro filaSecundaria[MAX_PESSOAS];
    int tamanhoFilaSecundaria;
} ControleEscadaRolante;

int tempoUltimaSaida = 0;

void* gerenciarEscadaRolante(void* args) {
    ControleEscadaRolante* controle = (ControleEscadaRolante *) args;
    Passageiro passageiroAtual;

    if (controle->tamanhoFilaPrimaria == 0) {
        passageiroAtual = controle->filaSecundaria[0];
    } else if (controle->tamanhoFilaSecundaria == 0) {
        passageiroAtual = controle->filaPrimaria[0];
    } else {
        passageiroAtual = controle->filaPrimaria[0].horarioChegada < controle->filaSecundaria[0].horarioChegada ? 
                           controle->filaPrimaria[0] : controle->filaSecundaria[0];
    }

    int indicePrimario = 0, indiceSecundario = 0;

    while (indicePrimario < controle->tamanhoFilaPrimaria || indiceSecundario < controle->tamanhoFilaSecundaria) {

        if (passageiroAtual.direcao == 0) {
            if ((indicePrimario < controle->tamanhoFilaPrimaria && (controle->filaPrimaria[indicePrimario].horarioChegada <= tempoUltimaSaida) || (controle->filaPrimaria[indicePrimario].horarioChegada > tempoUltimaSaida && controle->filaPrimaria[indicePrimario].horarioChegada < controle->filaSecundaria[indiceSecundario].horarioChegada)) || indiceSecundario == controle->tamanhoFilaSecundaria) {
                passageiroAtual = controle->filaPrimaria[indicePrimario];               
                ++indicePrimario;   

            } else if (controle->filaPrimaria[indicePrimario].horarioChegada > tempoUltimaSaida || indicePrimario == controle->tamanhoFilaPrimaria) {
                passageiroAtual = controle->filaSecundaria[indiceSecundario];
                if (tempoUltimaSaida > passageiroAtual.horarioChegada) passageiroAtual.horarioChegada = tempoUltimaSaida;
                ++indiceSecundario;

                int i = indiceSecundario;
                while (tempoUltimaSaida > controle->filaSecundaria[i].horarioChegada && i < controle->tamanhoFilaSecundaria) {
                    controle->filaSecundaria[i].horarioChegada = tempoUltimaSaida;
                    ++i;
                }
            } 
                              
        } else if (passageiroAtual.direcao == 1) {
            if ((indiceSecundario < controle->tamanhoFilaSecundaria && controle->filaSecundaria[indiceSecundario].horarioChegada <= tempoUltimaSaida || (controle->filaSecundaria[indiceSecundario].horarioChegada > tempoUltimaSaida && controle->filaSecundaria[indiceSecundario].horarioChegada < controle->filaPrimaria[indicePrimario].horarioChegada)) || indicePrimario == controle->tamanhoFilaPrimaria) {
                passageiroAtual = controle->filaSecundaria[indiceSecundario];      
                ++indiceSecundario;

            } else if (controle->filaSecundaria[indiceSecundario].horarioChegada > tempoUltimaSaida || indiceSecundario == controle->tamanhoFilaSecundaria) {
                passageiroAtual = controle->filaPrimaria[indicePrimario];
                if (tempoUltimaSaida > passageiroAtual.horarioChegada) passageiroAtual.horarioChegada = tempoUltimaSaida;
                ++indicePrimario;

                int i = indicePrimario;
                while (tempoUltimaSaida > controle->filaPrimaria[i].horarioChegada && i < controle->tamanhoFilaPrimaria) {
                    controle->filaPrimaria[i].horarioChegada = tempoUltimaSaida;
                    ++i;
                }                
            }
        }   
        tempoUltimaSaida = passageiroAtual.horarioChegada + 10;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int indiceArquivo;
    printf("Entre com o número do arquivo (1-58): ");
    scanf("%d", &indiceArquivo);

    if (indiceArquivo < 1 || indiceArquivo > 58) {
        fprintf(stderr, "Número do arquivo inválido. Deve ser entre 1 e 58.\n");
        return 1;
    }

    char nomeArquivo[20];
    sprintf(nomeArquivo, "E_%d", indiceArquivo);

    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    int n;
    fscanf(arquivo, "%d", &n);

    Passageiro primeiraLinha[MAX_PESSOAS];
    Passageiro segundaLinha[MAX_PESSOAS];
    int tamanhoPrimeiraLinha = 0, tamanhoSegundaLinha = 0;
    Passageiro individuo;

    for (int i = 0; i < n; ++i) {
        fscanf(arquivo, "%d %d", &individuo.horarioChegada, &individuo.direcao);
        if (individuo.direcao == 0) primeiraLinha[tamanhoPrimeiraLinha++] = individuo;
        else segundaLinha[tamanhoSegundaLinha++] = individuo;
    }

    fclose(arquivo);

    ControleEscadaRolante argumentosEscada;
    for (int i = 0; i < tamanhoPrimeiraLinha; i++) argumentosEscada.filaPrimaria[i] = primeiraLinha[i];
    argumentosEscada.tamanhoFilaPrimaria = tamanhoPrimeiraLinha;
    for (int i = 0; i < tamanhoSegundaLinha; i++) argumentosEscada.filaSecundaria[i] = segundaLinha[i];
    argumentosEscada.tamanhoFilaSecundaria = tamanhoSegundaLinha;

    pthread_t thread_id;
    pthread_create(&thread_id, NULL, gerenciarEscadaRolante, (void *) &argumentosEscada);
    pthread_join(thread_id, NULL);

    printf("Tempo da última saída: %d\n", tempoUltimaSaida);

    return 0;
}

