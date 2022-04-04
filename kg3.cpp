// треугольник 
// + создать
// + цвет границ
// + заливка
// + подписать
// + связать с клавой
// + переместить
// + повернуть
// + масштабировать

#include <iostream>
#include <math.h>
#include <queue>
#include "graphics.h"
#pragma comment(lib,"graphics.lib")

#define P 3.14
#define TEXTCOL 13 //красный 12 белый 15 маждента 13 циан 11 жёлтый 14
#define MAINCOL 11 
#define FILLCOL 14
#define BLACK 0

using namespace std;


// отрисовка линии попиксельно
void line_DDA(float x1, float y1, float x2, float y2, COLORREF cColor) {
    // Целочисленные значения координат начала и конца отрезка, округлённые до ближайшего целого
    int iX1 = roundf(x1);
    int iY1 = roundf(y1);
    int iX2 = roundf(x2);
    int iY2 = roundf(y2);

    // Длина и высота линии
    int deltaX = abs(iX1 - iX2);
    int deltaY = abs(iY1 - iY2);

    // Считаем минимальное количество итераций, необходимое  для отрисовки отрезка
    // Выбирая максимум из длины и высоты линии, обеспечиваем связность линии
    int length = max(deltaX, deltaY);

    // особый случай, на экране закрашивается ровно один пиксель
    if (length == 0) {
        putpixel(iX1, iY1, cColor);
        return;
    }
    // Вычисляем приращения на каждом шаге по осям абсцисс и ординат
    double dX = (x2 - x1) / length;
    double dY = (y2 - y1) / length;

    // Начальные значения
    double x = x1;
    double y = y1;

    // Основной цикл
    length++;
    while (length--) {
        x += dX;
        y += dY;
        putpixel(roundf(x), roundf(y), cColor);
    }
}


// класс для точек
class Point {
public:
    float x;
    float y;
    char* name;

    void namePoint(char* name) {
        outtextxy(x, y, name);
    }
};


// класс треугольника
class Triangle {
public:
    Point A;
    Point B;
    Point C;
    char name_A[2] = "A";
    char name_B[2] = "B";
    char name_C[2] = "C";
    int col = MAINCOL;

    // конструктор
    Triangle() {
        A.x = 2; A.y = 25;
        A.name = name_A;
        B.x = 40; B.y = 5;
        B.name = name_B;
        C.x = 10; C.y = 5;
        C.name = name_C;
        drawTriangle();
    }

    // отрисовка
    void drawTriangle() {
        setcolor(TEXTCOL);
        A.namePoint(A.name);
        B.namePoint(B.name);
        C.namePoint(C.name);

        line_DDA(A.x, A.y, B.x, B.y, col); // линия 1
        line_DDA(B.x, B.y, C.x, C.y, col); // линия 2
        line_DDA(A.x, A.y, C.x, C.y, col); // линия 3

        // заливка
        Point Cen; // точка центра треугольника
        Cen.x = (A.x + B.x + C.x) / 3;
        Cen.y = (A.y + B.y + C.y) / 3;
        Cen.x = floor(Cen.x);
        Cen.y = floor(Cen.y);
        fill(Cen, FILLCOL);
    }

    // заливка
    void fill(Point S, COLORREF colour) {
        Point pt = S;
        queue <Point> Pts;
        Pts.push(pt);

        while (!Pts.empty()) {
            pt = Pts.front(); 
            Pts.pop();
            if ( getpixel(pt.x, pt.y) != BLACK || pt.x<=0 || pt.y<=0 ) {
                if (getpixel(pt.x, pt.y) == TEXTCOL) {
                    pt.x--;
                    Pts.push(pt);
                }
            } else {
                putpixel(pt.x, pt.y, colour);
                pt.y--;         Pts.push(pt); // вниз
                pt.y++; pt.y++; Pts.push(pt); // вверх
                pt.y--; pt.x++; Pts.push(pt); // вправо
                pt.x--; pt.x--; Pts.push(pt); // влево
            }
        }
    }

    // пермещение
    void moveX(float amt) {
        A.x += amt;
        B.x += amt;
        C.x += amt;
    }
    void moveY(float amt) {
        A.y += amt;
        B.y += amt;
        C.y += amt;
    }

    // поворот
    void rotate(int u) { // u = -1 по часовой, u = 1 против
        Point tmpA = A, tmpB = B, tmpC = C;
        float ang = u * 0.05; // угол поворота

        Point Cen; // точка центра треугольника
        Cen.x = (A.x + B.x + C.x) / 3;
        Cen.y = (A.y + B.y + C.y) / 3;

        A.x = A.x - Cen.x; // расстояние от а до центра по х
        A.y = A.y - Cen.y; // по у
        B.x = B.x - Cen.x; // расстояние от b до центра по х
        B.y = B.y - Cen.y; // по у
        C.x = C.x - Cen.x; // расстояние от c до центра по х
        C.y = C.y - Cen.y; // по у

        // поворот вокруг нк
        // A
        float tmpX = A.x * cos(ang) + A.y * sin(ang);
        float tmpY = -A.x * sin(ang) + A.y * cos(ang);
        A.x = tmpX + Cen.x;
        A.y = tmpY + Cen.y;
        // B
        tmpX = B.x * cos(ang) + B.y * sin(ang);
        tmpY = -B.x * sin(ang) + B.y * cos(ang);
        B.x = tmpX + Cen.x;
        B.y = tmpY + Cen.y;
        // C
        tmpX = C.x * cos(ang) + C.y * sin(ang);
        tmpY = -C.x * sin(ang) + C.y * cos(ang);
        C.x = tmpX + Cen.x;
        C.y = tmpY + Cen.y;
    }

    // масштабирование
    void scale(float e) {
        Point Cen; // точка центра треугольника
        Cen.x = (A.x + B.x + C.x) / 3;
        Cen.y = (A.y + B.y + C.y) / 3;

        if ((abs(A.x - Cen.x) >= 1 && abs(A.y - Cen.y) >= 1) && (abs(B.x - Cen.x) >= 1 && abs(B.y - Cen.y) >= 1) &&
            (abs(C.x - Cen.x) >= 1 && abs(C.y - Cen.y) >= 1) || e > 1) { // предотвращение сжатия в точку
            // A.x
            float xe = (Cen.x + A.x) / 2;
            float lx = Cen.x - A.x;
            lx = lx * e;
            A.x = xe - lx / 2;
            // A.y
            float ye = (Cen.y + A.y) / 2;
            float ly = Cen.y - A.y;
            ly = ly * e;
            A.y = ye - ly / 2;

            // B.x
            xe = (Cen.x + B.x) / 2;
            lx = Cen.x - B.x;
            lx = lx * e;
            B.x = xe - lx / 2;
            // B.y
            ye = (Cen.y + B.y) / 2;
            ly = Cen.y - B.y;
            ly = ly * e;
            B.y = ye - ly / 2;

            // C.x
            xe = (Cen.x + C.x) / 2;
            lx = Cen.x - C.x;
            lx = lx * e;
            C.x = xe - lx / 2;
            // C.y
            ye = (Cen.y + C.y) / 2;
            ly = Cen.y - C.y;
            ly = ly * e;
            C.y = ye - ly / 2;
        }

    }
};



int main() {
    initwindow(1400, 700); // создаём консольное окно 1400 на 700

    Triangle Tri; // создание треугольника

    // управление
    int i = 1; // условие выхода
    while (i) {
        switch (getch()) {
        case 'w':
            cout << 'w' << endl;
            Tri.moveY(-10); // вверх
            break;
        case 'a':
            cout << 'a' << endl;
            Tri.moveX(-10); // влево
            break;
        case 's':
            cout << 's' << endl;
            Tri.moveY(10); // вниз
            break;
        case 'd':
            cout << 'd' << endl;
            Tri.moveX(10); // вправо
            break;
        case 'q':
            cout << 'q' << endl;
            Tri.rotate(1); // против часовой
            break;
        case 'e':
            cout << 'e' << endl;
            Tri.rotate(-1); // по часовой
            break;
        case '=':
            cout << '+' << endl;
            Tri.scale(1.5); // увеличение
            break;
        case '-':
            cout << '-' << endl;
            Tri.scale(0.5); // уменьшение
            break;
        default:
            cout << "default -> exit" << endl;
            i = 0;
            break;
        }
        cleardevice(); // отичстка экрана
        Tri.drawTriangle(); // перерисовка треугольника
    }

    getch(); // чтение одного символа с клавиатуры
    closegraph(); // освобождает всю память, выделенную под графическую систему, затем восстанавливает экран в режим, который был до вызова initwindow

    return 0;
}