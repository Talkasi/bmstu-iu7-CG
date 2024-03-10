#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>

#define N_MAIN_POINTS 5
#define N_ELLIPSE_POINTS 1000

typedef struct {
    QPointF Center;
    qreal Width;
    qreal Height;
    qreal Angle;
} Ellipse;

typedef struct {
    QPointF MainPoints[N_MAIN_POINTS];

    QVector<QPointF> EllipseLeft;
    QVector<QPointF> EllipseRight;
    QVector<QPointF> EllipseBottom;
} Figure;

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
    void on_ButtonDrawDefaultImage_clicked();
    void DrawFigure(const Figure &Figure, QGraphicsScene *Scene, QPen &Pen);
    void CalcDefaultFigurePoints(QGraphicsScene *Scene);
    void DrawImage();

    void on_ButtonTransfer_clicked();
    void on_ButtonScaling_clicked();
    void on_ButtonRotation_clicked();

    void on_InputFieldTransferDx_returnPressed();
    void on_InputFieldTransferDy_returnPressed();
    void on_InputFieldScalingMy_returnPressed();
    void on_InputFieldScalingMx_returnPressed();
    void on_InputFieldScalingKx_returnPressed();
    void on_InputFieldScalingKy_returnPressed();
    void on_InputFieldRotationRx_returnPressed();
    void on_InputFieldRotationRy_returnPressed();
    void on_InputFieldRotationAngle_returnPressed();

    void showEvent(QShowEvent *);
    void on_ButtonPrevious_clicked();
    void on_ButtonNext_clicked();

private:
    Ui::MainWindow *ui;

    QVector<Figure> FigureHistory;
    int CurrentFHIndex;

    qreal spacing = 40;
};

#endif // MAINWINDOW_H
