#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
#define MAX_OBSTACLES 20
#define POWER_UP_DURATION 100

// Estrutura para armazenar a posição do rastro e dos obstáculos
typedef struct {
    int x;
    int y;
} Position;

// Estrutura para lista encadeada dos vencedores
typedef struct Node {
    char name[50];
    int wins;
    struct Node *next;
} Node;

// Variáveis da lista encadeada
Node *head = NULL;

Position trail[MAX_TRAIL];
Position trail2[MAX_TRAIL];
Position obstacles[MAX_OBSTACLES];
int trailLength;
int trail2Length;
int powerUpActive1 = 0;
int powerUpActive2 = 0;
int powerUpCounter1 = 0;
int powerUpCounter2 = 0;

// Funções para manipulação da lista encadeada
Node* createNode(const char *name, int wins) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Erro ao alocar memória\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->wins = wins;
    newNode->next = NULL;
    return newNode;
}

void appendNode(Node **head, const char *name, int wins) {
    Node *newNode = createNode(name, wins);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    Node *temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

void saveListToFile(Node *head, const char *filename) {
    FILE *file = fopen(filename, "a"); // Abre o arquivo em modo de append
    if (file == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo para salvar a lista\n");
        exit(1);
    }
    Node *temp = head;
    while (temp != NULL) {
        fprintf(file, "%s %d\n", temp->name, temp->wins);
        temp = temp->next;
    }
    fclose(file);
}

void freeList(Node *head) {
    Node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

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
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x == nextX && obstacles[i].y == nextY) {
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
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        if (obstacles[i].x == nextX && obstacles[i].y == nextY) {
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

// Funções para Power-ups
void printPowerUp(int x, int y) {
    screenSetColor(MAGENTA, DARKGRAY);
    screenGotoxy(x, y);
    printf("*");
}

void activatePowerUp1() {
    powerUpActive1 = 1;
    powerUpCounter1 = 0;
}

void activatePowerUp2() {
    powerUpActive2 = 1;
    powerUpCounter2 = 0;
}

void deactivatePowerUps() {
    if (powerUpActive1 && powerUpCounter1 >= POWER_UP_DURATION) {
        powerUpActive1 = 0;
    }
    if (powerUpActive2 && powerUpCounter2 >= POWER_UP_DURATION) {
        powerUpActive2 = 0;
    }
}

void displayInitialScreen(char *player1, char *player2) {
    screenInit(1);
    keyboardInit();

    // Exibir o anagrama do nome do jogo
    printf("\n\n\n\n |           ██████╗ ███████╗ █████╗ ████████╗██╗  ██╗    ██████╗  █████╗  ██████╗███████╗\n");
    printf(" |           ██╔══██╗██╔════╝██╔══██╗╚══██╔══╝██║  ██║    ██╔══██╗██╔══██╗██╔════╝██╔════╝\n");
    printf(" |           ██║  ██║█████╗  ███████║   ██║   ███████║    ██████╔╝███████║██║     █████╗  \n");
    printf(" |           ██║  ██║██╔══╝  ██╔══██║   ██║   ██╔══██║    ██╔══██╗██╔══██║██║     ██╔══╝  \n");
    printf(" |           ██████╔╝███████╗██║  ██║   ██║   ██║  ██║    ██║  ██║██║  ██║╚██████╗███████╗\n");
    printf(" |           ╚═════╝ ╚══════╝╚═╝  ╚═╝   ╚═╝   ╚═╝  ╚═╝    ╚═╝  ╚═╝╚═╝  ╚═╝ ╚═════╝╚══════╝\n");

    printf("\nJogador 1 = AZUL");
    printf("\nJogador 2 = AMARELO");

    // Coletar os nomes dos jogadores
    printf("\nDigite o nome do Jogador 1: ");
    scanf("%s", player1);
    printf("%s", player1);
    printf("\n\nDigite o nome do Jogador 2: ");
    scanf("%s", player2);
    printf("%s", player2);

    screenDestroy();
    keyboardDestroy();
}

void initializeGame() {
    x = 34;
    y = 16;
    x2 = 90;
    y2 = 20;
    ver = hor = ver2 = hor2 = 0;
    trailLength = trail2Length = 0;
    powerUpActive1 = powerUpActive2 = 0;
    screenInit(1);
    keyboardInit();
    timerInit(50);
    PrintPlayer1ver(x, y);
    PrintPlayer2ver(x2, y2);

    // Gerar obstáculos aleatórios
    srand(time(NULL));
    for (int i = 0; i < MAX_OBSTACLES; i++) {
        obstacles[i].x = rand() % (MAXX - MINX + 1) + MINX;
        obstacles[i].y = rand() % (MAXY - MINY + 1) + MINY;
        screenSetColor(WHITE, DARKGRAY);
        screenGotoxy(obstacles[i].x, obstacles[i].y);
        printf("#");
    }

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

            // Verificar colisão com os rastros e obstáculos de ambos os jogadores
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

                // Gerenciar a duração dos power-ups
                if (powerUpActive1) powerUpCounter1++;
                if (powerUpActive2) powerUpCounter2++;
                deactivatePowerUps();

                screenUpdate();
            }
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return gameOver;
}

void saveWinner(const char *winner) {
    Node *temp = head;
    while (temp != NULL) {
        if (strcmp(temp->name, winner) == 0) {
            temp->wins++;
            saveListToFile(head, "winner.txt");
            return;
        }
        temp = temp->next;
    }
    appendNode(&head, winner, 1);
    saveListToFile(head, "winner.txt");
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

    freeList(head);
    return 0;
}
