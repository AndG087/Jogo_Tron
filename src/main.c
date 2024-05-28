#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Variáveis globais
int x, y;
int x2, y2;
int ver = 0, hor = 0;
int ver2 = 0, hor2 = 0;
int incX = 1, incY = 1;             
int incX2 = 1, incY2 = 1;

#define MAX_TRAIL 1000

// Estrutura para armazenar a posição do rastro
typedef struct {
    int x;
    int y;
} Position;

Position trail[MAX_TRAIL];
Position trail2[MAX_TRAIL];
int trailLength;
int trail2Length;

void addTrail(int x, int y) {
    if (trailLength < MAX_TRAIL) {
        trail[trailLength].x = x;
        trail[trailLength].y = y;
        trailLength++;
    }
}

void addTrail2(int x, int y) {
    if (trail2Length < MAX_TRAIL) {
        trail2[trail2Length].x = x;
        trail2[trail2Length].y = y;
        trail2Length++;
    }
}

int checkCollision(int nextX, int nextY) {
    for (int i = 0; i < trailLength; i++) {
        if (trail[i].x == nextX && trail[i].y == nextY) {
            return 1;
        }
    }
    for (int i = 0; i < trail2Length; i++) {
        if (trail2[i].x == nextX && trail2[i].y == nextY) {
            return 1;
        }
    }
    return 0;
}

int checkCollision2(int nextX, int nextY) {
    for (int i = 0; i < trail2Length; i++) {
        if (trail2[i].x == nextX && trail2[i].y == nextY) {
            return 1;
        }
    }
    for (int i = 0; i < trailLength; i++) {
        if (trail[i].x == nextX && trail[i].y == nextY) {
            return 1;
        }
    }
    return 0;
}

// Funções para o jogador 1
void PrintPlayer1ver(int nextX, int nextY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf("|");
}

void PrintPlayer1hor(int nextX, int nextY) {
    screenSetColor(CYAN, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf("=");
}

void printrastro(int nextX, int nextY) {
    screenSetColor(RED, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf(".");
}

// Funções para o jogador 2
void PrintPlayer2ver(int nextX, int nextY) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf("|");
}

void PrintPlayer2hor(int nextX, int nextY) {
    screenSetColor(YELLOW, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf("=");
}

void printrastro2(int nextX, int nextY) {
    screenSetColor(LIGHTGREEN, DARKGRAY);
    screenGotoxy(nextX, nextY);
    printf(".");
}

void displayInitialScreen(char *player1, char *player2) {
    screenInit(1);
    keyboardInit();
    printf("Digite o nome do Jogador 1: ");
    scanf("%s", player1);
    printf("Digite o nome do Jogador 2: ");
    scanf("%s", player2);
    screenDestroy();
    keyboardDestroy();
}

void saveWinner(const char *winner) {
    FILE *file = fopen("winner.txt", "w");
    if (file != NULL) {
        fprintf(file, "Vencedor: %s\n", winner);
        fclose(file);
    } else {
        printf("Erro ao abrir o arquivo para salvar o vencedor.\n");
    }
}

void initializeGame() {
    x = 34; y = 12;
    x2 = 54; y2 = 10;
    ver = hor = ver2 = hor2 = 0;
    trailLength = trail2Length = 0;
    screenInit(1);
    keyboardInit();
    timerInit(50);
    PrintPlayer1ver(x, y);
    PrintPlayer2ver(x2, y2);
    screenUpdate();
}

int playGame() {
    static int ch = 0;
    int gameOver = 0;
    char last_move1 = 'a';
    char last_move2 = 'j';

    while (ch != 10 && !gameOver) { // enter or collision
        // Handle user input
        hor = ver = hor2 = ver2 = 0;

        if (keyhit()) {
            ch = readch();
            screenUpdate();
        }

        // Update game state (move elements, verify collision, etc)
        int newX = x;
        int newY = y;
        int newX2 = x2;
        int newY2 = y2;

        if (timerTimeOver() == 1) {
            // Atualizar o último movimento dos jogadores
            if (ch == 'w' || ch == 's' || ch == 'a' || ch == 'd') {
                last_move1 = ch;
            }
            if (ch == 'i' || ch == 'k' || ch == 'j' || ch == 'l') {
                last_move2 = ch;
            }

            // Movimentação do primeiro jogador
            if (last_move1 == 'w') { // w - move up
                newY -= incY;
                if (newY <= MINY + 1) newY = MINY + 1;
                ver = 1;
            } else if (last_move1 == 's') { // s - move down
                newY += incY;
                if (newY >= MAXY - 1) newY = MAXY - 1;
                ver = 1;
            } else if (last_move1 == 'a') { // a - move left
                newX -= incX;
                if (newX <= MINX + 1) newX = MINX + 1;
                hor = 1;
            } else if (last_move1 == 'd') { // d - move right
                newX += incX;
                if (newX >= MAXX - strlen("|") - 1) newX = MAXX - strlen("|") - 1;
                hor = 1;
            }

            // Movimentação do segundo jogador
            if (last_move2 == 'i') { // i - move up
                newY2 -= incY2;
                if (newY2 <= MINY + 1) newY2 = MINY + 1;
                ver2 = 1;
            } else if (last_move2 == 'k') { // k - move down
                newY2 += incY2;
                if (newY2 >= MAXY - 1) newY2 = MAXY - 1;
                ver2 = 1;
            } else if (last_move2 == 'j') { // j - move left
                newX2 -= incX2;
                if (newX2 <= MINX + 1) newX2 = MINX + 1;
                hor2 = 1;
            } else if (last_move2 == 'l') { // l - move right
                newX2 += incX2;
                if (newX2 >= MAXX - strlen("|") - 1) newX2 = MAXX - strlen("|") - 1;
                hor2 = 1;
            }

            // Verificar colisão com os rastros de ambos os jogadores
            if (checkCollision(newX, newY) || (newX == x2 && newY == y2)) {
                gameOver = 1;
            } else if (checkCollision2(newX2, newY2) || (newX2 == x && newY2 == y)) {
                gameOver = 2;
            } else {
                // Atualizar o rastro e a posição de ambos os jogadores
                addTrail(x, y);
                printrastro(x, y);
                if (ver == 1) {
                    PrintPlayer1ver(newX, newY);
                } else if (hor == 1) {
                    PrintPlayer1hor(newX, newY);
                }
                x = newX;
                y = newY;

                addTrail2(x2, y2);
                printrastro2(x2, y2);
                if (ver2 == 1) {
                    PrintPlayer2ver(newX2, newY2);
                } else if (hor2 == 1) {
                    PrintPlayer2hor(newX2, newY2);
                }
                x2 = newX2;
                y2 = newY2;

                screenUpdate();
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return gameOver;
}

int main() {
    char player1[50], player2[50];
    int winsPlayer1 = 0, winsPlayer2 = 0;
    int winner;

    displayInitialScreen(player1, player2);

    while (winsPlayer1 < 2 && winsPlayer2 < 2) {
        initializeGame();
        winner = playGame();

        if (winner == 1) {
            winsPlayer2++;
        } else if (winner == 2) {
            winsPlayer1++;
        }

        printf("Placar: %s %d x %d %s\n", player1, winsPlayer1, winsPlayer2, player2);
    }

    if (winsPlayer1 == 2) {
        printf("Parabéns %s, você venceu a melhor de 3!\n", player1);
        saveWinner(player1);
    } else {
        printf("Parabéns %s, você venceu a melhor de 3!\n", player2);
        saveWinner(player2);
    }

    return 0;
}
