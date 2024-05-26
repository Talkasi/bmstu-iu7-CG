#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QMessageBox>
#include <QValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_input_validators();

    drawer = new Drawer(ui->graphicsView, this);

    set_preview_widget_color(ui->line_color_preview, drawer->get_line_color());
    set_preview_widget_color(ui->rect_color_preview, drawer->get_rect_color());
    set_preview_widget_color(ui->result_color_preview, drawer->get_result_color());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    drawer->maximize();
}

void MainWindow::showEvent(QShowEvent *e)
{
    drawer->maximize();
}

void MainWindow::set_input_validators()
{
    QValidator *IntValidator = new QIntValidator;

    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    IntValidator->setLocale(locale);

    ui->input_x_s_line->setValidator(IntValidator);
    ui->input_y_s_line->setValidator(IntValidator);
    ui->input_x_s_rect->setValidator(IntValidator);
    ui->input_y_s_rect->setValidator(IntValidator);

    ui->input_x_e_line->setValidator(IntValidator);
    ui->input_y_e_line->setValidator(IntValidator);
    ui->input_x_e_rect->setValidator(IntValidator);
    ui->input_y_e_rect->setValidator(IntValidator);
}

int get_int_from_input(int &number, QLineEdit *input)
{
    QString str = input->text();
    if (str.length() == 0)
        return 1;

    number = str.toInt();
    return 0;
}

int MainWindow::get_line(QLine &line, QString &error_msg)
{
    int x_s;
    int y_s;

    int x_e;
    int y_e;

    if (get_int_from_input(x_s, ui->input_x_s_line)) {
        error_msg = "> Ошибка. Поле `X` ввода координаты начала отрезка пустое.";
        return 0;
    }

    if (get_int_from_input(y_s, ui->input_y_s_line)) {
        error_msg = "> Ошибка. Поле `Y` ввода координаты начала отрезка пустое.";
        return 0;
    }

    if (get_int_from_input(x_e, ui->input_x_e_line)) {
        error_msg = "> Ошибка. Поле `X` ввода координаты конца отрезка пустое.";
        return 0;
    }

    if (get_int_from_input(y_e, ui->input_y_e_line)) {
        error_msg = "> Ошибка. Поле `Y` ввода координаты конца отрезка пустое.";
        return 0;
    }

    line.setPoints({x_s, y_s}, {x_e, y_e});

    return 1;
}

int MainWindow::get_rect(QRect &rect, QString &error_msg)
{
    int x_s;
    int y_s;

    int x_e;
    int y_e;

    if (get_int_from_input(x_s, ui->input_x_s_rect)) {
        error_msg = "> Ошибка. Поле `X` ввода координаты первой точки отсекателя пустое.";
        return 0;
    }

    if (get_int_from_input(y_s, ui->input_y_s_rect)) {
        error_msg = "> Ошибка. Поле `Y` ввода координаты первой точки отсекателя пустое.";
        return 0;
    }

    if (get_int_from_input(x_e, ui->input_x_e_rect)) {
        error_msg = "> Ошибка. Поле `X` ввода координаты второй точки отсекателя пустое.";
        return 0;
    }

    if (get_int_from_input(y_e, ui->input_y_e_rect)) {
        error_msg = "> Ошибка. Поле `Y` ввода координаты второй точки отсекателя пустое.";
        return 0;
    }

    rect.setCoords(x_s, y_s, x_e, y_e);

    return 1;
}

void MainWindow::on_btn_add_line_clicked()
{
    QLine new_line;
    QString error_msg;
    if (get_line(new_line, error_msg)) {
        drawer->draw_line(new_line);
    }
    else {
        QMessageBox::critical(NULL, "Ошибка!", error_msg);
    }
}

void MainWindow::on_btn_add_rect_clicked()
{
    QRect new_rect;
    QString error_msg;
    if (get_rect(new_rect, error_msg)) {
        drawer->draw_rect(new_rect);
    }
    else {
        QMessageBox::critical(NULL, "Ошибка!", error_msg);
    }
}

void MainWindow::on_btn_cut_clicked()
{
}

void MainWindow::on_btn_clear_clicked()
{
    drawer->clear();
}

void MainWindow::set_preview_widget_color(QWidget *widget, QColor color)
{
    QPalette palette;
    palette.setColor(QPalette::Window, color);
    widget->setAutoFillBackground(true);
    widget->setPalette(palette);
}

void MainWindow::on_btn_line_color_change_clicked()
{
    QColor line_color;
    line_color = QColor(QColorDialog::getColor(line_color).rgb());
    set_preview_widget_color(ui->line_color_preview, line_color);
    drawer->set_line_color(line_color);
}

void MainWindow::on_btn_rect_color_change_clicked()
{
    QColor rect_color;
    rect_color = QColor(QColorDialog::getColor(rect_color).rgb());
    set_preview_widget_color(ui->rect_color_preview, rect_color);
    drawer->set_rect_color(rect_color);
}

void MainWindow::on_btn_result_color_change_clicked()
{
    QColor result_color;
    result_color = QColor(QColorDialog::getColor(result_color).rgb());
    set_preview_widget_color(ui->result_color_preview, result_color);
    drawer->set_result_color(result_color);
}
