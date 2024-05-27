#ifndef DRAWER_H
#define DRAWER_H

#include <QGraphicsView>
#include <QPainter>
#include <scene.h>

struct myRect
{
    double x_min;
    double x_max;
    double y_min;
    double y_max;
};

class Drawer : public QObject
{
public:
    Drawer(QGraphicsView *view, QObject *parent = nullptr);

    void draw_point(QPointF &p);
    void draw_line(QLineF &line);
    void draw_line(QPointF &p1, QPointF &p2);
    void draw_lines(QPixmap &pixmap, QPointF lines_points[], int n_points, QColor &color);
    void draw_rect(myRect &rect);

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

    void save_rect(myRect &new_rect);
    void save_line(QLineF &line);

    void show_visible();

private:
    Scene *_scene;
    QGraphicsView *_view;

    QPixmap _pxp;
    QPixmap _rect_pxp;

    int _width;
    int _height;

    QColor line_color = Qt::blue;
    QColor rect_color = Qt::black;
    QColor result_color = Qt::red;

    int n_lines_points = 0;
    QPointF lines_points[100] = {};

    int n_res_lines_points = 0;
    QPointF res_lines_points[100] = {};

    bool rect_empty = true;
    myRect rect = {};
};

#endif // DRAWER_H
