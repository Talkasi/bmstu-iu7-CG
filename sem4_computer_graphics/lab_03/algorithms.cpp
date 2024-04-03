#include "mainwindow.h"
#include <QtMath>
#include <QDebug>
#include <QColorDialog>
#include <QPainterPath>


/*
 * TODO:
 * - Stears Bresenham
 * - By
 *
 * - Сравнения по времени и ступенчатости (гистограммы)
 * - Изменение цвета фона
 *
 * - Отрисовка пикселей
 */

void MainWindow::draw_pixel(int x, int y, QPen &Pen, QBrush &Brush)
{
    // qDebug() << x << y;
    Scene->addRect({(qreal)x, (qreal)y, 1, 1}, Pen, Brush);
}


int sign(int n) {
    if (n == 0)
        return 0;
    return (n >= 0) ? 1 : -1;
}


void swapPoints(QPoint &a, QPoint &b) {
    QPoint tmp = a;
    a = b;
    b = tmp;
}


void swapPoints(QPointF &a, QPointF &b) {
    QPointF tmp = a;
    a = b;
    b = tmp;
}


void MainWindow::draw_line_lib_alg(QLineF line)
{
    QPen Pen = QPen(LineColor, NPixels, Qt::SolidLine);
    Scene->addLine(line, Pen);
    Scene->update();
}


void MainWindow::draw_line_dda(QPointF Start, QPointF End)
{
    assert(Start != End);

    QPen Pen = Qt::NoPen;
    QBrush Brush = QBrush(LineColor);

    qreal deltaX = End.x() - Start.x();
    qreal deltaY = End.y() - Start.y();

    qreal nSteps = qMax(qFabs(deltaX), qFabs(deltaY));

    qreal dx = deltaX / nSteps;
    qreal dy = deltaY / nSteps;

    qreal x = Start.x();
    qreal y = Start.y();

    for (int i = 0; i <= nSteps; ++i) {
        draw_pixel(qRound(x), qRound(y), Pen, Brush);
        x += dx;
        y += dy;
    }
}


#if 0
void MainWindow::draw_line_bresenhams_int(QPoint Start, QPoint End)
{
    assert(Start != End);

    QPen Pen = Qt::NoPen;
    QBrush Brush = QBrush(LineColor);

    if (Start.x() > End.x())
        swapPoints(Start, End);

    int x = Start.x();
    int y = Start.y();

    int dx = End.x() - Start.x();
    int dy = End.y() - Start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    qreal err = 0;
    draw_pixel(x, y, Pen, Brush);

    if (dx > EPS) {
        qreal k = qFabs((qreal)dy) / dx;
        for (x += sx; x <= End.x(); x += sx) {
            err += k;
            if (err >= 0.5) {
                y += sy;
                err = err - 1;
            }
            draw_pixel(x, y, Pen, Brush);
        }
    } else {
        qreal k = dx / qFabs((qreal)dy) ;
        for (y += sy; y <= End.y(); y += sy) {
            err += k;
            if (err >= 0.5) {
                x += sx;
                err = err - 1;
            }
            draw_pixel(x, y, Pen, Brush);
        }
    }
}

#else
void MainWindow::draw_line_bresenhams_int(QPoint Start, QPoint End)
{
    assert(Start != End);
    QPen Pen = Qt::NoPen;
    QBrush Brush = QBrush(LineColor);

    int x = Start.x();
    int y = Start.y();
    int dx = End.x() - Start.x();
    int dy = End.y() - Start.y();

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
        draw_pixel(x, y, Pen, Brush);
        if (err >= 0) {
            if (exchange == 0)
                y += sy;
            else
                x += sx;
            err -= 2 * dx;
        }
        if (err <= 0) {
            if (exchange == 0)
                x += sx;
            else
                y += sy;
            err += 2 * dy;
        }
    }
}
#endif

void MainWindow::draw_line_bresenhams_double(QPointF Start, QPointF End)
{
    assert(Start != End);
    QPen Pen = Qt::NoPen;
    QBrush Brush = QBrush(LineColor);

    qreal x = Start.x();
    qreal y = Start.y();
    qreal dx = End.x() - Start.x();
    qreal dy = End.y() - Start.y();

    qreal sx = sign(dx);
    qreal sy = sign(dy);

    dx = qFabs(dx);
    dy = qFabs(dy);

    int exchange = 0;
    if (dy - dx > EPS) {
        exchange = 1;
        qreal tmp = dx;
        dx = dy;
        dy = tmp;
    }

    qreal k = dy / dx;
    qreal err = k - 0.5;
    int rounded_dx = qRound(dx);
    for (int i = 0; i <= rounded_dx; ++i) {
        draw_pixel(x, y, Pen, Brush);
        if (err >= -EPS) {
            if (exchange == 0)
                y += sy;
            else
                x += sx;
            err -= 1;
        }
        if (err <= EPS) {
            if (exchange == 0)
                x += sx;
            else
                y += sy;
            err += k;
        }
    }
}


void MainWindow::draw_line_bresenhams_step_elimination(QPointF Start, QPointF End)
{

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

void rotate_point(QPoint &point, QPoint center_point, qreal angle)
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


void MainWindow::draw_sun_lib_alg(qreal line_length, qreal angle)
{
    qreal x_center = Scene->sceneRect().width() / 2;
    qreal y_center = Scene->sceneRect().height() / 2;

    QPointF Start = {x_center, y_center};
    QPointF End = {x_center, y_center - line_length};
    for (qreal angle_i = 0; angle_i < 2 * M_PI; angle_i += angle) {
        draw_line_lib_alg({Start, End});
        rotate_point(End, Start, angle);
    }
}


void MainWindow::draw_sun_dda(qreal line_length, qreal angle)
{
    qreal x_center = Scene->sceneRect().width() / 2;
    qreal y_center = Scene->sceneRect().height() / 2;

    QPointF Start = {x_center, y_center};
    QPointF End = {x_center, y_center - line_length};
    for (qreal angle_i = 0; angle_i < 2 * M_PI; angle_i += angle) {
        draw_line_dda(Start, End);
        rotate_point(End, Start, angle);
    }
}


void MainWindow::draw_sun_bresenhams_int(qreal line_length, qreal angle)
{
    qreal x_center = Scene->sceneRect().width() / 2;
    qreal y_center = Scene->sceneRect().height() / 2;

    QPointF StartF = {x_center, y_center};
    QPointF EndF = {x_center, y_center - line_length};

    QPoint Start = {qRound(StartF.x()), qRound(StartF.y())};
    QPoint End = {qRound(EndF.x()), qRound(EndF.y())};
    for (qreal angle_i = 0; angle_i < 2 * M_PI; angle_i += angle) {
        End.setX(qRound(EndF.x()));
        End.setY(qRound(EndF.y()));
        draw_line_bresenhams_int(Start, End);
        rotate_point(EndF, StartF, angle);
    }
}


void MainWindow::draw_sun_bresenhams_double(qreal line_length, qreal angle)
{
    qreal x_center = Scene->sceneRect().width() / 2;
    qreal y_center = Scene->sceneRect().height() / 2;

    QPointF Start = {x_center, y_center};
    QPointF End = {x_center, y_center - line_length};
    for (qreal angle_i = 0; angle_i - 2 * M_PI < EPS; angle_i += angle) {
        draw_line_bresenhams_double(Start, End);
        rotate_point(End, Start, angle);
    }
}


