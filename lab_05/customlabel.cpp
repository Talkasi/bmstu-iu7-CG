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
    const QPoint p = event->pos();
    emit mousePressed(p);
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

void CustomLabel::onMousePressed(const QPoint &point)
{
    QPainter painter(&this->pxp);
    painter.setPen(line_color);
    painter.setBackground(QBrush(bg_color));

    if (is_point_first) {
        is_point_first = false;
        painter.drawPoint(point);
    }
    else
        painter.drawLine(prev_point, point);

    prev_point = point;
    this->setPixmap(pxp);
}

void CustomLabel::clear_pixmap()
{
    pxp.fill(Qt::transparent);
    this->setPixmap(pxp);
    is_point_first = true;
}
