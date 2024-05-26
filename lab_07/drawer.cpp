#include "drawer.h"
#include <QObject>

int cohen_sutherland_algorithm(QPoint &p1, QPoint &p2, myRect &r);

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

    // connect(_scene, &Scene::mouseLeftButtonHold, this, &Drawer::onMouseLeftButtonPressed);
    // connect(_scene, &Scene::mouseRightButtonHoldWithShift, this, &Drawer::onMouseLeftButtonPressed);
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
        rect.x_min = new_point.x();
        rect.y_min = new_point.y();
    }
    else {
        if (rect.x_min > new_point.x()) {
            rect.x_max = rect.x_min;
            rect.x_min = new_point.x();
        }
        else {
            rect.x_max = new_point.x();
        }

        if (rect.y_min > new_point.y()) {
            rect.y_max = rect.y_min;
            rect.y_min = new_point.y();
        }
        else {
            rect.y_max = new_point.y();
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

void Drawer::draw_lines(QPoint lines_points[], int n_points, QColor &color)
{
    QPainter painter(&_pxp);
    QPen pen(color);

    pen.setWidth(1);
    painter.setPen(pen);

    for (int i = 0; i + 1 < n_points; i += 2)
        painter.drawLine(lines_points[i], lines_points[i + 1]);

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

void Drawer::draw_rect(myRect &r)
{
    _rect_pxp.fill(Qt::transparent);
    QPainter painter(&_rect_pxp);
    QPen pen(rect_color);

    pen.setWidth(1);
    painter.setPen(pen);

    QRect qr = QRect(QPoint(r.x_min, r.y_min), QPoint(r.x_max, r.y_max));
    painter.drawRect(qr);

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

    draw_rect(rect);
}

void Drawer::set_result_color(QColor &color)
{
    result_color = color;

    draw_lines(res_lines_points, n_res_lines_points, color);
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

void Drawer::save_line(QLine &line)
{
    lines_points[n_lines_points] = line.p1();
    lines_points[n_lines_points + 1] = line.p2();
    n_lines_points += 2;
}

void Drawer::save_rect(myRect &new_rect)
{
    rect_empty = true;
    rect = new_rect;
}

void Drawer::show_visible()
{
    n_res_lines_points = 0;

    for (int i = 0; i + 1 < n_lines_points; i += 2) {
        QPoint p1 = lines_points[i];
        QPoint p2 = lines_points[i + 1];

        if (cohen_sutherland_algorithm(p1, p2, rect)) {
            res_lines_points[n_res_lines_points] = p1;
            res_lines_points[n_res_lines_points + 1] = p2;
            n_res_lines_points += 2;
        }
    }

    draw_lines(res_lines_points, n_res_lines_points, result_color);
}

enum visibility
{
    VISIBLE = 1,
    PARTLY_VISIBLE = 0,
    NOT_VISIBLE = -1
};

enum
{
    VERTICAL = -1,
    HORIZONTAL = 1
};

enum
{
    LEFT,
    RIGHT,
    BOTTOM,
    TOP,
    N_RECT_SIDES
};

void init_t_arr(bool t[N_RECT_SIDES], myRect r, QPoint p)
{
    t[LEFT] = p.x() < r.x_min;
    t[RIGHT] = p.x() > r.x_max;
    t[BOTTOM] = p.y() < r.y_min;
    t[TOP] = p.y() > r.y_max;
}

bool logic_mul(bool t1[N_RECT_SIDES], bool t2[N_RECT_SIDES])
{
    bool res = 0;

    for (int i = 0; i < N_RECT_SIDES; ++i)
        res |= t1[i] & t2[i];

    return res;
}

visibility get_visibility(bool t1[N_RECT_SIDES], bool t2[N_RECT_SIDES])
{
    visibility res = PARTLY_VISIBLE;

    bool s1 = t1[0] | t1[1] | t1[2] | t1[3];
    bool s2 = t2[0] | t2[1] | t2[2] | t2[3];

    if (s1 == 0 && s2 == 0)
        res = VISIBLE;
    else if (logic_mul(t1, t2))
        res = NOT_VISIBLE;

    return res;
}

int cohen_sutherland_algorithm(QPoint &p1, QPoint &p2, myRect &r)
{
    int fl = 0;
    double m = 0;

    int window[N_RECT_SIDES] = {r.x_min, r.x_max, r.y_min, r.y_max};

    if (p1.x() == p2.x()) {
        fl = VERTICAL;
    }
    else {
        m = (double)(p2.y() - p1.y()) / (double)(p2.x() - p1.x());
        if (fabs(m) < 1e-8)
            fl = HORIZONTAL;
    }

    bool t1[N_RECT_SIDES] = {};
    bool t2[N_RECT_SIDES] = {};

    for (int i = 0; i < N_RECT_SIDES; ++i) {
        init_t_arr(t1, r, p1);
        init_t_arr(t2, r, p2);

        visibility vision = get_visibility(t1, t2);
        if (vision == VISIBLE)
            break;

        if (vision == NOT_VISIBLE)
            return 0;

        if (t1[i] == t2[i])
            continue;

        if (t1[i] == 0)
            std::swap(p1, p2);

        if (fl == VERTICAL) {
            p1.setY(window[i]);
        }
        else {
            if (i < 2) {
                p1.setY(qRound(m * (window[i] - p1.x()) + p1.y()));
                p1.setX(window[i]);
            }
            else {
                p1.setX(qRound((window[i] - p1.y()) / m + p1.x()));
                p1.setY(window[i]);
            }
        }
    }

    return 1;
}
