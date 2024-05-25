#include "view.h"
#include <QApplication>
#include <QDateTime>
#include <QMessageBox>
#include <QPainter>
#include <QStack>
#include <QThread>

View::View(QObject *parent) : QGraphicsScene(parent)
{
    resize_scene_rect();

    // connect(this, &View::mousePressed, this, &View::onMousePressed);
}

// void View::mousePressEvent(QMouseEvent *event)
// {
//     emit mousePressed(event->pos(), event->button());
// }

void View::set_line_color(QColor color)
{
    this->line_color = color;
}

void View::set_rect_color(QColor color)
{
    this->rect_color = color;
}

void View::set_result_color(QColor color)
{
    this->result_color = color;
}

QColor View::get_line_color()
{
    return this->line_color;
}

QColor View::get_rect_color()
{
    return this->rect_color;
}

QColor View::get_result_color()
{
    return this->result_color;
}

void View::resize_scene_rect()
{
    QRect SceneRect = this->rect();

    int height = SceneRect.height();
    int width = SceneRect.width();

    pxp = QPixmap(width, height);
    pxp.fill(Qt::transparent);
    this->clear();
    this->addPixmap(pxp);
}

void View::resizeEvent(QResizeEvent *event)
{
    resize_scene_rect();
}

void View::onLeftButtonPressed(const QPoint &point)
{
    // QPainter painter(&this->pxp);
    // painter.setPen(line_color);

    // if (figures.n_figures == 0)
    //     figures.n_figures = 1;

    // Figure *cur_figure = &figures.data[figures.n_figures - 1];

    // if (cur_figure->n_points == 0)
    //     painter.drawPoint(point);
    // else
    //     painter.drawLine(cur_figure->points[cur_figure->n_points - 1], point);

    // cur_figure->points[cur_figure->n_points++] = point;
    // this->setPixmap(pxp);
}

void View::onRightButtonPressed(const QPoint &point)
{
    // QPainter painter(&this->pxp);
    // painter.setPen(line_color);
    // painter.setBackground(QBrush(bg_color));

    // painter.drawLine(cur_figure->points[0], cur_figure->points[cur_figure->n_points - 1]);
    // cur_figure->points[cur_figure->n_points++] = cur_figure->points[0];
    // ++figures.n_figures;

    // this->setPixmap(pxp);
}

void View::onMousePressed(const QPoint &point, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
        onLeftButtonPressed(point);
    // else if (button == Qt::RightButton)
    //     onRightButtonPressed(point);
}

void View::clear_pixmap()
{
    pxp.fill(Qt::transparent);
    // this->setPixmap(pxp);
    // for (int i = 0; i < figures.n_figures; ++i)
    //     figures.data[i].n_points = 0;
    // figures.n_figures = 1;
    // seed_set = false;
}

void View::draw_line(QLine line)
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.drawLine(line);
}

void View::draw_rect(QRect rect)
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.drawRect(rect);
}

void View::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
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

void View::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (this->is_left_hold) {
        if (this->mouse_pos != mouseEvent->scenePos()) {
            this->mouse_pos = mouseEvent->scenePos();
            emit mouseLeftBtnHold();
        }
    }
}

void View::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() == Qt::LeftButton) {
        this->is_left_hold = false;
    }
}

QPointF View::get_mouse_pos()
{
    return this->mouse_pos;
}
