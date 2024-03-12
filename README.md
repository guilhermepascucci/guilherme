# Sistemas Operacionais - Laboratório 2

Questão 1

a) Sim, no código acima, depois da chamada fork (), o processo filho executa o comando ls e imprime sua frase, enquanto o processo pai espera pelo término do processo filho para poder imprimeir sua frase, seguindo por "child complete" após o fim do processo filho.

b) Serão criados 8 processos no total, nos quais se iniciam com o processo original, que começa a execução, a primeira chamada de fork(), duplicando o processo, assim como a segunda chamada de fork(), e por fim a terceira chamada de fork(), duplicando novamente o número de processos.

