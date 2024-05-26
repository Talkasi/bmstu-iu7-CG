#include "scene.h"

#include <QApplication>
#include <Qt>

Scene::Scene(QObject *parent) : QGraphicsScene{parent}
{
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    this->shift_pressed = QApplication::queryKeyboardModifiers() == Qt::ShiftModifier;

    if (mouseEvent->button() == Qt::LeftButton) {
        this->is_left_hold = true;
        this->mouse_pos = mouseEvent->scenePos();

        emit mousePosChanged();
        emit mouseLeftButtonPressed();
    }
    else if (mouseEvent->button() == Qt::RightButton) {
        this->mouse_pos = mouseEvent->scenePos();

        emit mousePosChanged();

        if (shift_pressed)
            emit mouseRightButtonHoldWithShift();
        else
            emit mouseRightButtonPressed();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (this->is_left_hold) {
        if (this->mouse_pos != mouseEvent->scenePos()) {
            // std::cout << mouseEvent->scenePos().x() << " " << mouseEvent->scenePos().x() << std::endl;
            this->mouse_pos = mouseEvent->scenePos();
            emit mouseLeftButtonHold();
        }
    }
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        this->is_left_hold = false;
    }
}

QPointF Scene::get_mouse_pos()
{
    return this->mouse_pos;
}
