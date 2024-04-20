*PROBLEMAS*

1. Qual a estratégia que você utilizou para evitar que duas pessoas acessem a escada rolante ao mesmo tempo em cada abordagem?

Resposta: No código utilizando threads, a exclusão mútua é garantida pelo fato de que o acesso e a modificação da variável compartilhada tempoUltimaSaida estão confinados dentro de uma única thread que executa a função gerenciarEscadaRolante. Como o código fornecido não está executando múltiplas threads em paralelo que acessam e modificam tempoUltimaSaida simultaneamente, não há risco de condição de corrida. Já na implementação com processos, a separação de memória entre os processos já provê uma exclusão mútua natural, pois cada processo tem seu próprio espaço de endereçamento. Isso significa que mesmo que ambos os processos estejam lendo e escrevendo em suas próprias variáveis tempoUltimaSaida, eles não interferem um com o outro diretamente.

2. Como garantir que somente uma das direções está ativa de cada vez em cada uma das abordagens?

Resposta: A exclusão mútua é natural, pois só há uma thread processando todas as pessoas em sequência, de uma direção de cada vez. Cada processo cuida de uma direção e a comunicação via pipe garante que o processo pai aguarde o processo filho terminar antes de concluir o processamento, evitando a ativação simultânea das duas direções.

3. Discorra sobre as diferenças entre as implementações utilizando threads e processos e diga qual foi mais eficiente na solução do problema, justificando sua resposta.

Reposta: Utilizando Threads acaba sendo mais eficientes para o problema da escada rolante, pois são mais leves e têm comunicação mais rápida por compartilharem a mesma memória. Utilizando processos, proporcionam isolamento e são mais robustos, mas são mais pesados e a comunicação entre eles é mais lenta, o que não é necessário para este problema.





*Foto Terminal AWS - Teste código utilizando Threads*


![capturaAWS](https://github.com/guilhermepascucci/guilherme/assets/99357352/a9cdcbcd-ac66-40af-a66f-7e643b0685c4)
