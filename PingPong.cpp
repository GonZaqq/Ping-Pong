#include <iostream>
#include <conio.h>
#include <windows.h>

using namespace std;

// Объявление и инициализация переменных
const int WIDTH = 40; // Ширина игрового поля
const int HEIGHT = 20; // Высота игрового поля
int paddle1X, paddle1Y; // Позиции игроков по оси Y
int paddle2X, paddle2Y; // Позиции игроков по оси X
int ballX, ballY; // Позиция мяча
int ballDirX, ballDirY; // Скорость мяча по осям X и Y
int score1, score2;
bool gameover;

enum Difficulty {
    EASY,
    HARD
};

enum MenuOption {
    PLAY_1_PLAYER,
    PLAY_2_PLAYERS,
    EXIT
};

Difficulty currentDifficulty = EASY;
MenuOption selectedOption = PLAY_1_PLAYER;

void Setup()
{
    gameover = false;
    paddle1X = WIDTH / 2 - 1;
    paddle1Y = HEIGHT - 2;
    paddle2X = WIDTH / 2 - 1;
    paddle2Y = 1;
    ballX = WIDTH / 2;
    ballY = HEIGHT / 2;
    ballDirX = 1;
    ballDirY = 1;
    score1 = 0;
    score2 = 0;
}

// Отобразить игровое поле
void Draw()
{
    system("cls"); // Очистить экран
    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == 0 || x == WIDTH - 1)
                cout << "#"; // Отобразить боковые границы
            else if (x >= paddle1X && x <= paddle1X && y == paddle1Y)
                cout << "==="; // Отобразить игрока 1
            else if (x >= paddle2X && x <= paddle2X && y == paddle2Y)
                cout << "==="; // Отобразить игрока 2
            else if (x == ballX && y == ballY)
                cout << "O"; // Отобразить мяч
            else
                cout << " ";
        }
        cout << endl;
    }

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score1 << " - " << score2 << endl;
    cout << "Difficulty: " << (currentDifficulty == EASY ? "Easy" : "Hard") << endl;
}

void Input()
{
    // Обработка клавиш игроков
    if (selectedOption == PLAY_1_PLAYER) // Обновление
    {
        if (GetAsyncKeyState('A') & 0x8000) // Движение игрока 1 влево
            paddle1X--;
        if (GetAsyncKeyState('D') & 0x8000) // Движение игрока 1 направо
            paddle1X++;
    }
    else if (selectedOption == PLAY_2_PLAYERS)
    {
        if (GetAsyncKeyState(VK_LEFT) & 0x8000) // Движение игрока 2 влево
            paddle2X--;
        if (GetAsyncKeyState(VK_RIGHT) & 0x8000) // Движение игрока 2 направо
            paddle2X++;
    }
    else if (selectedOption == EXIT)
    {
        if (GetAsyncKeyState(VK_RETURN) & 0x8000)
            gameover = true;
    }
}

void SelectDifficulty()
{
    system("cls");
    cout << "Select Difficulty:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Hard" << endl;

    char choice;
    cin >> choice;
    switch (choice)
    {
    case '1':
        currentDifficulty = EASY;
        break;
    case '2':
        currentDifficulty = HARD;
        break;
    default:
        currentDifficulty = EASY;
        break;
    }
}

void MainMenu()
{
    system("cls");
    cout << "Main Menu:" << endl;
    cout << "1. Play 1 Player" << endl;
    cout << "2. Play 2 Players" << endl;
    cout << "3. Exit" << endl;

    char choice;
    cin >> choice;
    switch (choice)
    {
    case '1':
        selectedOption = PLAY_1_PLAYER;
        break;
    case '2':
        selectedOption = PLAY_2_PLAYERS;
        break;
    case '3':
        selectedOption = EXIT;
        break;
    default:
        selectedOption = PLAY_1_PLAYER;
        break;
    }
}

void Logic()
{
    if (selectedOption == PLAY_1_PLAYER)
    {
        // Движение ракетки для игрока 2 (ИИ)
        if (currentDifficulty == EASY)
        {
            if (ballX < paddle2X)
                paddle2X--;
            if (ballX > paddle2X)
                paddle2X++;
        }
        else if (currentDifficulty == HARD)
        {
            // Искусственный интеллект следует за мячом горизонтально
            if (ballX < paddle2X)
                paddle2X--;
            if (ballX > paddle2X)
                paddle2X++;

            // Искусственный интеллект пытается предсказать вертикальное движение мяча
            if (ballDirY > 0 && ballY > paddle2Y)
                paddle2Y++;
            else if (ballDirY < 0 && ballY < paddle2Y)
                paddle2Y--;
        }
    }

    // Движение мяча
    ballX += ballDirX;
    ballY += ballDirY;

    // Столкновение мяча с границами
    if (ballX == 0 || ballX == WIDTH - 1)
        ballDirX = -ballDirX;
    if (ballY == 0)
        ballDirY = -ballDirY;

    // Столкновение мяча с ракетками
    if (ballY == paddle2Y + 1 && ballX >= paddle2X && ballX <= paddle2X + 1)
        ballDirY = -ballDirY;
    if (ballY == paddle1Y - 1 && ballX >= paddle1X && ballX <= paddle1X + 1)
        ballDirY = -ballDirY;

    // Мяч вышел за пределы игрового поля
    if (ballY == HEIGHT - 1)
    {
        score2++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        ballDirY = -ballDirY;
    }
    if (ballY == 0)
    {
        score1++;
        ballX = WIDTH / 2;
        ballY = HEIGHT / 2;
        ballDirY = -ballDirY;
    }

    // Условие окончания игры
    if (score1 >= 3 || score2 >= 3)
        gameover = true;
}

int main()
{
    while (!gameover)
    {
        MainMenu();

        if (selectedOption == PLAY_1_PLAYER || selectedOption == PLAY_2_PLAYERS)
        {
            SelectDifficulty();
            Setup();

            while (!gameover)
            {
                Draw();
                Input();
                Logic();
                Sleep(10); // Пауза для управления скоростью игры
            }
        }
    }

    system("cls");
    cout << "Game Over!" << endl;
    cout << "Final Score: " << score1 << " - " << score2 << endl;

    return 0;
}
