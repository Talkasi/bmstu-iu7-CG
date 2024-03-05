#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

#define POINTS_N 3

typedef struct {
    QPointF Coordinates;
    int n;
} UsersPoint;

typedef struct {
    QPointF Center;
    int TriangleLine[2];
    UsersPoint Points[POINTS_N];
    qreal AngleOY;
} NeededCircleDesc;

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
    void on_InputFieldTriangleX0_returnPressed();
    void on_InputFieldTriangleY0_returnPressed();
    void on_InputFieldTriangleX1_returnPressed();
    void on_InputFieldTriangleY1_returnPressed();
    void on_InputFieldTriangleX2_returnPressed();
    void on_InputFieldTriangleY2_returnPressed();
    void on_InputFieldPointX_returnPressed();
    void on_InputFieldPointY_returnPressed();

    void on_ButtonDeleteAllPoints_clicked();
    void on_ButtonDeleteCurrentPoint_clicked();
    void on_ButtonCompute_clicked();
    void on_ButtonAddPoint_clicked();
    void on_ButtonClearTriangle_clicked();

    int GetAndCheckPointsInput(QVector<UsersPoint> *PointsVector);
    int GetAndCheckTriangleInput(QPointF *Triangle);
    int GetPointFromLineEdits(QPointF *NewPoint);
    void DrawAnswer(QPointF *Triangle, NeededCircleDesc &Circle);
    void ClearNeeded();

    void resizeEvent(QResizeEvent *event);
    void on_XlineEditTextChanged(QString String);
    void on_YlineEditTextChanged(QString String);

    void on_InputFieldTriangleX0_textChanged(const QString &arg1);

    void on_InputFieldTriangleY0_textChanged(const QString &arg1);

    void on_InputFieldTriangleX1_textChanged(const QString &arg1);

    void on_InputFieldTriangleY1_textChanged(const QString &arg1);

    void on_InputFieldTriangleX2_textChanged(const QString &arg1);

    void on_InputFieldTriangleY2_textChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
