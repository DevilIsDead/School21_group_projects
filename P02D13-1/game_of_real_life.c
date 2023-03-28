#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 25
#define M 80

int input_game_mode();
int input_chance();
void input_random(char (*mtrx)[M], int flag);
void output_field(char (*mtrx)[M]);
void output_field_w(char (*mtrx)[M]);

void create_field(char (*game_field)[M], char (*game_field_new)[M]);
void authentication_field_file(char (*mtrx)[M], int mode);
int count_number_neighbors(char (*mtrx)[M], int i, int j);
void copy_field_from_to(char (*mtrx_from)[M], char (*mtrx_to)[M]);
void start_menu(char (*game_field_new)[M], int *chance);
int are_changes(char (*mtrx1)[M], char (*mtrx2)[M]);

int no_walls(char (*mtrx)[M], int i, int j);
int up_wall(char (*mtrx)[M], int i, int j);
int down_wall(char (*mtrx)[M], int i, int j);
int left_up_corner(char (*mtrx)[M], int i, int j);
int left_down_corner(char (*mtrx)[M], int i, int j);
int right_up_corner(char (*mtrx)[M], int i, int j);
int right_down_corner(char (*mtrx)[M], int i, int j);
int left_wall(char (*mtrx)[M], int i, int j);
int right_wall(char (*mtrx)[M], int i, int j);

int change_speed(int speed, char command, int *steps);
void finish();

int main() {
    char game_field_new[N][M], game_field[N][M];
    int speed = 3, steps = 1;
    int flag_random;

    authentication_field_file(game_field_new, -1);
    output_field(game_field_new);
    start_menu(game_field_new, &flag_random);

    initscr();
    while (are_changes(game_field_new, game_field) && steps != 0) {
        copy_field_from_to(game_field_new, game_field);
        create_field(game_field, game_field_new);
        output_field_w(game_field_new);
        halfdelay(speed);
        char command = getch();
        speed = change_speed(speed, command, &steps);
        clear();
    }
    finish();

    printf("End 'Game of Life'");
    return 0;
}

int change_speed(int speed, char command, int *steps) {
    switch (command) {
        case 'q':
            *steps = 0;
            break;
        case '-':
            if (speed < 5) speed += 1;
            break;
        case '+':
            if (speed > 1) speed -= 1;
            break;
    }
    return speed;
}

void finish() {
    curs_set(1);
    clear();
    refresh();
    resetty();
    endwin();
}

void create_field(char (*game_field)[M], char (*game_field_new)[M]) {
    for (int i = 1; i < N - 1; ++i)
        for (int j = 1; j < M - 1; ++j) {
            int num_nei = count_number_neighbors(game_field, i, j);
            if (game_field[i][j] == ' ' && num_nei == 3) game_field_new[i][j] = '*';
            if (game_field[i][j] == '*' && (num_nei != 3 && num_nei != 2)) game_field_new[i][j] = ' ';
        }
}

void start_menu(char (*game_field_new)[M], int *chance) {
    int mode = input_game_mode();
    if (mode)
        authentication_field_file(game_field_new, mode);
    else {
        *chance = input_chance();
        input_random(game_field_new, *chance);
    }
}

int input_game_mode() {
    int flg2 = 1, mode = 0;
    do {
        printf("Choose game mode [0-5]: ");
        scanf("%d", &mode);
        if (mode < 0 || mode > 5) {
            flg2 = 1;
            printf("Incorrect mode. Try again!\n");
        } else
            flg2 = 0;
    } while (flg2);
    return mode;
}

int input_chance() {
    int flg2 = 1;
    int chance;
    do {
        printf("Chance of cell birth [2-100] = 1/");
        scanf("%d", &chance);
        if (chance >= 2 && chance <= 100)
            flg2 = 0;
        else {
            printf("Incorrect chance. Try again!\n");
            flg2 = 1;
        }
    } while (flg2);
    return chance;
}

void authentication_field_file(char (*mtrx)[M], int mode) {
    FILE *fp;
    switch (mode) {
        case 1:
            fp = fopen("clock.txt", "r");
            break;
        case 2:
            fp = fopen("star.txt", "r");
            break;
        case 3:
            fp = fopen("spaceship.txt", "r");
            break;
        case 4:
            fp = fopen("honey_farm.txt", "r");
            break;
        case 5:
            fp = fopen("Gosper_gun.txt", "r");
            break;
        case -1:
            fp = fopen("config.txt", "r");
            break;
    }

    if (fp == NULL)
        printf("Error reading file\n");
    else
        for (int i = 0; i <= N; i++)
            for (int j = 0; j <= M; j++) fscanf(fp, "%c", &mtrx[i][j]);
    fclose(fp);
}

void output_field(char (*mtrx)[M]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) printf("%c", mtrx[i][j]);
        printf("\n");
    }
}

void output_field_w(char (*mtrx)[M]) {
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) printw("%c", mtrx[i][j]);
        printw("\n");
    }
}

int count_number_neighbors(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (i == 1 && j == 1)
        number_neighbors = left_up_corner(mtrx, i, j);
    else if (i == 1 && j == M - 2)
        number_neighbors = right_up_corner(mtrx, i, j);
    else if (i == N - 2 && j == 1)
        number_neighbors = left_down_corner(mtrx, i, j);
    else if (i == N - 2 && j == M - 2)
        number_neighbors = right_down_corner(mtrx, i, j);
    else if (i == 1)
        number_neighbors = up_wall(mtrx, i, j);
    else if (i == N - 2)
        number_neighbors = down_wall(mtrx, i, j);
    else if (j == 1)
        number_neighbors = left_wall(mtrx, i, j);
    else if (j == M - 2)
        number_neighbors = right_wall(mtrx, i, j);
    else
        number_neighbors = no_walls(mtrx, i, j);
    return number_neighbors;
}

int no_walls(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int right_wall(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i][1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int left_wall(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i + 1][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i][M - 2] == '*') number_neighbors += 1;
    return number_neighbors;
}

int up_wall(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[N - 2][j] == '*') number_neighbors += 1;
    if (mtrx[N - 2][j - 1] == '*') number_neighbors += 1;
    if (mtrx[N - 2][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int down_wall(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[1][j] == '*') number_neighbors += 1;
    if (mtrx[1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int left_up_corner(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[N - 2][j] == '*') number_neighbors += 1;
    if (mtrx[N - 2][M - 2] == '*') number_neighbors += 1;
    if (mtrx[N - 2][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int left_down_corner(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i - 1][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[1][M - 2] == '*') number_neighbors += 1;
    if (mtrx[1][j] == '*') number_neighbors += 1;
    if (mtrx[1][j + 1] == '*') number_neighbors += 1;
    if (mtrx[i][M - 2] == '*') number_neighbors += 1;
    if (mtrx[i][j + 1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int right_up_corner(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[N - 2][j] == '*') number_neighbors += 1;
    if (mtrx[N - 2][j - 1] == '*') number_neighbors += 1;
    if (mtrx[N - 2][1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i + 1][j] == '*') number_neighbors += 1;
    if (mtrx[i + 1][1] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i][1] == '*') number_neighbors += 1;
    return number_neighbors;
}

int right_down_corner(char (*mtrx)[M], int i, int j) {
    int number_neighbors = 0;
    if (mtrx[i - 1][j] == '*') number_neighbors += 1;
    if (mtrx[i - 1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i - 1][1] == '*') number_neighbors += 1;
    if (mtrx[1][j - 1] == '*') number_neighbors += 1;
    if (mtrx[1][j] == '*') number_neighbors += 1;
    if (mtrx[1][1] == '*') number_neighbors += 1;
    if (mtrx[i][j - 1] == '*') number_neighbors += 1;
    if (mtrx[i][1] == '*') number_neighbors += 1;
    return number_neighbors;
}

void copy_field_from_to(char (*mtrx_from)[M], char (*mtrx_to)[M]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j) mtrx_to[i][j] = mtrx_from[i][j];
}

void input_random(char (*mtrx)[M], int flag) {
    srand(time(NULL));
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            if (i == 0 || j == 0 || i == N - 1 || j == M - 1)
                mtrx[i][j] = '+';
            else {
                int tmp = rand() % flag + 1;
                if (tmp < flag)
                    mtrx[i][j] = ' ';
                else
                    mtrx[i][j] = '*';
            }
}

int are_changes(char (*mtrx1)[M], char (*mtrx2)[M]) {
    int flg = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (mtrx1[i][j] != mtrx2[i][j]) flg = 1;
    return flg;
}