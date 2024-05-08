#ifndef CUSTOMLABEL_H
#define CUSTOMLABEL_H
#include <QEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QTableWidget>

typedef struct {
    size_t n_points;
    QPoint points[1000];
} Figure;

typedef struct {
    size_t n_figures;
    Figure data[50];
} Figures;

typedef struct {
    size_t n_questions;
    QPoint data;
} QuestionPoint;

class CustomLabel : public QLabel
{
    Q_OBJECT
signals:
    void mousePressed(const QPoint &point, Qt::MouseButton button);

public:
    CustomLabel(QWidget *parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    void mousePressEvent(QMouseEvent *event);

    void set_picture_style();
    void resize_scene_rect();

    void set_figure_color(QColor lineColor);
    void set_bg_color(QColor bgColor);

    void clear_pixmap();

    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);

    void onMousePressed(const QPoint &point, Qt::MouseButton button);
    void onLeftButtonPressed(const QPoint &point);
    void onRightButtonPressed(const QPoint &point);

    void fill_figure(unsigned long delayMs);
    void prepare_borders(QPoint points[]);

    void prepare_borders_to_fill();
    void get_rect_p(QPoint &min, QPoint &max);
    void round_side(QPoint &p1, QPoint &p2);

    void draw_clever_line(QPoint &p1, QPoint &p2);
    void draw_line_bresenham_i(QPainter &painter, QPoint Start, QPoint End);

    void draw_borders();
    void add_in_question(QPoint p);
    void clear_questions();

private:
    QPixmap pxp;

    QColor figure_color = Qt::green;
    QColor line_color = Qt::black;
    QColor bg_color = Qt::white;
    QColor spec_bg_color = Qt::transparent;
    QColor helper_color = QColorConstants::Svg::pink;

    Figures figures = {};

    size_t n_questionable = 0;
    QuestionPoint questionable_points[1000] = {};
};

#endif // CUSTOMLABEL_H
