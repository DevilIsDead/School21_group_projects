#include <stdio.h>

int ballmoveX(int ballX, int ballXD, int ballYD, int ballY);
int ballmoveY(int ballY, int ballXD, int ballYD);
int ballmoveXD(int ballX, int ballXD);
int ballmoveYD(int ballY, int ballYD, int leftRackY, int leftRackX, int rightRackY, int rightRackX,
               int ballX);
void draw(int countright, int countleft, int leftRackX, int leftRackY, int rightRackX, int rightRackY,
          int ballY, int ballX);
int rocketmoveleft(int leftRackX, char key);
int rocketmoveright(int rightRackX, char key);
int countLeftChange(int ballY, int countleft);
int countRightChange(int ballY, int countright);

int main() {              // дефолтные значения
    int leftRackX = 12;   // высота левая ракетка
    int leftRackY = 2;    // отступ слева левая ракетка
    int rightRackX = 12;  // высота правая ракетка
    int rightRackY = 77;  // отступ слева правая ракетка
    int ballY = 39;       //  положение мяча по горизонтали
    int ballX = 12;       //  положение мяча по вертикали
    int countright = 0;   // счет правого игрока
    int countleft = 0;    // счет левого игрока
    int ballYD = 1;       // направление движение мяча по вертикали
    int ballXD = 1;       // направление движение мяча по горизонтали

    do {
        draw(countright, countleft, leftRackX, leftRackY, rightRackX, rightRackY, ballY, ballX);
        ballX = ballmoveX(ballX, ballXD, ballYD, ballY);  // получаем новые значения из функции
        ballY = ballmoveY(ballY, ballXD, ballYD);  // получаем новые значения из функции
        ballXD = ballmoveXD(ballX, ballXD);  // получаем новые значения из функции
        ballYD = ballmoveYD(ballY, ballYD, leftRackY, leftRackX, rightRackY, rightRackX,
                            ballX);  // получаем новые значения из функции
        char key = getchar();        // получаем
        if (key == 'q') {
            printf("\e[1;1H\e[2J");
            printf(
                "==============================\n\n\n\n\n********  Game Over!  ********\n\n\n            %d "
                "| %d\n\n\n\n==============================",
                countright, countleft);
            return 0;
        }
        leftRackX = rocketmoveleft(leftRackX, key);  // получаем новые значения из функции
        rightRackX = rocketmoveright(rightRackX, key);  // получаем новые значения из функции
        countleft = countLeftChange(ballY, countleft);  // получаем новые значения из функции
        countright = countRightChange(ballY, countright);  // получаем новые значения из функции

    } while ((countleft < 21) &&
             (countright < 21));  // играем до тех пор пока счет одного игрока не будет равен 21
    printf("\e[1;1H\e[2J");  // очищаем поле
    printf(
        "==============================\n\n\n\n\n********  Game Over!  ********\n\n\n            %d | "
        "%d\n\n\n\n==============================",
        countright, countleft);
    return 0;
}

int ballmoveX(int ballX, int ballXD, int ballYD,
              int ballY)  // проверяем флаг направления движения мяча по X и изменяем расположение мяча в
                          // правильном направлении по X
{
    if ((ballYD == -1) && (ballXD == 1)) {
        ballX++;
    }
    if ((ballYD == 1) && (ballXD == -1)) {
        ballX--;
    }
    if ((ballYD == -1) && (ballXD == -1)) {
        ballX--;
    }
    if ((ballYD == 1) && (ballXD == 1)) {
        ballX++;
    }

    if ((ballY == 1) || (ballY == 78)) ballX = 12;

    return ballX;
}

int ballmoveY(int ballY, int ballXD, int ballYD)  // проверяем флаг направления движения мяча по Y и изменяем
                                                  // расположение мяча в правильном направлении по Y

{
    if ((ballYD == -1) && (ballXD == 1)) ballY--;

    if ((ballYD == 1) && (ballXD == -1)) ballY++;

    if ((ballYD == -1) && (ballXD == -1)) ballY--;

    if ((ballYD == 1) && (ballXD == 1)) ballY++;

    if ((ballY == 0) || (ballY == 79)) ballY = 39;

    return ballY;
}

int ballmoveXD(int ballX, int ballXD)  // функция изменения флага направления движения мяча по X
{
    if (ballX - 1 == 0)  // мяч достигает верхней границы - меняем направление
    {
        ballXD *= -1;
    }

    if (ballX + 1 == 24)  // мяч достигает нижней границы - меняем направление
    {
        ballXD *= -1;
    }
    return ballXD;
}

int ballmoveYD(int ballY, int ballYD, int leftRackY, int leftRackX, int rightRackY, int rightRackX,
               int ballX)  // функция изменения флага направления движения мяча по Y
{
    if ((ballY - 1 == leftRackY) &&
        ((ballX == leftRackX) || (ballX == leftRackX + 1) ||
         (ballX == leftRackX - 1))) {  // мяч достигает левой ракетки - меняем направление
        ballYD *= -1;
    }

    if ((ballY + 1 == rightRackY) &&
        ((ballX == rightRackX) || (ballX == rightRackX + 1) ||
         (ballX == rightRackX - 1))) {  // мяч достигает правой ракетки - меняем направление
        ballYD *= -1;
    }
    return ballYD;
}

void draw(int countright, int countleft, int leftRackX, int leftRackY, int rightRackX, int rightRackY,
          int ballY, int ballX)  // функция отрисовки поля
{
    printf("\e[1;1H\e[2J");  // очистка поля
    printf("                                      %d | %d\n", countright,
           countleft);  // рисуем пока не дойдем до края по высоте, до низа
    for (int x = 0; x < 25; x++) {
        for (int y = 0; y < 80; y++)  // рисуем пока не дойдем до края по ширине , до правого края
        {
            if (x == 0 || x == 24)  // граница верхняя и граница нижняя
            {
                printf("-");
                continue;
            }

            if (y == 0 || y == 79)  // граница левая и граница правая
            {
                printf("|");
                continue;
            }

            if (((y == leftRackY) && (x == leftRackX)) || ((y == leftRackY) && (x == leftRackX + 1)) ||
                ((y == leftRackY) && (x == leftRackX - 1))) {  // левая ракетка
                printf("H");
                continue;
            }

            if (((y == rightRackY) && (x == rightRackX)) || ((y == rightRackY) && (x == rightRackX + 1)) ||
                ((y == rightRackY) && (x == rightRackX - 1))) {  // правая ракетка
                printf("H");
                continue;
            }

            if (y == ballY && x == ballX) {  // мяч
                printf("@");
                continue;
            }

            printf(" ");
        }
        printf("\n");
    }
}

int rocketmoveleft(int leftRackX, char key)  // функция движения левой ракетки
{
    if (key == 'z' && (leftRackX < 22)) leftRackX++;
    if (key == 'a' && (leftRackX > 2)) leftRackX--;
    return leftRackX;
}

int rocketmoveright(int rightRackX, char key)  // функция движения правой ракетки
{
    if (key == 'm' && (rightRackX < 22)) rightRackX++;
    if (key == 'k' && (rightRackX > 2)) rightRackX--;
    return rightRackX;
}

int countLeftChange(int ballY, int countleft)  // функция изменения счета левого игрока
{
    if (ballY == 1) countleft++;
    return countleft;
}

int countRightChange(int ballY, int countright)  // функция изменения счета правого игрока
{
    if (ballY == 78) countright++;
    return countright;
}
