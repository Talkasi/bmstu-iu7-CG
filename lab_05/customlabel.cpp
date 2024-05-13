#include "customlabel.h"
#include <QPainter>
#include <QThread>
#include <QDateTime>
#include <QMessageBox>

CustomLabel::CustomLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent, f)
{
    resize_scene_rect();
    set_picture_style();

    connect(this, &CustomLabel::mousePressed, this, &CustomLabel::onMousePressed);
}

void CustomLabel::mousePressEvent(QMouseEvent *event)
{
    emit mousePressed(event->pos(), event->button());
}

void CustomLabel::set_picture_style()
{
    this->setStyleSheet("background-color: #" +
                        QString::asprintf("%02X%02X%02X", bg_color.red(), bg_color.green(), bg_color.blue()) + ";\n");
}

void CustomLabel::resize_scene_rect()
{
    QRect SceneRect = this->rect();

    int height = SceneRect.height();
    int width = SceneRect.width();

    pxp = QPixmap(width, height);
    pxp.fill(Qt::transparent);

    this->setPixmap(pxp);
}

void CustomLabel::set_figure_color(QColor figure_color)
{
    this->figure_color = figure_color;
}

void CustomLabel::set_bg_color(QColor bgColor)
{
    this->bg_color = bgColor;
}

void CustomLabel::resizeEvent(QResizeEvent *event)
{
    resize_scene_rect();
    set_picture_style();
}

void CustomLabel::showEvent(QShowEvent *event)
{
    resize_scene_rect();
    set_picture_style();
}

void CustomLabel::onLeftButtonPressed(const QPoint &point)
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    if (figures.n_figures == 0)
        figures.n_figures = 1;

    Figure *cur_figure = &figures.data[figures.n_figures - 1];

    if (cur_figure->n_points == 0)
        painter.drawPoint(point);
    else
        draw_line_bresenham_i(painter, cur_figure->points[cur_figure->n_points - 1], point);

    cur_figure->points[cur_figure->n_points++] = point;
    this->setPixmap(pxp);
}

void CustomLabel::onRightButtonPressed(const QPoint &point)
{
    Figure *cur_figure = &figures.data[figures.n_figures - 1];
    if (cur_figure->n_points <= 2) {
        QMessageBox messageBox;
        messageBox.critical(0,"Ошибка", "> Недостаточно отрезков для замыкания фигуры. Необходимо минимум 2.");
        messageBox.setFixedSize(500,200);
        return;
    }

    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    draw_line_bresenham_i(painter, cur_figure->points[0], cur_figure->points[cur_figure->n_points - 1]);
    cur_figure->points[cur_figure->n_points++] = cur_figure->points[0];
    ++figures.n_figures;

    this->setPixmap(pxp);
}

void CustomLabel::onMousePressed(const QPoint &point, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
        onLeftButtonPressed(point);
    else if (button == Qt::RightButton)
        onRightButtonPressed(point);
}

void CustomLabel::clear_pixmap()
{
    pxp.fill(Qt::transparent);
    this->setPixmap(pxp);
    for (int i = 0; i < figures.n_figures; ++i)
        figures.data[i].n_points = 0;
    figures.n_figures = 0;
}

typedef struct {
    double a;
    double b;
    double c;
} LineCoefs;

LineCoefs get_line_coefs(int x1, int y1, int x2, int y2)
{
    LineCoefs lc;
    lc.a = y1 - y2;
    lc.b = x2 - x1;
    lc.c = x1 * y2 - x2 * y1;
    return lc;
}

QPoint solve_lines_intersection(LineCoefs &lc1, LineCoefs &lc2)
{
    double opr = lc1.a * lc2.b - lc2.a * lc1.b;
    double opr1 = lc1.b * lc2.c - lc1.c * lc2.b;
    double opr2 = lc1.c * lc2.a - lc1.a * lc2.c;

    QPoint res;
    res.setX(qRound(opr1 / opr));
    res.setY(qRound(opr2 / opr));

    return res;
}

void CustomLabel::round_side(QPoint &p1, QPoint &p2)
{
    if (p1 == p2)
        return;

    LineCoefs lc = get_line_coefs(p1.x(), p1.y(), p2.x(), p2.y());

    int y_min, y_max;

    if (p1.y() > p2.y()) {
        y_max = p1.y();
        y_min = p2.y();
    } else {
        y_max = p2.y();
        y_min = p1.y();
    }

    QImage canvas_image(pxp.toImage());
    for (int y = y_min; y < y_max; ++y) {
        LineCoefs scan_lc = {0, 1, (double)-y};
        QPoint intersec_p = solve_lines_intersection(lc, scan_lc);

        if (canvas_image.pixelColor(intersec_p.x(), y) != helper_color)
            canvas_image.setPixelColor(intersec_p.x(), y, helper_color);
        else {
            int x = intersec_p.x() + 1;
            while (canvas_image.pixelColor(x, y) == helper_color)
                ++x;
            canvas_image.setPixelColor(x, y, helper_color);
        }
    }
    pxp = QPixmap::fromImage(canvas_image);
    this->setPixmap(pxp);
}

static int sign(int n)
{
    if (n == 0)
        return 0;
    return (n >= 0) ? 1 : -1;
}

void CustomLabel::draw_line_bresenham_i(QPainter &painter, QPoint Start, QPoint End)
{
    int x = Start.x();
    int y = Start.y();
    if (Start == End)
        return;

    int dx = End.x() - Start.x();
    int dy = End.y() - Start.y();

    int sx = sign(dx);
    int sy = sign(dy);

    dx = qAbs(dx);
    dy = qAbs(dy);

    int exchange = 0;
    if (dy > dx) {
        exchange = 1;
        int tmp = dx;
        dx = dy;
        dy = tmp;
    }

    int err = 2 * dy - dx;
    for (int i = 0; i <= dx; ++i) {
        painter.drawPoint(x, y);

        if (err >= 0) {
            if (exchange)
                x += sx;
            else
                y += sy;
            err -= 2 * dx;
        }
        if (err <= 0) {
            if (exchange)
                y += sy;
            else
                x += sx;
            err += 2 * dy;
        }
    }
}

void CustomLabel::prepare_borders_to_fill()
{
    for (size_t i = 0; i < figures.n_figures; ++i)
        for (size_t j = 0; figures.data[i].n_points && j < figures.data[i].n_points - 1; ++j) {
            round_side(figures.data[i].points[j],
                       figures.data[i].points[j + 1]);
        }
}

void CustomLabel::get_rect_p(QPoint &min, QPoint &max) {
    QPoint cur_point = figures.data[0].points[0];
    min.setX(cur_point.x());
    min.setY(cur_point.y());
    max = min;

    for (size_t i = 0; i < figures.n_figures; ++i) {
        for (size_t j = 0; j < figures.data[i].n_points; ++j) {
            cur_point = figures.data[i].points[j];
            if (cur_point.x() < min.x())
                min.setX(cur_point.x());

            if (cur_point.y() < min.y())
                min.setY(cur_point.y());

            if (cur_point.x() > max.x())
                max.setX(cur_point.x());

            if (cur_point.y() > max.y())
                max.setY(cur_point.y());
        }
    }

    min.setX(min.x() - 1);
    max.setX(max.x() + 1);
}

void CustomLabel::draw_borders()
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    for (size_t i = 0; i < figures.n_figures; ++i)
        for (size_t j = 0; figures.data[i].n_points && j < figures.data[i].n_points - 1; ++j) {
            draw_line_bresenham_i(painter, figures.data[i].points[j],
                                  figures.data[i].points[j + 1]);
        }

    this->setPixmap(pxp);
}

bool CustomLabel::all_figures_closed()
{
    for (size_t i = 0; i < figures.n_figures; ++i)
        if (figures.data[i].n_points - 1 == 0 || figures.data[i].points[0] != figures.data[i].points[figures.data[i].n_points - 1])
            return false;

    return true;
}

int CustomLabel::fill_figure(unsigned long delayMs, unsigned long &time)
{
    if (!all_figures_closed())
        return 1;

    time = QDateTime::currentMSecsSinceEpoch();

    prepare_borders_to_fill();
    if (delayMs > 0) {
        this->repaint();
        QThread::msleep(delayMs);
    }

    QPoint min_rect_p;
    QPoint max_rect_p;

    get_rect_p(min_rect_p, max_rect_p);
    QImage canvas_image(pxp.toImage());

    bool flag;
    for (int y = max_rect_p.y(); y >= min_rect_p.y(); --y) {
        flag = false;
        for (int x = min_rect_p.x(); x <= max_rect_p.x(); ++x) {
            if (canvas_image.pixelColor(x, y) == helper_color)
                flag = !flag;

            if (flag)
                canvas_image.setPixelColor(x, y, figure_color);
            else
                canvas_image.setPixelColor(x, y, spec_bg_color);
        }

        if (delayMs > 0) {
            pxp = QPixmap::fromImage(canvas_image);
            this->setPixmap(pxp);
            this->repaint();

            QThread::msleep(delayMs);
        }
    }

    pxp = QPixmap::fromImage(canvas_image);
    this->setPixmap(pxp);

    if (delayMs > 0) {
        this->repaint();
        QThread::msleep(delayMs);
    }

    draw_borders();

    time = QDateTime::currentMSecsSinceEpoch() - time;
    return 0;
}
