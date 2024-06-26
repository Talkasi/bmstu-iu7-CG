#include "algorithms.h"
#include "mainwindow.h"
#include <QColorDialog>
#include <QDebug>
#include <QPainterPath>
#include <QtMath>

void draw_pixel(QPainter &painter, int x, int y, qreal alpha)
{
    QColor color = painter.pen().color();
    color.setAlpha(alpha);
    painter.setPen(color);
    painter.drawPoint(x, y);
}

static int sign(int n)
{

    if (n == 0)
        return 0;
    return (n >= 0) ? 1 : -1;
}

static void swapPoints(QPoint &a, QPoint &b)
{
    QPoint tmp = a;
    a = b;
    b = tmp;
}

static void swapPoints(QPointF &a, QPointF &b)
{
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

int draw_line_lib(QPainter &painter, QPointF Start, QPointF End)
{
    painter.drawLine(Start, End);
    return 0;
}

int draw_line_dda(QPainter &painter, QPointF Start, QPointF End)
{
    if (areEqual(Start, End)) {
        int x = qRound(Start.x());
        int y = qRound(Start.y());

        painter.drawPoint(x, y);
        return 1;
    }

    qreal deltaX = End.x() - Start.x();
    qreal deltaY = End.y() - Start.y();

    qreal nSteps = qMax(qFabs(deltaX), qFabs(deltaY));

    qreal dx = (qreal)deltaX / (qreal)nSteps;
    qreal dy = (qreal)deltaY / (qreal)nSteps;

    qreal fx = Start.x();
    qreal fy = Start.y();

    int roundedNSteps = qRound(nSteps);
    for (int i = 0; i <= roundedNSteps; ++i) {
        painter.drawPoint(qRound(fx), qRound(fy));

        fx += dx;
        fy += dy;
    }

    return 0;
}

int count_stairs_dda(QPainter &painter, QPointF Start, QPointF End)
{
    if (areEqual(Start, End))
        return 1;

    qreal deltaX = End.x() - Start.x();
    qreal deltaY = End.y() - Start.y();

    qreal nSteps = qMax(qFabs(deltaX), qFabs(deltaY));

    qreal dx = (qreal)deltaX / (qreal)nSteps;
    qreal dy = (qreal)deltaY / (qreal)nSteps;

    qreal fx = Start.x();
    qreal fy = Start.y();

    int n_stairs = 1;
    int roundedNSteps = qRound(nSteps);
    for (int i = 0; i <= roundedNSteps; ++i) {
        if (i + 1 <= roundedNSteps && qRound(fx) != qRound(fx + dx) && qRound(fy) != qRound(fy + dy))
            ++n_stairs;

        fx += dx;
        fy += dy;
    }

    return n_stairs;
}

int draw_line_bresenham_i(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
        painter.drawPoint(x, y);
        return 1;
    }

    int dx = qRound(End.x()) - qRound(Start.x());
    int dy = qRound(End.y()) - qRound(Start.y());

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
    for (int i = 0; i <= dx; ++i) {
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
    }

    return 0;
}

int count_stairs_bresenham_i(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End))
        return 1;

    int dx = qRound(End.x()) - qRound(Start.x());
    int dy = qRound(End.y()) - qRound(Start.y());

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
    for (int i = 0; i <= dx; ++i) {
        if (err >= 0) {
            if (exchange)
                x += sx;
            else
                y += sy;
            err -= 2 * dx;

            if (i + 1 <= dx)
                ++n_stairs;
        }
        if (err <= 0) {
            if (exchange)
                y += sy;
            else
                x += sx;
            err += 2 * dy;
        }
    }

    return n_stairs;
}

int draw_line_bresenham_d(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
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
    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
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
    }

    return 0;
}

int count_stairs_bresenham_d(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End))
        return 1;

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
    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
        if (err >= 0) {
            if (exchange)
                x += sx;
            else
                y += sy;
            err -= 1;

            if (i + 1 <= rounded_dx)
                ++n_stairs;
        }
        if (err <= 0) {
            if (exchange)
                y += sy;
            else
                x += sx;
            err += k;
        }
    }

    return n_stairs;
}

int draw_line_bresenham_antialiasing(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End)) {
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

    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
        draw_pixel(painter, x, y, 255 * e);

        if (e < w) {
            if (exchange)
                y += sy;
            else
                x += sx;

            e += m;
        }
        else {
            x += sx;
            y += sy;
            e -= w;
        }
    }

    return 0;
}

int count_stairs_bresenham_antialiasing(QPainter &painter, QPointF Start, QPointF End)
{
    int x = qRound(Start.x());
    int y = qRound(Start.y());
    if (areEqual(Start, End))
        return 1;

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
    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
        if (e < w) {
            if (exchange)
                y += sy;
            else
                x += sx;

            e += m;
        }
        else {
            x += sx;
            y += sy;
            e -= w;
            if (i + 1 <= rounded_dx)
                ++n_stairs;
        }
    }

    return n_stairs;
}

static qreal fPart(qreal x)
{
    return x - qFloor(x);
}

int draw_line_Wu(QPainter &painter, QPointF Start, QPointF End)
{
    if (areEqual(Start, End)) {
        int x = qRound(Start.x());
        int y = qRound(Start.y());
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

    qreal first_intencity = 255 * (1 - fPart(yEnd)) * xGap;
    qreal second_intencity = 255 * fPart(yEnd) * xGap;
    if (steep) {
        draw_pixel(painter, yPxl1, xPxl1, first_intencity);
        draw_pixel(painter, yPxl1 + 1, xPxl1, second_intencity);
    }
    else {
        draw_pixel(painter, xPxl1, yPxl1, first_intencity);
        draw_pixel(painter, xPxl1, yPxl1 + 1, second_intencity);
    }

    qreal y = yEnd + gradient;

    xEnd = qRound(x1);
    yEnd = y1 + gradient * (xEnd - x1);
    xGap = fPart(x1 + 0.5);

    int xPxl2 = xEnd;
    int yPxl2 = qFloor(yEnd);

    first_intencity = 255 * (1 - fPart(yEnd)) * xGap;
    second_intencity = 255 * fPart(yEnd) * xGap;
    if (steep) {
        draw_pixel(painter, yPxl2, xPxl2, first_intencity);
        draw_pixel(painter, yPxl2 + 1, xPxl2, second_intencity);
    }
    else {
        draw_pixel(painter, xPxl2, yPxl2, first_intencity);
        draw_pixel(painter, xPxl2, yPxl2 + 1, second_intencity);
    }

    if (steep) {
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            draw_pixel(painter, qFloor(y), x, 255 * (1 - fPart(y)));
            draw_pixel(painter, qFloor(y) + 1, x, 255 * fPart(y));
            y = y + gradient;
        }
    }
    else {
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            draw_pixel(painter, x, qFloor(y), 255 * (1 - fPart(y)));
            draw_pixel(painter, x, qFloor(y) + 1, 255 * fPart(y));
            y = y + gradient;
        }
    }

    return 0;
}

int count_stairs_Wu(QPainter &painter, QPointF Start, QPointF End)
{
    if (areEqual(Start, End))
        return 1;

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

    int xPxl1 = xEnd;
    int yPxl1 = qFloor(yEnd);

    int n_stairs = 1;
    qreal intery = yEnd + gradient;

    xEnd = qRound(x1);
    int xPxl2 = xEnd;
    if (steep) {
        int prev_first_coord = yPxl1 + 1;
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            int cur_first_coord1 = qFloor(intery);

            if (cur_first_coord1 != prev_first_coord)
                ++n_stairs;

            prev_first_coord = cur_first_coord1;
            intery = intery + gradient;
        }
    }
    else {
        int prev_second_coord = xPxl1 + 1;
        for (int x = xPxl1 + 1; x <= xPxl2; ++x) {
            int cur_second_coord1 = qFloor(intery);

            if (cur_second_coord1 != prev_second_coord)
                ++n_stairs;

            prev_second_coord = cur_second_coord1;
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

void MainWindow::draw_spectrum(QPainter &painter, qreal line_length, qreal angle,
                               int (*alg)(QPainter &, QPointF, QPointF))
{
    qreal x_center = pxp.width() / 2;
    qreal y_center = pxp.height() / 2;

    QPointF Start = {x_center, y_center};
    QPointF End = {x_center, y_center - line_length};

    int n_steps = qFloor(2 * M_PI / angle);
    for (int i = 0; i <= n_steps; ++i) {
        alg(painter, Start, End);
        rotate_point(End, Start, angle);
    }
}

// void MainWindow::draw_spectrum(QPainter &painter, qreal line_length, qreal angle, int (*alg)(QPainter &, QPoint,
// QPoint, alg_mode))
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
