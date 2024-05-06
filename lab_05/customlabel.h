#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableWidget>

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

    void set_line_color(QColor lineColor);
    void set_bg_color(QColor bgColor);

    void clear_pixmap();

    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

    void onMousePressed(const QPoint &point, Qt::MouseButton button);
    void onLeftButtonPressed(const QPoint &point);
    void onRightButtonPressed(const QPoint &point);

    void fill_figure(QPoint min_rect_p, QPoint max_rect_p);

    bool is_extremum(QImage &image, int x, int y);
    bool is_top_extremum(QImage &image, int x, int y);
    bool is_bottom_extremum(QImage &image, int x, int y);

private:
    QPixmap pxp;

    QColor line_color = Qt::black;
    QColor bg_color = Qt::white;

    bool is_point_first = true;
    QPoint prev_point;
    QPoint first_point;
};

#endif // CUSTOMLABEL_H
