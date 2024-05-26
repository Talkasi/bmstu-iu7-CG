#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsView>
#include <QPainter>
#include <scene.h>

class Drawer
{
public:
    Drawer(QGraphicsView *view, QObject *parent = nullptr);

    void draw_point(QPoint &p, QColor &color);
    void draw_line(QLine &line, QColor &color);
    // void draw_lines(QLine &lines[], QColor &color);
    void draw_rect(QRect &rect, QColor &color);

    QColor get_pixel_color(const QPoint &position) const;
    QColor get_pixel_color(int x, int y) const;

    void clear();
    void render();

    int width();
    int height();
    void maximize();

    Scene *get_scene();

private:
    Scene *_scene;
    QGraphicsView *_view;
    QPixmap _pxp;

    int _width;
    int _height;
};

#endif // DRAWER_H
