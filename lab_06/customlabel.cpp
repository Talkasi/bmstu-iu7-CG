#include "customlabel.h"
#include <QDateTime>
#include <QMessageBox>
#include <QPainter>
#include <QStack>
#include <QThread>

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

void CustomLabel::set_figure_color(QColor figure_color)
{
    this->figure_color = figure_color;
}

void CustomLabel::set_bg_color(QColor bgColor)
{
    this->bg_color = bgColor;
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

void CustomLabel::resizeEvent(QResizeEvent *event)
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
        painter.drawLine(cur_figure->points[cur_figure->n_points - 1], point);

    cur_figure->points[cur_figure->n_points++] = point;
    this->setPixmap(pxp);
}

void CustomLabel::onRightButtonPressed(const QPoint &point)
{
    Figure *cur_figure = &figures.data[figures.n_figures - 1];
    if (cur_figure->n_points <= 2) {
        QMessageBox messageBox;
        messageBox.critical(0, "Ошибка", "> Недостаточно отрезков для замыкания фигуры. Необходимо минимум 2.");
        messageBox.setFixedSize(500, 200);
        return;
    }

    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    painter.drawLine(cur_figure->points[0], cur_figure->points[cur_figure->n_points - 1]);
    cur_figure->points[cur_figure->n_points++] = cur_figure->points[0];
    ++figures.n_figures;

    this->setPixmap(pxp);
}

void CustomLabel::onMiddleButtonPresssed(const QPoint &point)
{
    QPainter painter(&this->pxp);
    painter.setBackground(QBrush(bg_color));

    painter.setPen(Qt::darkRed);
    painter.drawPoint(point);

    seed_point = point;
    this->setPixmap(pxp);
}

void CustomLabel::onMousePressed(const QPoint &point, Qt::MouseButton button)
{
    if (button == Qt::LeftButton)
        onLeftButtonPressed(point);
    else if (button == Qt::RightButton)
        onRightButtonPressed(point);
    else if (button == Qt::MiddleButton)
        onMiddleButtonPresssed(point);
}

void CustomLabel::clear_pixmap()
{
    pxp.fill(Qt::transparent);
    this->setPixmap(pxp);
    for (int i = 0; i < figures.n_figures; ++i)
        figures.data[i].n_points = 0;
    figures.n_figures = 1;
}

void CustomLabel::get_rect_p(QPoint &min, QPoint &max)
{
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

    min.setX(min.x());
    max.setX(max.x());
}

void CustomLabel::draw_borders()
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    for (size_t i = 0; i < figures.n_figures; ++i)
        for (size_t j = 0; figures.data[i].n_points && j < figures.data[i].n_points - 1; ++j) {
            painter.drawLine(figures.data[i].points[j], figures.data[i].points[j + 1]);
        }

    this->setPixmap(pxp);
}

bool CustomLabel::all_figures_closed()
{
    for (size_t i = 0; i < figures.n_figures && figures.data[i].n_points; ++i)
        if (figures.data[i].n_points - 1 == 0 ||
            (figures.data[i].points[0] != figures.data[i].points[figures.data[i].n_points - 1]))
            return false;

    return true;
}

void CustomLabel::draw_ellipse(Ellipse ellipse)
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    painter.drawEllipse(ellipse.center, ellipse.rx, ellipse.ry);

    this->setPixmap(pxp);
}

void CustomLabel::seed_search(QImage &canvas_image, QStack<QPoint> &stack, int xl, int x_prev, int y)
{
    int x = xl;

    do {
        bool fl = false;

        while (x <= x_prev && canvas_image.pixelColor({x, y}) != line_color &&
               canvas_image.pixelColor({x, y}) != figure_color) {
            ++x;
            fl = true;
        }

        if (fl == true) {
            if (x == x_prev && canvas_image.pixelColor({x, y}) != line_color &&
                canvas_image.pixelColor({x, y}) != figure_color)
                stack.push({x, y});
            else
                stack.push({x - 1, y});
        }
        ++x;
    } while (x - 1 < x_prev);
}

int CustomLabel::fill_figure_with_seed(unsigned long delayMs, unsigned long &time)
{
    if (!all_figures_closed())
        return 1;

    time = QDateTime::currentMSecsSinceEpoch();
    QStack<QPoint> stack = {};

    QImage canvas_image(pxp.toImage());

    QPoint seed = {};
    int x_prev;
    int x;
    stack.push(seed_point);
    while (!stack.isEmpty()) {
        seed = stack.pop();
        if (canvas_image.pixelColor(seed) == bg_color)
            continue;

        x = seed.x();

        do {
            canvas_image.setPixelColor({x, seed.y()}, figure_color);
            ++x;
        } while (canvas_image.pixelColor({x, seed.y()}) != line_color);

        x_prev = x - 1;
        x = seed.x();

        do {
            canvas_image.setPixelColor({x, seed.y()}, figure_color);
            --x;
        } while (canvas_image.pixelColor({x, seed.y()}) != line_color);

        seed_search(canvas_image, stack, x + 1, x_prev, seed.y() - 1);
        seed_search(canvas_image, stack, x + 1, x_prev, seed.y() + 1);

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

    time = QDateTime::currentMSecsSinceEpoch() - time;
    return 0;
}
