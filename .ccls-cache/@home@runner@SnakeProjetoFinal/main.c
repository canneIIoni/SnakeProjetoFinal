#include <stdio.h>
#include <stdlib.h>

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
void printBoard(Snake* snake, Food food);

int main() {
    Snake* snake = initializeSnake(5, 5);
    Food food = {10, 10}; // Posição inicial da comida

    // Loop principal do jogo
    while (1) {
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