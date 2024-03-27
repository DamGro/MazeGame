#include <stdio.h>
#include <stdlib.h>

char** maze;
int rows;
int cols;
int start_row;
int start_col;
int player_row;
int player_col;
int finish_row;
int finish_col;
int score = 0;

void alloc_maze()
{
    maze = malloc(rows * sizeof(char*));

    int i;
    for (i = 0; i < rows; ++i) {
        maze[i] = malloc(cols * sizeof(char));
    }
}

void ReadMaze(char* file_name)
{
    char c;
    FILE* maze_file;
    errno_t err = fopen_s(&maze_file, file_name, "r");

    if (err == 0) {
        alloc_maze();
        int i, j;

        for (i = 0; i < rows; i++) {
            for (j = 0; j < cols; j++) {
                if ((c = fgetc(maze_file)) == '\n') {
                    c = fgetc(maze_file);
                }

                maze[i][j] = c;

                if (c == 'P') {
                    player_row = i;
                    player_col = j;
                }
                if (c == 'F') {
                    finish_col = j;
                    finish_row = i;
                }
            }
        }
    }
    else {
        printf("No such file!\n");
        return;
    }

    fclose(maze_file);
}

void print_maze()
{
    int i, j;
    system("CLS");
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c", maze[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
}

void Table_score()
{
    FILE* plik;
    errno_t result = fopen_s(&plik, "result.txt", "r");  // Otwórz plik w trybie odczytu ("r")

    if (result != 0) {
        printf("Error!\n");
        return;
    }

    char c;
    while ((c = fgetc(plik)) != EOF) {  // Czytaj znaki z pliku i wyświetl je na ekranie
        putchar(c);
    }

    fclose(plik);
}

void get_lvl()
{
    int lvl;
    printf("Podaj poziom trudnosci, 1 = latwy, 2 = sredni, 3 = trudny.\n");
    scanf_s("%d", &lvl);

    switch (lvl)
    {
    case 1:
        rows = 12;
        cols = 20;
        ReadMaze("Latwe.txt");
        break;
    case 2:
        rows = 25;
        cols = 37;
        ReadMaze("Sredni.txt");
        break;
    case 3:
        rows = 41;
        cols = 61;
        ReadMaze("Trudny.txt");
        break;
    default:
        printf("Niepoprawne dane!\n");
        break;
    }
}



void play()
{
    char ch;
    int px, py = 0;
    get_lvl();

    while (1) {
        if ((player_col == finish_col) && (player_row == finish_row)) {
            printf("GAME OVER!\n");
            FILE* file;
            errno_t fptr = fopen_s(&file, "result.txt", "a");
            char name[30];

            if (fptr != 0) {
                printf("Error!\n");
                exit(1);
            }

            printf("Enter name: ");
            scanf_s("%s", name, 30);
            fprintf_s(file, "%s - > %d\n", name, score);
            fclose(file);
            return;
        }
        print_maze();
        scanf_s(" %c", &ch, 1);

        switch (ch) {
        case 'w':
            px = player_row - 1;
            py = player_col;
            if (px >= 0 && (maze[px][py] == ' ' || maze[px][py] == 'F')) {
                maze[player_row][player_col] = ' ';
                maze[px][py] = 'P';
                ++score;
                player_row = px;
            }
            break;
        case 'd':
            px = player_row;
            py = player_col + 1;
            if (py < cols && (maze[px][py] == ' ' || maze[px][py] == 'F')) {
                maze[player_row][player_col] = ' ';
                maze[px][py] = 'P';
                ++score;
                player_col = py;
            }
            break;
        case 'a':
            px = player_row;
            py = player_col - 1;
            if (py >= 0 && (maze[px][py] == ' ' || maze[px][py] == 'F')) {
                maze[player_row][player_col] = ' ';
                maze[px][py] = 'P';
                ++score;
                player_col = py;
            }
            break;
        case 's':
            px = player_row + 1;
            py = player_col;
            if (px < rows && (maze[px][py] == ' ' || maze[px][py] == 'F')) {
                maze[player_row][player_col] = ' ';
                maze[px][py] = 'P';
                ++score;
                player_row = px;
            }
            break;
        case 'p':
            printf("GAME OVER!\n");
            FILE* file;
            errno_t fptr = fopen_s(&file, "result.txt", "a");
            char name[64];

            if (fptr != 0) {
                printf("Error!\n");
                exit(1);
            }
            printf("Enter name: ");
            scanf_s("%s", name, (unsigned int)(sizeof(name) / sizeof(name[0])));
            fprintf_s(file, "%s - > %d\n", name, score);
            fclose(file);
            return;
        }
    }
}

int main()
{
    int l;
    printf("Podaj poziom trudnosci, 1 = play, 2 = wyniki \n");
    scanf_s("%d", &l);

    switch (l) {
    case 1:
        play();
        break;
    case 2:
        Table_score();
        break;
    }

    return 0;
}
