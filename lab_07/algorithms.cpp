#include "algorithms.h"

#define LEFT 1   /* двоичное 0001 */
#define RIGHT 2  /* двоичное 0010 */
#define BOTTOM 4 /* двоичное 0100 */
#define TOP 8    /* двоичное 1000 */

/* вычисление кода точки */
int vcode(const QRect &r, const QPoint &p)
{
    QPoint min_p = r.topLeft();
    QPoint max_p = r.bottomRight();

    int code = 0;

    code += p.x() < min_p.x() ? LEFT : 0;   /* +1 если точка левее прямоугольника */
    code += p.x() > max_p.x() ? RIGHT : 0;  /* +2 если точка правее прямоугольника */
    code += p.y() < min_p.y() ? BOTTOM : 0; /* +4 если точка ниже прямоугольника */
    code += p.y() > max_p.y() ? TOP : 0;    /* +8 если точка выше прямоугольника */

    return code;
}

/* если отрезок ab не пересекает прямоугольник r, функция возвращает -1;
   если отрезок ab пересекает прямоугольник r, функция возвращает 0 и отсекает
   те части отрезка, которые находятся вне прямоугольника */
int cohen_sutherland(const QRect &r, QPoint &a, QPoint &b)
{
    int code_a, code_b, code; /* код концов отрезка */
    QPoint c;                 /* одна из точек */

    code_a = vcode(r, a);
    code_b = vcode(r, b);

    /* пока одна из точек отрезка вне прямоугольника */
    while (code_a | code_b) {
        /* если обе точки с одной стороны прямоугольника, то отрезок не пересекает прямоугольник */
        if (code_a & code_b)
            return -1;

        /* выбираем точку c с ненулевым кодом */
        if (code_a) {
            code = code_a;
            c = a;
        }
        else {
            code = code_b;
            c = b;
        }

        /* если c левее r, то передвигаем c на прямую x = r->x_min
           если c правее r, то передвигаем c на прямую x = r->x_max */
        if (code & LEFT) {
            // c.y += (a.y - b.y) * (r.x_min - c.x) / (a.x - b.x);
            // c.x = r.x_min;
            c.setY(c.y() + (a.y() - b.y()) * (r.topLeft().x() - c.x()) / (a.x() - b.x()));
            c.setX(r.topLeft().x());
        }
        else if (code & RIGHT) {
            // c.y += (a.y - b.y) * (r.x_max - c.x) / (a.x - b.x);
            // c.x = r.x_max;
            c.setY(c.y() + (a.y() - b.y()) * (r.bottomRight().x() - c.x()) / (a.x() - b.x()));
            c.setX(r.bottomRight().x());
        } /* если c ниже r, то передвигаем c на прямую y = r.y_min
              если c выше r, то передвигаем c на прямую y = r.y_max */
        else if (code & BOTTOM) {
            // c.x += (a.x - b.x) * (r.y_min - c.y) / (a.y - b.y);
            // c.y = r.y_min;
            c.setX(c.x() + (a.x() - b.x()) * (r.topLeft().y() - c.y()) / (a.y() - b.y()));
            c.setY(r.topLeft().y());
        }
        else if (code & TOP) {
            // c.x += (a.x - b.x) * (r.y_max - c.y) / (a.y - b.y);
            // c.y = r.y_max;
            c.setX(c.x() + (a.x() - b.x()) * (r.bottomRight().y() - c.y()) / (a.y() - b.y()));
            c.setY(r.bottomRight().y());
        }

        /* обновляем код */
        if (code == code_a) {
            /* a = c */
            code_a = vcode(r, a);
        }
        else {
            /* b = c */
            code_b = vcode(r, b);
        }
    }

    /* оба кода равны 0, следовательно обе точки в прямоугольнике */
    return 0;
}
