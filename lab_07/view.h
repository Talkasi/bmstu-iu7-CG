#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QMouseEvent>

class View : public QGraphicsScene
{
    Q_OBJECT
signals:
    void posChanged();
    void mouseLeftBtnClicked();
    void mouseRightBtnClicked();
    void mouseLeftBtnHold();
    void mouseRightBtnClickedWithShift();

public:
    explicit View(QObject *parent = nullptr);

    QPointF get_mouse_pos();

    void draw_line(QLine line);
    void draw_rect(QRect rect);

    void set_line_color(QColor color);
    void set_rect_color(QColor color);
    void set_result_color(QColor color);

    QColor get_line_color();
    QColor get_rect_color();
    QColor get_result_color();

    void resize_scene_rect();

    void set_figure_color(QColor lineColor);
    void set_bg_color(QColor bgColor);

    void resizeEvent(QResizeEvent *event);

    void onMousePressed(const QPoint &point, Qt::MouseButton button);
    void onLeftButtonPressed(const QPoint &point);
    void onRightButtonPressed(const QPoint &point);

    void clear_pixmap();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QPixmap pxp = {};

    QPointF mouse_pos;
    bool is_left_hold = false;
    bool shift_pressed;

    QColor rect_color = Qt::black;
    QColor line_color = Qt::green;
    QColor result_color = Qt::red;
};

#endif // CUSTOMLABEL_H
