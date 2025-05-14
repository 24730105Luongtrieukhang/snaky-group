#include <iostream>
#include <windows.h>
#include <cstdlib>
#include <conio.h>
#include <time.h>
using namespace std;

#define WIDTH 50
#define HEIGHT 20

void gotoxy(int column, int line);
void setColor(int color);

struct Point
{
    int x, y;
};

class CONRAN
{
public:
    struct Point A[100];
    struct Point moi;
    int DoDai;
    int diem;
    bool gameover;

    CONRAN()
    {
        DoDai = 3;
        A[0].x = 10;
        A[0].y = 10;
        A[1].x = 11;
        A[1].y = 10;
        A[2].x = 12;
        A[2].y = 10;
        TaoMoi();
        diem = 0;
        gameover = false;
    }

    void TaoMoi()
    {
        srand(time(NULL));
        moi.x = rand() % (WIDTH - 2) + 1;
        moi.y = rand() % (HEIGHT - 2) + 1;
    }

    bool KiemTraVaCham()
    {
        // Kiểm tra va chạm với tường
        if (A[0].x == 0 || A[0].x == WIDTH || A[0].y == 0 || A[0].y == HEIGHT)
            return true;

        // Kiểm tra va chạm với thân rắn
        for (int i = 1; i < DoDai; i++)
        {
            if (A[0].x == A[i].x && A[0].y == A[i].y)
                return true;
        }
        return false;
    }

    void VeTuong()
    {
        setColor(11); // Màu xanh da trời cho tường
        for (int i = 0; i <= WIDTH; i++)
        {
            gotoxy(i, 0);
            cout << "#";
            gotoxy(i, HEIGHT);
            cout << "#";
        }
        for (int i = 0; i <= HEIGHT; i++)
        {
            gotoxy(0, i);
            cout << "#";
            gotoxy(WIDTH, i);
            cout << "#";
        }
    }

    void VeDiem()
    {
        gotoxy(WIDTH + 2, 2);
        setColor(14); // Màu vàng cho điểm
        cout << "Diem: " << diem;
    }

    void Ve()
    {
        // Vẽ đầu rắn
        gotoxy(A[0].x, A[0].y);
        setColor(10); // Màu xanh lá cho đầu rắn
        cout << "O";

        // Vẽ thân rắn
        for (int i = 1; i < DoDai; i++)
        {
            gotoxy(A[i].x, A[i].y);
            setColor(2); // Màu xanh lá đậm cho thân rắn
            cout << "o";
        }

        // Vẽ mồi
        gotoxy(moi.x, moi.y);
        setColor(12); // Màu đỏ cho mồi
        cout << "*";

        VeTuong();
        VeDiem();
    }

    void DiChuyen(int Huong)
    {
        for (int i = DoDai - 1; i > 0; i--)
            A[i] = A[i - 1];

        if (Huong == 0)
            A[0].x = A[0].x + 1;
        if (Huong == 1)
            A[0].y = A[0].y + 1;
        if (Huong == 2)
            A[0].x = A[0].x - 1;
        if (Huong == 3)
            A[0].y = A[0].y - 1;

        // Kiểm tra ăn mồi
        if (A[0].x == moi.x && A[0].y == moi.y)
        {
            DoDai++;
            diem += 10;
            TaoMoi();
        }

        // Kiểm tra va chạm
        if (KiemTraVaCham())
        {
            gameover = true;
        }
    }
};

int main()
{
    CONRAN r;
    int Huong = 0;
    char t;

    // Ẩn con trỏ
    CONSOLE_CURSOR_INFO cursor;
    cursor.dwSize = 100;
    cursor.bVisible = false;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor);

    while (!r.gameover)
    {
        if (kbhit())
        {
            t = getch();
            if (t == 'a')
                Huong = 2;
            if (t == 'w')
                Huong = 3;
            if (t == 'd')
                Huong = 0;
            if (t == 'x')
                Huong = 1;
            if (t == 'q')
                break;
        }
        system("cls");
        r.Ve();
        r.DiChuyen(Huong);
        Sleep(100);
    }

    // Hiển thị màn hình Game Over
    system("cls");
    gotoxy(WIDTH / 2 - 5, HEIGHT / 2);
    setColor(12);
    cout << "GAME OVER!";
    gotoxy(WIDTH / 2 - 7, HEIGHT / 2 + 1);
    setColor(14);
    cout << "Diem so: " << r.diem;
    gotoxy(WIDTH / 2 - 12, HEIGHT / 2 + 2);
    setColor(15);
    cout << "Nhan phim bat ky de thoat";
    _getch();

    return 0;
}

void gotoxy(int column, int line)
{
    COORD coord;
    coord.X = column;
    coord.Y = line;
    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        coord);
}

void setColor(int color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
