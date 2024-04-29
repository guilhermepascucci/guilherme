*Sistemas Operacionais - Laboratório 04*

Código executado no terminal AWS:

![image](https://github.com/guilhermepascucci/guilherme/assets/99357352/a856b57f-27f9-4cc9-9945-cbbb04492523)

Explicação geral do código feito:

As constantes M e N são definidas para especificar as dimensões da matriz A e do vetor x. Contendo a estrutura chamada thread_data que é usada para armazenar o índice da linha que cada thread deve processar. Cada thread executa a função 'multiply', que calcula o produto da linha especificada da matriz A com o vetor x e armazena o resultado em y. E na main, inicializa a matriz e o vetor com valores, cria uma thread para cada linha da matriz, espera pelo término das threads e imprime o vetor resultado y.
