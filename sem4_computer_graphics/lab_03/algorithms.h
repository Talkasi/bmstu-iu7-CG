#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <QPainter>


int draw_line_lib(QPainter &painter, QPointF Start, QPointF End);
int draw_line_dda(QPainter &painter, QPointF Start, QPointF End);
int draw_line_bresenham_i(QPainter &painter, QPointF Start, QPointF End);
int draw_line_bresenham_d(QPainter &painter, QPointF Start, QPointF End);
int draw_line_bresenham_antialiasing(QPainter &painter, QPointF Start, QPointF End);
int draw_line_Wu(QPainter &painter, QPointF Start, QPointF End);

int count_stairs_dda(QPainter &painter, QPointF Start, QPointF End);
int count_stairs_bresenham_i(QPainter &painter, QPointF Start, QPointF End);
int count_stairs_bresenham_d(QPainter &painter, QPointF Start, QPointF End);
int count_stairs_bresenham_antialiasing(QPainter &painter, QPointF Start, QPointF End);
int count_stairs_Wu(QPainter &painter, QPointF Start, QPointF End);


void rotate_point(QPointF &point, QPointF &center_point, qreal angle);
void rotate_point(QPoint &point, QPoint &center_point, qreal angle);

#endif // ALGORITHMS_H
