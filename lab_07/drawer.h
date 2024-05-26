#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsView>
#include <QPainter>
#include <scene.h>

class Drawer : public QObject
{
public:
    Drawer(QGraphicsView *view, QObject *parent = nullptr);

    void draw_point(QPoint &p);
    void draw_line(QLine &line);
    void draw_line(QPoint &p1, QPoint &p2);
    void draw_rect(QRect &rect);

    QColor get_pixel_color(const QPoint &position) const;
    QColor get_pixel_color(int x, int y) const;

    void clear();
    void render();

    int width();
    int height();
    void maximize();

    Scene *get_scene();

    void onMouseLeftButtonPressed();
    void onMouseRightButtonPressed();

    void set_line_color(QColor &color);
    void set_rect_color(QColor &color);
    void set_result_color(QColor &color);

    QColor get_line_color();
    QColor get_rect_color();
    QColor get_result_color();

private:
    Scene *_scene;
    QGraphicsView *_view;

    QPixmap _pxp;
    QPixmap _rect_pxp;

    int _width;
    int _height;

    int n_lines_points = 0;
    QPoint lines_points[100] = {};

    bool rect_empty = true;
    QRect rect = {};

    QColor line_color = Qt::blue;
    QColor rect_color = Qt::black;
    QColor result_color = Qt::red;
};

#endif // DRAWER_H
