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
        emit posChanged();
        emit mouseLeftBtnClicked();
    }
    else if (mouseEvent->button() == Qt::RightButton) {
        this->mouse_pos = mouseEvent->scenePos();
        emit posChanged();
        if (shift_pressed)
            emit mouseRightBtnClickedWithShift();
        else
            emit mouseRightBtnClicked();
    }
}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (this->is_left_hold) {
        if (this->mouse_pos != mouseEvent->scenePos()) {
            // std::cout << mouseEvent->scenePos().x() << " " << mouseEvent->scenePos().x() << std::endl;
            this->mouse_pos = mouseEvent->scenePos();
            emit mouseLeftBtnHold();
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
