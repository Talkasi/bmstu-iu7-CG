#include "customlabel.h"
#include <QPainter>

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

void CustomLabel::set_line_color(QColor lineColor)
{
    this->line_color = lineColor;
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

    if (is_point_first) {
        is_point_first = false;
        first_point = point;
        painter.drawPoint(point);
    }
    else
        painter.drawLine(prev_point, point);

    prev_point = point;
    this->setPixmap(pxp);
}

void CustomLabel::onRightButtonPressed(const QPoint &point)
{
    if (is_point_first)
        return;

    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    painter.drawLine(first_point, prev_point);

    prev_point = first_point;
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
    is_point_first = true;
}

bool CustomLabel::is_top_extremum(QImage &image, int x, int y)
{
    int n = 0;

    n += image.pixelColor(x - 1, y - 1) == line_color;
    n += image.pixelColor(x, y - 1) == line_color;
    n += image.pixelColor(x + 1, y - 1) == line_color;

    return n >= 2;
}

bool CustomLabel::is_bottom_extremum(QImage &image, int x, int y)
{
    int n = 0;

    n += image.pixelColor(x - 1, y + 1) == line_color;
    n += image.pixelColor(x, y + 1) == line_color;
    n += image.pixelColor(x + 1, y + 1) == line_color;

    return n >= 2;
}

bool CustomLabel::is_extremum(QImage &image, int x, int y)
{
    if (is_top_extremum(image, x, y))
        return true;

    if (is_bottom_extremum(image, x, y))
        return true;

    return false;
}

void CustomLabel::fill_figure(QPoint min_rect_p, QPoint max_rect_p)
{
    QImage canvas_image(pxp.toImage());

    bool flag;
    for (int y = max_rect_p.y(); y >= min_rect_p.y(); --y) {
        flag = false;
        for (int x = min_rect_p.x(); x <= max_rect_p.x(); ++x) {
            if (canvas_image.pixelColor(x, y) == line_color)
                if ((flag == 0 && !is_extremum(canvas_image, x, y)) || flag == 1)
                    flag = !flag;

            if (flag)
                canvas_image.setPixelColor(x, y, line_color);
            else
                canvas_image.setPixelColor(x, y, bg_color);
        }
    }

    pxp = QPixmap::fromImage(canvas_image);
    this->setPixmap(pxp);
}
