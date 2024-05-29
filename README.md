# Jogo_Tron

Um jogo super divertido, desenvolvido para a disciplina de PIF da Faculdade Cesar School. A proposta do jogo é inspirada no jogo de arcade Tron (sim, o mesmo que inspirou o filme) e é feito na linguagem C.

## cli-lib

Biblioteca de Interface de Linha de Comando, para desenvolvimento de aplicativos e jogos CLI em C. Possui funções para acessar teclado, tela e gerenciar tarefas de temporização.

### Requisitos

Esta biblioteca funciona com os seguintes sistemas operacionais:

- Sistemas baseados em Linux (Ubuntu, etc)
- MacOS

É necessário ter o GCC instalado.

### Uso

O arquivo `main.c` contém um exemplo de como usar as funções de Teclado, Tela e Temporizador.

Para compilar este exemplo via linha de comando, basta navegar até o diretório raiz da biblioteca e digitar o seguinte comando:

```bash
gcc ./src/*.c -I./include -o tron.out

```
Para executar:

```bash
./tron.out
```
