#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

// Variáveis globais
int x = 34, y = 12 ,ver = 0, hor = 0;
int incX = 1, incY = 1;

// Definições para o tamanho máximo da tela (ajuste conforme necessário)
#define MAX_TRAIL 1000

// Estrutura para armazenar a posição do rastro
typedef struct {
    int x;
    int y;
} Position;

Position trail[MAX_TRAIL];
int trailLength = 0;

void addTrail(int x, int y) {
    if (trailLength < MAX_TRAIL) {
        trail[trailLength].x = x;
        trail[trailLength].y = y;
        trailLength++;
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

int main() {
    static int ch = 0;
    int gameOver = 0;

    screenInit(1);
    keyboardInit();
    timerInit(50);

    PrintPlayer1ver(x, y);
    screenUpdate();

    while (ch != 10 && !gameOver) { // enter or collision
        // Handle user input
        hor = 0;
        ver = 0;
        
        if (keyhit()) {
            ch = readch();
            screenUpdate();
        }

        // Update game state (move elements, verify collision, etc)
        int newX = x;
        int newY = y;
        
        if (timerTimeOver() == 1) {
            
            
            // Movimentação com base na tecla pressionada
            if (ch == 119) { // w - move up
                newY -= incY;
                if (newY <= MINY + 1) newY = MINY + 1;
                ver = 1;
            } else if (ch == 115) { // s - move down
                newY += incY;
                if (newY >= MAXY - 1) newY = MAXY - 1;
                ver = 1;
            } else if (ch == 97) { // a - move left
                newX -= incX;
                if (newX <= MINX + 1) newX = MINX + 1;
                hor = 1;
            } else if (ch == 100) { // d - move right
                newX += incX;
                if (newX >= MAXX - strlen("|") - 1) newX = MAXX - strlen("|") - 1;
                hor = 1;
            } else if(ch == 0){
                newX += incX;
                if (newX >= MAXX - strlen("|") - 1) newX = MAXX - strlen("|") - 1;
                hor = 1;
            }

            // Verificar colisão com o rastro
            if (checkCollision(newX, newY)) {
                gameOver = 1;
                screenSetColor(RED, DARKGRAY);
                screenGotoxy(35, 12);
                
            } else {
                // Printar rastro
                addTrail(x, y);
                printrastro(x, y);

                if(ver == 1){
                    PrintPlayer1ver(newX, newY);
                }else if(hor == 1){
                    PrintPlayer1hor(newX, newY);
                }
                

                // Atualizar coordenadas
                x = newX;
                y = newY;

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
    return 0;
}
