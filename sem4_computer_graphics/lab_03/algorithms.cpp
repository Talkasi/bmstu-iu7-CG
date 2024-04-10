#include "mainwindow.h"
#include "algorithms.h"
#include <QtMath>
#include <QDebug>
#include <QColorDialog>
#include <QPainterPath>


/*
 * TODO:
 * - Check input (0, -)
 * - Errors cleaning
 * - Do smth with border radius
 */

void draw_pixel(QPainter &painter, int x, int y, qreal alpha)
{
    QColor color = painter.pen().color();
    color.setAlpha(alpha);
    painter.setPen(color);
    painter.drawPoint(x, y);
}


static int sign(int n) {
    if (n == 0)
        return 0;
    return (n >= 0) ? 1 : -1;
}


static void swapPoints(QPoint &a, QPoint &b) {
    QPoint tmp = a;
    a = b;
    b = tmp;
}

static void swapPoints(QPointF &a, QPointF &b) {
    QPointF tmp = a;
    a = b;
    b = tmp;
}


static int areEqual(QPointF &A, QPointF &B)
{
    return (qFabs(A.x() - B.x()) < EPS && qFabs(A.y() - B.y()) < EPS);
}

static int areEqual(QPoint &A, QPoint &B)
{
    return (A.x() == B.x() && A.y() == B.y());
}


int draw_line_lib(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    painter.drawLine(Start, End);
    return 0;
}


int draw_line_dda(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    if (areEqual(Start, End)) {
        int x = qRound(Start.x());
        int y = qRound(Start.y());

        if (mode == DRAW)
            painter.drawPoint(x, y);

        return 1;
    }

    qreal deltaX = End.x() - Start.x();
    qreal deltaY = End.y() - Start.y();

    int nSteps = qRound(qMax(qFabs(deltaX), qFabs(deltaY)));

    qreal dx = (qreal) deltaX / (qreal) nSteps;
    qreal dy = (qreal) deltaY / (qreal) nSteps;

    qreal fx = Start.x();
    qreal fy = Start.y();

    int n_stairs = 1;
    for (int i = 0; i <= nSteps; ++i) {
        int x = qRound(fx);
        int y = qRound(fy);

        if (mode == DRAW)
            painter.drawPoint(x, y);

        if (mode == STAIRS) {
            if (i + 1 <= nSteps && x != qRound(fx + dx) && y != qRound(fy + dy))
                ++n_stairs;
        }

        fx += dx;
        fy += dy;
    }

    return n_stairs;
}


int draw_line_bresenham_i(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
        if (mode == DRAW)
            painter.drawPoint(x, y);

        return 1;
    }

    int dx = qRound(End.x() - Start.x());
    int dy = qRound(End.y() - Start.y());

    int sx = sign(dx);
    int sy = sign(dy);

    dx = qAbs(dx);
    dy = qAbs(dy);

    int exchange = 0;
    if (dy > dx) {
        exchange = 1;
        int tmp = dx;
        dx = dy;
        dy = tmp;
    }

    int err = 2 * dy - dx;

    int n_stairs = 1;
    int x_prev = x;
    int y_prev = y;
    for (int i = 0; i <= dx; ++i) {
        if (mode == DRAW)
            painter.drawPoint(x, y);

        if (err >= 0) {
            if (exchange)
                x += sx;
            else
                y += sy;
            err -= 2 * dx;
        }
        if (err <= 0) {
            if (exchange)
                y += sy;
            else
                x += sx;
            err += 2 * dy;
        }

        if (mode == STAIRS) {
            if (i + 1 <= dx && x != x_prev && y != y_prev)
                ++n_stairs;

            x_prev = x;
            y_prev = y;
        }
    }

    return n_stairs;
}


int draw_line_bresenham_d(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
        if (mode == DRAW)
            painter.drawPoint(x, y);

        return 1;
    }

    qreal dx = End.x() - Start.x();
    qreal dy = End.y() - Start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = qFabs(dx);
    dy = qFabs(dy);

    int exchange = 0;
    if (dy > dx) {
        exchange = 1;
        qreal tmp = dx;
        dx = dy;
        dy = tmp;
    }

    qreal k = dy / dx;
    qreal err = k - 0.5;

    int n_stairs = 1;
    int x_prev = x;
    int y_prev = y;
    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
        if (mode == DRAW)
            painter.drawPoint(x, y);

        if (err >= 0) {
            if (exchange)
                x += sx;
            else
                y += sy;
            err -= 1;
        }
        if (err <= 0) {
            if (exchange)
                y += sy;
            else
                x += sx;
            err += k;
        }


        if (mode == STAIRS) {
            if (i + 1 <= dx && x != x_prev && y != y_prev)
                ++n_stairs;

            x_prev = x;
            y_prev = y;
        }
    }

    return n_stairs;
}


int draw_line_bresenham_antialiasing(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
        if (mode == DRAW)
            painter.drawPoint(x, y);

        return 1;
    }

    qreal dx = End.x() - Start.x();
    qreal dy = End.y() - Start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = qFabs(dx);
    dy = qFabs(dy);

    int exchange = 0;
    if (dy > dx) {
        exchange = 1;
        qreal tmp = dx;
        dx = dy;
        dy = tmp;
    }

    qreal m = dy / dx;
    qreal e = 0.5;
    qreal w = 1.0 - m;
    qreal gradient;

    int n_stairs = 1;
    int x_prev = x;
    int y_prev = y;
    int rounded_dx = dx;
    for (int i = 0; i <= rounded_dx; ++i)
    {
        gradient = 255 * e;
        if (mode == DRAW)
            draw_pixel(painter, x, y, gradient);

        if (e < w) {
            if (exchange == 0)
                x += sx;
            else
                y += sy;

            e += m;
        } else {
            x += sx;
            y += sy;
            e -= w;
        }


        if (mode == STAIRS) {
            if (i + 1 <= dx && x != x_prev && y != y_prev)
                ++n_stairs;

            x_prev = x;
            y_prev = y;
        }
    }

    return n_stairs;
}


static qreal fPart(qreal x) {
    return x - qFloor(x);
}

int draw_line_Wu(QPainter &painter, QPointF Start, QPointF End, alg_mode mode)
{
    if (areEqual(Start, End)) {
        int x = qRound(Start.x());
        int y = qRound(Start.y());

        if (mode == DRAW)
            painter.drawPoint(x, y);

        return 1;
    }

    qreal x0 = Start.x();
    qreal y0 = Start.y();
    qreal x1 = End.x();
    qreal y1 = End.y();

    int steep = qFabs(y1 - y0) > qFabs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    qreal dx = x1 - x0;
    qreal dy = y1 - y0;

    qreal gradient = 1;
    if (dx != 0)
        gradient = dy / dx;

    qreal xEnd = qRound(x0);
    qreal yEnd = y0 + gradient * (xEnd - x0);
    qreal xGap = 1 - fPart(x0 + 0.5);

    int xPxl1 = xEnd;
    int yPxl1 = qFloor(yEnd);

    int n_stairs = 1;

    qreal first_intencity = 255 * (1 - fPart(yEnd)) * xGap;
    qreal second_intencity = 255 * fPart(yEnd) * xGap;
    if (steep) {
        if (mode == DRAW) {
            draw_pixel(painter, yPxl1, xPxl1, first_intencity);
            draw_pixel(painter, yPxl1 + 1, xPxl1, second_intencity);
        }
    } else {
        if (mode == DRAW) {
            draw_pixel(painter, xPxl1, yPxl1, first_intencity);
            draw_pixel(painter, xPxl1, yPxl1 + 1, second_intencity);
        }
    }

    qreal intery = yEnd + gradient;

    xEnd = qRound(x1);
    yEnd = y1 + gradient * (xEnd - x1);
    xGap = fPart(x1 + 0.5);

    int xPxl2 = xEnd;
    int yPxl2 = qFloor(yEnd);

    first_intencity = 255 * (1 - fPart(yEnd)) * xGap;
    second_intencity = 255 * fPart(yEnd) * xGap;
    if (steep) {
        if (mode == DRAW) {
            draw_pixel(painter, yPxl2, xPxl2, first_intencity);
            draw_pixel(painter, yPxl2 + 1, xPxl2, second_intencity);
        }
    } else {
        if (mode == DRAW) {
            draw_pixel(painter, xPxl2, yPxl2, first_intencity);
            draw_pixel(painter, xPxl2, yPxl2 + 1, second_intencity);
        }
    }

    if (steep) {
        int prev_first_coord = yPxl1 + 1;
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            int floored_intery = qFloor(intery);

            int cur_first_coord1 = floored_intery;
            int cur_first_coord2 = floored_intery + 1;

            first_intencity = 255 * (1 - fPart(intery));
            second_intencity = 255 * fPart(intery);

            if (mode == DRAW) {
                draw_pixel(painter, cur_first_coord1, x, first_intencity);
                draw_pixel(painter, cur_first_coord2, x, second_intencity);
            }

            if (mode == STAIRS) {
                if (cur_first_coord1 != prev_first_coord)
                    ++n_stairs;

                prev_first_coord = cur_first_coord1;
            }

            intery = intery + gradient;
        }
    } else {
        int prev_second_coord = xPxl1 + 1;
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            int floored_intery = qFloor(intery);

            int cur_second_coord1 = floored_intery;
            int cur_second_coord2 = floored_intery + 1;

            first_intencity = 255 * (1 - fPart(intery));
            second_intencity = 255 * fPart(intery);

            if (mode == DRAW) {
                draw_pixel(painter, x, cur_second_coord1, first_intencity);
                draw_pixel(painter, x, cur_second_coord2, second_intencity);
            }

            if (mode == STAIRS) {
                if (cur_second_coord1 != prev_second_coord)
                    ++n_stairs;

                prev_second_coord = cur_second_coord1;
            }

            intery = intery + gradient;
        }
    }

    return n_stairs;
}


void rotate_point(QPointF &point, QPointF &center_point, qreal angle)
{
    qreal xc = center_point.x();
    qreal yc = center_point.y();

    qreal x = point.x();
    qreal y = point.y();

    qreal NewX = xc + (x - xc) * qCos(angle) + (y - yc) * qSin(angle);
    qreal NewY = yc + (y - yc) * qCos(angle) - (x - xc) * qSin(angle);

    point.setX(NewX);
    point.setY(NewY);
}

void rotate_point(QPoint &point, QPoint &center_point, qreal angle)
{
    qreal xc = center_point.x();
    qreal yc = center_point.y();

    qreal x = point.x();
    qreal y = point.y();

    qreal NewX = xc + (x - xc) * qCos(angle) + (y - yc) * qSin(angle);
    qreal NewY = yc + (y - yc) * qCos(angle) - (x - xc) * qSin(angle);

    point.setX(qRound(NewX));
    point.setY(qRound(NewY));
}


void MainWindow::draw_spectrum(QPainter &painter, qreal line_length, qreal angle, int (*alg)(QPainter &, QPointF, QPointF, alg_mode))
{
    qreal x_center = pxp.width() / 2;
    qreal y_center = pxp.height() / 2;

    QPointF Start = {x_center, y_center};
    QPointF End = {x_center, y_center - line_length};

    int n_steps = qFloor(2 * M_PI / angle);
    for (int i = 0; i <= n_steps; ++i) {
        alg(painter, Start, End, DRAW);
        rotate_point(End, Start, angle);
    }
}


// void MainWindow::draw_spectrum(QPainter &painter, qreal line_length, qreal angle, int (*alg)(QPainter &, QPoint, QPoint, alg_mode))
// {
//     qreal x_center = pxp.width() / 2;
//     qreal y_center = pxp.height() / 2;

//     QPointF StartF = {x_center, y_center};
//     QPointF EndF = {x_center, y_center - line_length};

//     QPoint Start = {qRound(StartF.x()), qRound(StartF.y())};
//     QPoint End = {qRound(EndF.x()), qRound(EndF.y())};
//     int n_steps = qFloor(2 * M_PI / angle);
//     for (int i = 0; i <= n_steps; ++i) {
//         End.setX(qRound(EndF.x()));
//         End.setY(qRound(EndF.y()));
//         alg(painter, Start, End, DRAW);
//         rotate_point(EndF, StartF, angle);
//     }
// }

