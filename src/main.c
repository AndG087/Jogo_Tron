#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Variáveis globais
int x = 34, y = 12;
int x2 = 54, y2 = 10;
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
int trailLength = 0;
int trail2Length = 0;

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
    return 0;
}

int checkCollision2(int nextX, int nextY) {
    for (int i = 0; i < trail2Length; i++) {
        if (trail2[i].x == nextX && trail2[i].y == nextY) {
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

int main() {
    static int ch = 0;
    int gameOver = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    PrintPlayer1ver(x, y);
    PrintPlayer2ver(x2, y2); // Imprime o segundo jogador
    screenUpdate();

    while (ch != 10 && !gameOver) { // enter or collision
        // Handle user input
        hor = 0;
        ver = 0;
        hor2 = 0;
        ver2 = 0;

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
            // Movimentação do primeiro jogador
            if (ch == 'w') { // w - move up
                newY -= incY;
                if (newY <= MINY + 1) newY = MINY + 1;
                ver = 1;
            } else if (ch == 's') { // s - move down
                newY += incY;
                if (newY >= MAXY - 1) newY = MAXY - 1;
                ver = 1;
            } else if (ch == 'a') { // a - move left
                newX -= incX;
                if (newX <= MINX + 1) newX = MINX + 1;
                hor = 1;
            } else if (ch == 'd') { // d - move right
                newX += incX;
                if (newX >= MAXX - strlen("|") - 1) newX = MAXX - strlen("|") - 1;
                hor = 1;
            }

            // Movimentação do segundo jogador
            if (ch == 'i') { // i - move up
                newY2 -= incY2;
                if (newY2 <= MINY + 1) newY2 = MINY + 1;
                ver2 = 1;
            } else if (ch == 'k') { // k - move down
                newY2 += incY2;
                if (newY2 >= MAXY - 1) newY2 = MAXY - 1;
                ver2 = 1;
            } else if (ch == 'j') { // j - move left
                newX2 -= incX2;
                if (newX2 <= MINX + 1) newX2 = MINX + 1;
                hor2 = 1;
            } else if (ch == 'l') { // l - move right
                newX2 += incX2;
                if (newX2 >= MAXX - strlen("|") - 1) newX2 = MAXX - strlen("|") - 1;
                hor2 = 1;
            }

            // Verificar colisão com o rastro do primeiro jogador
            if (checkCollision(newX, newY)) {
                gameOver = 1;
                // Implemente o que acontece quando o primeiro jogador perde
            }
            // Verificar colisão com o rastro do segundo jogador
            else if (checkCollision2(newX2, newY2)) {
                gameOver = 2; // Indica que o segundo jogador perdeu
                // Implemente o que acontece quando o segundo jogador perde
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
    if (gameOver = 1){
        printf("  ######     ###    ##     ## ########      ######### ##    ## ######## ########\n ##    ##   ## ##   ###   ### ##            ##     ## ##    ## ##       ##     ##\n ##        ##   ##  #### #### ##            ##     ## ##    ## ##       ##     ##\n ##  ###  ##     ## ## ### ## ######        ##     ## ##    ## ######   ########\n ##  # ## ######### ##     ## ##            ##     ## ##    ## ##       ##   ##\n ##  # ## ##     ## ##     ## ##            ##     ##  #    #  ##       ##    ##\n  ######  ##     ## ##     ## ########      #########   ####   ######## ##     ##\n");
    }
    if (gameOver = 2){
        printf("  ######     ###    ##     ## ########      ######### ##    ## ######## ########\n ##    ##   ## ##   ###   ### ##            ##     ## ##    ## ##       ##     ##\n ##        ##   ##  #### #### ##            ##     ## ##    ## ##       ##     ##\n ##  ###  ##     ## ## ### ## ######        ##     ## ##    ## ######   ########\n ##  # ## ######### ##     ## ##            ##     ## ##    ## ##       ##   ##\n ##  # ## ##     ## ##     ## ##            ##     ##  #    #  ##       ##    ##\n  ######  ##     ## ##     ## ########      #########   ####   ######## ##     ##\n");
    }
    return 0;
}
