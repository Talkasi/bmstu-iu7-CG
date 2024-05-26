#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawer.h"
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
    void set_input_validators();
    void set_preview_widget_color(QWidget *widget, QColor color);

    int get_line(QLine &line, QString &error_msg);
    int get_rect(myRect &rect, QString &error_msg);

private slots:
    void on_btn_add_line_clicked();
    void on_btn_add_rect_clicked();

    void on_btn_cut_clicked();
    void on_btn_clear_clicked();

    void on_btn_line_color_change_clicked();
    void on_btn_rect_color_change_clicked();
    void on_btn_result_color_change_clicked();

    void resizeEvent(QResizeEvent *e);
    void showEvent(QShowEvent *e);

private:
    Ui::MainWindow *ui;
    Drawer *drawer;
};
#endif // MAINWINDOW_H
