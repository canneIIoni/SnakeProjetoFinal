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
void moveSnake(Snake* snake, int dx, int dy, int* ateFood);
int checkCollision(Snake* snake);

// Funções para a comida
Food generateFood();
int checkFoodCollision(Snake* snake, Food food);

// Funções para o jogo
void printBoard(Snake* snake, Food food);
void gameOver();

// Função para verificar se há entrada de teclado sem bloqueio
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

int main() {
    Snake* snake = initializeSnake(5, 5);
    Food food = generateFood();
    int ateFood = 0;

    char direction = 'D'; // Inicialmente, a cobra se move para a direita

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
      moveSnake(snake, dx, dy, &ateFood);

      // Verificar colisão com a comida
      if (checkFoodCollision(snake, food)) {
          ateFood = 1;  // Define a flag para indicar que a cobra comeu a fruta
          food = generateFood();
      }

      // Verificar colisão com a parede ou consigo mesma
      if (checkCollision(snake)) {
          gameOver();
          break;
      }

        // Imprimir o tabuleiro
        printBoard(snake, food);

        // Aguardar por um curto período para controlar a velocidade do jogo
        sleep(1);
        // Limpar a tela
        system("clear");
    }

    free(snake);
    return 0;
}

// Implementação das demais funções

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

// Função para adicionar um nó à cobra
void addToSnake(Snake* snake, int x, int y) {
    Node* newNode = createNode(x, y);
    newNode->next = snake->head;
    snake->head = newNode;
}
// Função para mover a cobra
void moveSnake(Snake* snake, int dx, int dy, int* ateFood) {
    // Adiciona um novo nó à cabeça da cobra
    addToSnake(snake, snake->head->x + dx, snake->head->y + dy);

    // Atualiza a posição da cauda
    Node* current = snake->head;
    Node* prev = NULL;

    while (current->next != NULL) {
        prev = current;
        current = current->next;
    }

    // Verifica se a cobra comeu a fruta
    if (*ateFood) {
        // Se a cobra comeu a fruta, não remove a cauda neste movimento
        *ateFood = 0;
    } else {
        // Se a cobra não comeu a fruta, remove o último nó da cauda
        free(current);
        prev->next = NULL;
    }
}

// Função para verificar colisão com a parede ou consigo mesma
int checkCollision(Snake* snake) {
    int headX = snake->head->x;
    int headY = snake->head->y;

    // Verificar colisão com a parede
    if (headX < 0 || headX >= 20 || headY < 0 || headY >= 20) {
        return 1;
    }

    // Verificar colisão consigo mesma
    Node* current = snake->head->next; // Começar a partir do segundo nó
    while (current != NULL) {
        if (headX == current->x && headY == current->y) {
            return 1;
        }
        current = current->next;
    }

    return 0;
}

// Função para gerar comida em uma posição aleatória
Food generateFood() {
    Food food;
    food.x = rand() % 20;
    food.y = rand() % 20;
    return food;
}

// Função para verificar colisão com a comida
int checkFoodCollision(Snake* snake, Food food) {
    return (snake->head->x == food.x && snake->head->y == food.y);
}

// Função para imprimir o tabuleiro
void printBoard(Snake* snake, Food food) {
    // Criar uma matriz para representar o tabuleiro
    char board[22][22];  // Increase the size of the board to accommodate borders

    // Inicializar o tabuleiro com espaços em branco
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            // Adicionar bordas ao redor do tabuleiro
            if (i == 0 || i == 21 || j == 0 || j == 21) {
                board[i][j] = '#';  // Use '#' for borders
            } else {
                board[i][j] = ' ';
            }
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
    for (int i = 0; i < 22; i++) {
        for (int j = 0; j < 22; j++) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

// Função para exibir a mensagem de fim de jogo
void gameOver() {
    printf("Game Over!\n");
}