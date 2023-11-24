#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  // Adicionado para a função sleep
#include <termios.h>
#include <fcntl.h>

// Definição de estruturas
typedef struct Node {
    int x, y;
    struct Node* next;
} Node;

typedef struct Snake {
    Node* head;
    Node* tail;
} Snake;

typedef struct Food {
    int x, y;
} Food;

// Funções para a cobra
Snake* initializeSnake(int x, int y);
void addToSnake(Snake* snake, int x, int y);
void moveSnake(Snake* snake, int dx, int dy);

// Funções para o jogo
void printBoard(Snake* snake, Food food);
void gameOver();

int kbhit();  // Adicionada a declaração da função kbhit

int main() {
    Snake* snake = initializeSnake(5, 5);
    Food food = {10, 10}; // Posição inicial da comida

    char direction = 'D'; // Inicialmente, a cobra se move para a direita

    // Loop principal do jogo
    while (1) {
        // Lógica do jogo
        if (kbhit()) {
            direction = getchar();
        }

        int dx = 0, dy = 0;

        // Atualizar a direção com base na entrada
        switch (direction) {
            case 'W':
                dy = -1;
                break;
            case 'S':
                dy = 1;
                break;
            case 'A':
                dx = -1;
                break;
            case 'D':
                dx = 1;
                break;
        }

        // Mover a cobra
        moveSnake(snake, dx, dy);

        // Imprimir o tabuleiro
        printBoard(snake, food);

        // Aguardar por um curto período para controlar a velocidade do jogo
        sleep(1);
        // Limpar a tela
        system("clear");
    }

    free(snake); // Liberar a memória alocada para a cobra
    return 0;
}

// Função auxiliar para criar um novo nó
Node* createNode(int x, int y) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = NULL;
    return newNode;
}

// Função para inicializar a cobra
Snake* initializeSnake(int x, int y) {
    Snake* snake = (Snake*)malloc(sizeof(Snake));
    Node* head = createNode(x, y);
    snake->head = head;
    snake->tail = head;
    return snake;
}

// Função para imprimir o tabuleiro
void printBoard(Snake* snake, Food food) {
    // Criar uma matriz para representar o tabuleiro
    char board[20][20];

    // Inicializar o tabuleiro com espaços em branco
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            board[i][j] = ' ';
        }
    }

    // Colocar a cobra no tabuleiro
    Node* current = snake->head;
    while (current != NULL) {
        // Adicionar a cobra ao tabuleiro
        board[current->y][current->x] = 'o';
        current = current->next;
    }

    // Colocar a comida no tabuleiro
    board[food.y][food.x] = '*';

    // Imprimir o tabuleiro
    for (int i = 0; i < 20; i++) {
        for (int j = 0; j < 20; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Função para adicionar um nó à cobra
void addToSnake(Snake* snake, int x, int y) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->next = snake->head;
    snake->head = newNode;
}

// Função para mover a cobra
void moveSnake(Snake* snake, int dx, int dy) {
    // Adiciona um novo nó à cabeça da cobra
    addToSnake(snake, snake->head->x + dx, snake->head->y + dy);

    // Atualiza a posição da cauda
    Node* current = snake->head;
    Node* prev = NULL;

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    // Remove o último nó da cauda
    free(current);
    prev->next = NULL;
}

int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Obter as configurações atuais do terminal
    tcgetattr(STDIN_FILENO, &oldt);

    // Configurar o terminal para não bloquear a entrada
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Obter o estado do arquivo de entrada padrão e configurá-lo para não bloquear
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Tentar ler um caractere
    ch = getchar();

    // Restaurar as configurações originais do terminal e do arquivo
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Se um caractere foi lido, desfazer o buffer e retornar 1
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}