#include "drawer.h"
#include <QObject>

Drawer::Drawer(QGraphicsView *view, QObject *parent)
{
    view->setFrameShape(QFrame::NoFrame);

    _scene = new Scene(parent);
    _view = view;

    view->setScene(_scene);
    view->setStyleSheet("QGraphicsView {background-color: white}");

    QRect view_rect = view->rect();
    view->scene()->setSceneRect(0, 0, view_rect.width(), view_rect.height());

    _width = _scene->width();
    _height = _scene->height();

    clear();

    connect(_scene, &Scene::mouseLeftButtonPressed, this, &Drawer::onMouseLeftButtonPressed);
    connect(_scene, &Scene::mouseRightButtonPressed, this, &Drawer::onMouseRightButtonPressed);

    connect(_scene, &Scene::mouseLeftButtonHold, this, &Drawer::onMouseLeftButtonPressed);
    connect(_scene, &Scene::mouseRightButtonHoldWithShift, this, &Drawer::onMouseLeftButtonPressed);
}

void Drawer::onMouseLeftButtonPressed()
{
    QPointF tmp = _scene->get_mouse_pos();
    QPoint new_point = {qRound(tmp.x()), qRound(tmp.y())};

    lines_points[n_lines_points] = new_point;
    ++n_lines_points;

    if (n_lines_points % 2) {
        /* This was the first point of the line */
    }
    else {
        /* This was the second point of the line */
        draw_line(lines_points[n_lines_points - 2], new_point);
    }
}

void Drawer::onMouseRightButtonPressed()
{
    QPointF tmp = _scene->get_mouse_pos();
    QPoint new_point = {qRound(tmp.x()), qRound(tmp.y())};

    if (rect_empty) {
        rect.setBottomLeft(new_point);
    }
    else {
        QPoint prev_point = rect.bottomLeft();
        if (prev_point.x() > new_point.x()) {
            rect.setTopRight(new_point);
        }
        else {
            rect.setTopRight(prev_point);
            rect.setBottomLeft(new_point);
        }
        draw_rect(rect);
    }
    rect_empty = !rect_empty;
}

void Drawer::maximize()
{
    QRectF new_scene_rect = _view->rect();
    _scene->setSceneRect(0, 0, new_scene_rect.width(), new_scene_rect.height());

    _width = _scene->width();
    _height = _scene->height();

    _pxp = QPixmap(_width, _height);
    _pxp.fill(Qt::transparent);

    _rect_pxp = QPixmap(_width, _height);
    _rect_pxp.fill(Qt::transparent);
}

void Drawer::draw_line(QLine &line)
{
    QPainter painter(&_pxp);
    QPen pen(line_color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(line);

    render();
}

void Drawer::draw_line(QPoint &p1, QPoint &p2)
{
    QPainter painter(&_pxp);
    QPen pen(line_color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(p1, p2);

    render();
}

void Drawer::draw_rect(QRect &rect)
{
    _rect_pxp.fill(Qt::transparent);
    QPainter painter(&_rect_pxp);
    QPen pen(rect_color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawRect(rect);

    render();
}

QColor Drawer::get_pixel_color(const QPoint &pos) const
{
    return QColor(_pxp.toImage().pixelColor(pos.x(), pos.y()).rgb());
}

QColor Drawer::get_pixel_color(int x, int y) const
{
    return get_pixel_color(QPoint(x, y));
}

void Drawer::clear()
{
    this->_pxp.fill(Qt::transparent);
    this->_rect_pxp.fill(Qt::transparent);

    _scene->clear();

    n_lines_points = 0;
    rect_empty = true;
}

void Drawer::render()
{
    _scene->clear();
    _scene->addPixmap(this->_rect_pxp);
    _scene->addPixmap(this->_pxp);
}

Scene *Drawer::get_scene()
{
    return _scene;
}

int Drawer::width()
{
    return _width;
}

int Drawer::height()
{
    return _height;
}

void Drawer::set_line_color(QColor &color)
{
    line_color = color;
}

void Drawer::set_rect_color(QColor &color)
{
    rect_color = color;
}

void Drawer::set_result_color(QColor &color)
{
    result_color = color;
}

QColor Drawer::get_line_color()
{
    return line_color;
}

QColor Drawer::get_rect_color()
{
    return rect_color;
}

QColor Drawer::get_result_color()
{
    return result_color;
}
