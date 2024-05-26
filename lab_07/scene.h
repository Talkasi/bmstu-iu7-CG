#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{
    Q_OBJECT
signals:
    void mousePosChanged();

    void mouseLeftButtonPressed();
    void mouseRightButtonPressed();

    void mouseLeftButtonHold();
    void mouseRightBtnClickedWithShift();

public:
    explicit Scene(QObject *parent = nullptr);
    QPointF get_mouse_pos();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

private:
    QPointF mouse_pos;
    bool is_left_hold = false;
    bool shift_pressed;
};

#endif // SCENE_H
