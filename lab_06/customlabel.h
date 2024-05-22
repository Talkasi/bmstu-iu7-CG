#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableWidget>

typedef struct
{
    size_t n_points;
    QPoint points[1000];
} Figure;

typedef struct
{
    size_t n_figures;
    Figure data[50];
} Figures;

typedef struct
{
    int rx;
    int ry;
    QPoint center;
} Ellipse;

class CustomLabel : public QLabel
{
    Q_OBJECT
signals:
    void mousePressed(const QPoint &point, Qt::MouseButton button);

public:
    CustomLabel(QWidget *parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    void mousePressEvent(QMouseEvent *event);

    void set_picture_style();
    void resize_scene_rect();

    void set_figure_color(QColor lineColor);
    void set_bg_color(QColor bgColor);

    void clear_pixmap();

    void resizeEvent(QResizeEvent *event);

    void onMousePressed(const QPoint &point, Qt::MouseButton button);
    void onLeftButtonPressed(const QPoint &point);
    void onRightButtonPressed(const QPoint &point);
    void onMiddleButtonPresssed(const QPoint &point);

    int fill_figure_with_seed(unsigned long delayMs, unsigned long &time);
    void seed_search(QImage &canvas_image, QStack<QPoint> &stack, int xl, int x_prev, int y);
    void prepare_borders(QPoint points[]);

    void prepare_borders_to_fill();
    void get_rect_p(QPoint &min, QPoint &max);
    void round_side(QPoint &p1, QPoint &p2);
    void draw_line_bresenham_i(QPainter &painter, QPoint Start, QPoint End);

    void draw_borders();
    void draw_ellipse(Ellipse ellipse);
    bool all_figures_closed();

    bool is_seed_set();
    QPoint get_seed_point();

private:
    QPixmap pxp = {};

    QColor figure_color = Qt::green;
    QColor line_color = Qt::black;
    QColor bg_color = QColor(254, 254, 254);

    Figures figures = {};

    bool seed_set = false;
    int seed_point_r = 1;
    QPoint seed_point = {};
};

#endif // CUSTOMLABEL_H
