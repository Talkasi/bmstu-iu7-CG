#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLineEdit>


#define EPS 1e-6


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void showEvent(QShowEvent *event);
    void resizeEvent(QResizeEvent *event);

    void set_input_validators();

    void resize_scene_rect();

    void on_LineColorButton_clicked();
    void on_PixelLengthSlider_valueChanged(int value);
    void on_BackgroundColorButton_clicked();
    void on_DrawLineButton_clicked();
    void on_ClearButton_clicked();

    int get_line(QLineF &line);
    int get_double(qreal &number, QLineEdit *InputField);

    void draw_pixel(int x, int y, QPen &Pen, QBrush &Brush);

    void draw_line_lib_alg(QLineF line);
    void draw_line_dda(QPointF Start, QPointF End);
    void draw_line_bresenhams_int(QPoint Start, QPoint End);
    void draw_line_bresenhams_double(QPointF Start, QPointF End);
    void draw_line_bresenhams_step_elimination(QPointF Start, QPointF End);

    void draw_sun_lib_alg(qreal line_length, qreal angle);
    void draw_sun_dda(qreal line_length, qreal angle);
    void draw_sun_bresenhams_int(qreal line_length, qreal angle);
    void draw_sun_bresenhams_double(qreal line_length, qreal angle);

    void Tests();

    void on_DrawSunButton_clicked();

private:
    Ui::MainWindow *ui;

    QGraphicsScene *Scene;
    QColor LineColor = QColor(0, 0, 0, 255);
    QColor BackgroundColor = QColor(255, 255, 255, 255);
    int NPixels = 1;
};
#endif // MAINWINDOW_H
