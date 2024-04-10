#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QPainter>
#include "algorithms.h"


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
    void on_BackgroundColorButton_clicked();
    void on_DrawLineButton_clicked();
    void on_ClearButton_clicked();
    void on_DrawSunButton_clicked();
    void on_CompareTimeButton_clicked();
    void on_CompareStearsButton_clicked();

    int get_line(QLineF &line);
    int get_double(qreal &number, QLineEdit *InputField);

    void set_picture_style();
    void set_line_color_button_style();
    void set_bg_color_button_style();


private:
    Ui::MainWindow *ui;

    void draw_spectrum(QPainter &painter, qreal line_length, qreal angle, int (*alg)(QPainter &, QPointF, QPointF, alg_mode));
    void draw_spectrum(QPainter &painter, qreal line_length, qreal angle, int (*alg)(QPainter &, QPoint, QPoint, alg_mode));


    QPixmap pxp;
    QColor LineColor = QColor(0, 0, 0, 255);
    QColor BackgroundColor = QColor(255, 255, 255, 255);
};
#endif // MAINWINDOW_H
