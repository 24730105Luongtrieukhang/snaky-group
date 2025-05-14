#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>
#include <vector>
using namespace std;

#define WIDTH 50
#define HEIGHT 20

void gotoxy(int column, int line);
void setColor(int color);

struct Point {
    int x, y;
};

struct Food {
    Point pos;
    int type; // 0: normal, 1: speed up, 2: slow down, 3: bonus points
    char symbol;
    int color;
    int points;
};

class CONRAN {
public:
    struct Point A[100];
    vector<Food> foods;
    int DoDai;
    int diem;
    int level;
    int speed;
    bool gameover;
    vector<Point> obstacles;

    CONRAN() {
        DoDai = 3;
        A[0].x = 12; A[0].y = 10;
        A[1].x = 11; A[1].y = 10;
        A[2].x = 10; A[2].y = 10;
        diem = 0;
        level = 1;
        speed = 100;
        gameover = false;
        TaoMoi();        // Gọi tạo thức ăn trước
        TaoVatCan();     // Tạo vật cản sau
    }

    void TaoVatCan() {
        obstacles.clear();
        int numObstacles = level * 2;
        for (int i = 0; i < numObstacles; i++) {
            Point obstacle;
            do {
                obstacle.x = rand() % (WIDTH - 2) + 1;
                obstacle.y = rand() % (HEIGHT - 2) + 1;
            } while (!ViTriHopLe(obstacle));
            obstacles.push_back(obstacle);
        }
    }

    bool ViTriHopLe(Point p) {
        for (int i = 0; i < DoDai; i++)
            if (p.x == A[i].x && p.y == A[i].y)
                return false;
        for (const Food &food : foods)
            if (p.x == food.pos.x && p.y == food.pos.y)
                return false;
        for (const Point &obs : obstacles)
            if (p.x == obs.x && p.y == obs.y)
                return false;
        return true;
    }

    void TaoMoi() {
        foods.clear();
        Food normal;
        do {
            normal.pos.x = rand() % (WIDTH - 2) + 1;
            normal.pos.y = rand() % (HEIGHT - 2) + 1;
        } while (!ViTriHopLe(normal.pos));
        normal.type = 0;
        normal.symbol = '*';
        normal.color = 12;
        normal.points = 10;
        foods.push_back(normal);

        if (rand() % 100 < 30) {
            Food special;
            do {
                special.pos.x = rand() % (WIDTH - 2) + 1;
                special.pos.y = rand() % (HEIGHT - 2) + 1;
            } while (!ViTriHopLe(special.pos));

            int specialType = rand() % 3 + 1;
            special.type = specialType;
            switch (specialType) {
                case 1:
                    special.symbol = '>';
                    special.color = 14;
                    special.points = 15;
                    break;
                case 2:
                    special.symbol = '<';
                    special.color = 11;
                    special.points = 15;
                    break;
                case 3:
                    special.symbol = '$';
                    special.color = 13;
                    special.points = 30;
                    break;
            }
            foods.push_back(special);
        }
    }

    bool KiemTraVaCham() {
        if (A[0].x == 0 || A[0].x == WIDTH || A[0].y == 0 || A[0].y == HEIGHT)
            return true;

        for (int i = 1; i < DoDai; i++)
            if (A[0].x == A[i].x && A[0].y == A[i].y)
                return true;

        for (const Point &obstacle : obstacles)
            if (A[0].x == obstacle.x && A[0].y == obstacle.y)
                return true;

        return false;
    }

    void VeTuong() {
        setColor(11);
        for (int i = 0; i <= WIDTH; i++) {
            gotoxy(i, 0); cout << "#";
            gotoxy(i, HEIGHT); cout << "#";
        }
        for (int i = 0; i <= HEIGHT; i++) {
            gotoxy(0, i); cout << "#";
            gotoxy(WIDTH, i); cout << "#";
        }
    }

    void VeThongTin() {
        gotoxy(WIDTH + 2, 2); setColor(14); cout << "Diem: " << diem;
        gotoxy(WIDTH + 2, 3); setColor(10); cout << "Level: " << level;
        gotoxy(WIDTH + 2, 4); setColor(11); cout << "Toc do: " << (1000 - speed) / 100;

        gotoxy(WIDTH + 2, 6); setColor(15); cout << "Huong dan:";
        gotoxy(WIDTH + 2, 7); cout << "W,A,S,D: Di chuyen";
        gotoxy(WIDTH + 2, 8); cout << "Q: Thoat";
        gotoxy(WIDTH + 2, 9); cout << "P: Tam dung";

        gotoxy(WIDTH + 2, 11); setColor(12); cout << "* : +10 diem";
        gotoxy(WIDTH + 2, 12); setColor(14); cout << "> : Tang toc";
        gotoxy(WIDTH + 2, 13); setColor(11); cout << "< : Giam toc";
        gotoxy(WIDTH + 2, 14); setColor(13); cout << "$ : +30 diem";
    }

    void Ve() {
        gotoxy(A[0].x, A[0].y); setColor(10); cout << "O";
        for (int i = 1; i < DoDai; i++) {
            gotoxy(A[i].x, A[i].y);
            setColor(2); cout << "o";
        }

        for (const Food &food : foods) {
            gotoxy(food.pos.x, food.pos.y);
            setColor(food.color);
            cout << food.symbol;
        }

        for (const Point &obstacle : obstacles) {
            gotoxy(obstacle.x, obstacle.y);
            setColor(8);
            cout << "X";
        }

        VeTuong();
        VeThongTin();
    }

    void DiChuyen(int Huong) {
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];

        if (Huong == 0) A[0].x++;
        if (Huong == 1) A[0].y++;
        if (Huong == 2) A[0].x--;
        if (Huong == 3) A[0].y--;

        for (size_t i = 0; i < foods.size(); i++) {
            if (A[0].x == foods[i].pos.x && A[0].y == foods[i].pos.y) {
                DoDai++;
                diem += foods[i].points;

                switch (foods[i].type) {
                    case 1: speed = max(50, speed - 20); break;
                    case 2: speed = min(150, speed + 20); break;
                }

                if (diem >= level * 100) {
                    level++;
                    TaoVatCan();
                }

                foods.erase(foods.begin() + i);
                if (foods.empty()) TaoMoi();
                break;
            }
        }

        if (KiemTraVaCham())
            gameover = true;
    }
};

int main() {
    srand(time(NULL));
    CONRAN r;
    int Huong = 0, HuongCu = 0;
    char t;
    bool isPaused = false;

    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

    while (!r.gameover) {
        if (kbhit()) {
            t = getch();
            if ((t == 'a' || t == 'A') && HuongCu != 0) Huong = 2;
            if ((t == 'w' || t == 'W') && HuongCu != 1) Huong = 3;
            if ((t == 'd' || t == 'D') && HuongCu != 2) Huong = 0;
            if ((t == 's' || t == 'S') && HuongCu != 3) Huong = 1;
            if (t == 'q' || t == 'Q') break;
            if (t == 'p' || t == 'P') isPaused = !isPaused;
        }

        if (!isPaused) {
            system("cls");
            r.Ve();
            r.DiChuyen(Huong);
            HuongCu = Huong;
            Sleep(r.speed);
        }
    }

    system("cls");
    gotoxy(WIDTH / 2 - 5, HEIGHT / 2); setColor(12); cout << "GAME OVER!";
    gotoxy(WIDTH / 2 - 7, HEIGHT / 2 + 1); setColor(14); cout << "Diem so: " << r.diem;
    gotoxy(WIDTH / 2 - 7, HEIGHT / 2 + 2); setColor(10); cout << "Level: " << r.level;
    gotoxy(WIDTH / 2 - 12, HEIGHT / 2 + 3); setColor(15); cout << "Nhan phim bat ky de thoat";
    _getch();

    return 0;
}

void gotoxy(int column, int line) {
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
