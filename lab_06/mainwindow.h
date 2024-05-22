#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "customlabel.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void on_ButtonWhiteLine_clicked();
    void on_ButtonYellowLine_clicked();
    void on_ButtonRedLine_clicked();
    void on_ButtonOrangeLine_clicked();
    void on_ButtonPurpleLine_clicked();
    void on_ButtonBlueLine_clicked();
    void on_ButtonGreenLine_clicked();

    void on_ClearButton_clicked();
    void on_AddPointButton_clicked();
    void on_FillFigureButton_clicked();
    void on_CloseFigureButton_clicked();

    void set_input_validators();
    int get_point(QPoint &point);
    int get_ellipse(Ellipse &ellipse);
    void add_point_to_table(const QPoint &new_point, Qt::MouseButton button);
    void ClearNeeded();

private slots:
    void on_AddSeedPointButton_clicked();
    void on_AddFigureButton_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
