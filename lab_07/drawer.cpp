#include "drawer.h"

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

    // connect(_scene, &Scene::clicked, this, &Drawer::btn_line_color_change_clicked);
}

void Drawer::maximize()
{
    QRectF new_scene_rect = _view->rect();
    _scene->setSceneRect(0, 0, new_scene_rect.width(), new_scene_rect.height());

    _width = _scene->width();
    _height = _scene->height();

    _pxp = QPixmap(_width, _height);
    _pxp.fill(Qt::transparent);
}

void Drawer::draw_point(QPoint &p, QColor &color)
{
    QPainter painter(&_pxp);
    QPen pen(color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(p.x(), p.y(), p.x(), p.y());

    _scene->clear();
    _scene->addPixmap(this->_pxp);
}

void Drawer::draw_line(QLine &line, QColor &color)
{
    QPainter painter(&_pxp);
    QPen pen(color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawLine(line);

    _scene->clear();
    _scene->addPixmap(this->_pxp);
}

// void Drawer::draw_lines(QLine lines[], QColor &color)
// {
//     for (int i = 0; i < lines.size(); i++)
//         draw_line(lines[i].start, lines[i].end, color);
// }

void Drawer::draw_rect(QRect &rect, QColor &color)
{
    QPainter painter(&_pxp);
    QPen pen(color);

    pen.setWidth(1);
    painter.setPen(pen);

    painter.drawRect(rect);

    _scene->clear();
    _scene->addPixmap(this->_pxp);
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
    this->_pxp = QPixmap(_scene->width(), _scene->height());
    // this->_pxp.fill(Qt::transparent);
    this->_pxp.fill(Qt::white);

    _scene->clear();
}

void Drawer::render()
{
    _scene->clear();
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
