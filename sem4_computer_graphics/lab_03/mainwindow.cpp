#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtMath>
#include <QDebug>
#include <QColorDialog>
#include <QPainterPath>


enum algorithms {
    LIBRARY,
    DDA,
    B_DOUBLE,
    B_INT,
    B_STEARS,
    BY
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_input_validators();

    Scene = new QGraphicsScene();
    ui->GraphicsView->setScene(Scene);
}


MainWindow::~MainWindow()
{
    delete Scene;
    delete ui;
}

void MainWindow::set_input_validators() {
    QValidator *DoubleValidator = new QDoubleValidator;
    QLocale locale_double(QLocale::C);
    locale_double.setNumberOptions(QLocale::RejectGroupSeparator);
    DoubleValidator->setLocale(locale_double);

    ui->InputFieldLineX0->setValidator(DoubleValidator);
    ui->InputFieldLineY0->setValidator(DoubleValidator);
    ui->InputFieldLineX1->setValidator(DoubleValidator);
    ui->InputFieldLineY1->setValidator(DoubleValidator);
    ui->InputFieldStep->setValidator(DoubleValidator);
    ui->InputFieldLineLength->setValidator(DoubleValidator);
}


void MainWindow::resize_scene_rect()
{
    int spacing = 50;
    QRect SceneRect = ui->GraphicsView->rect();
    SceneRect.setHeight(SceneRect.height() - spacing * 2);
    SceneRect.setWidth(SceneRect.width() - spacing * 2);
    Scene->setSceneRect(SceneRect);
}


void MainWindow::showEvent(QShowEvent *event)
{
    resize_scene_rect();

    Tests();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    resize_scene_rect();

    Tests();
}


void MainWindow::on_LineColorButton_clicked()
{
    QColorDialog ColorDialog;
    LineColor = ColorDialog.getColor();
    ui->LineColorButton->setStyleSheet("border-radius: 12px;\nbackground-color: #" +
                                       QString::asprintf("%02X%02X%02X",
                                       LineColor.red(), LineColor.green(), LineColor.blue()) + ";\n");
}


void MainWindow::on_BackgroundColorButton_clicked()
{
    QColorDialog ColorDialog;
    BackgroundColor = ColorDialog.getColor();
    ui->BackgroundColorButton->setStyleSheet("border-radius: 12px;\nbackground-color: #" +
                                             QString::asprintf("%02X%02X%02X",
                                             BackgroundColor.red(), BackgroundColor.green(), BackgroundColor.blue()) + ";\n");
}


void MainWindow::on_PixelLengthSlider_valueChanged(int value)
{
    value = (value + 1) / 10;
    NPixels = value * value;
    if (NPixels == 0)
        NPixels = 1;
    ui->NPixelsLabel->setText(QString::asprintf("%d", NPixels));
}


int get_double_from_input(qreal &number, QLineEdit *input)
{
    QString str = input->text();
    if (str.length() == 0)
        return 1;

    number = str.toDouble();
    return 0;
}


int MainWindow::get_line(QLineF &line)
{
    qreal x0 = 0;
    qreal y0 = 0;

    qreal x1 = 0;
    qreal y1 = 0;

    if (get_double_from_input(x0, ui->InputFieldLineX0)) {
        ui->LabelCommunicator->setText("> Ошибка: поле X0 для ввода отрезка пустое.");
        return 1;
    }

    if (get_double_from_input(y0, ui->InputFieldLineY0)) {
        ui->LabelCommunicator->setText("> Ошибка: поле Y0 для ввода отрезка пустое.");
        return 1;
    }

    if (get_double_from_input(x1, ui->InputFieldLineX1)) {
        ui->LabelCommunicator->setText("> Ошибка: поле X1 для ввода отрезка пустое.");
        return 1;
    }

    if (get_double_from_input(y1, ui->InputFieldLineY1)) {
        ui->LabelCommunicator->setText("> Ошибка: поле Y1 для ввода отрезка пустое.");
        return 1;
    }

    if (qFabs(x0 - x1) < EPS && qFabs(y0 - y1) < EPS) {
        ui->LabelCommunicator->setText("> Ошибка: отрезок вырожден.");
        return 1;
    }

    line.setLine(x0, y0, x1, y1);
    return 0;
}


int MainWindow::get_double(qreal &number, QLineEdit *InputField)
{
    QString str = InputField->text();
    if (str.isEmpty())
        return 1;

    number = str.toDouble();
    return 0;
}


QPoint round_point(QPointF pointf)
{
    int x = qRound(pointf.x());
    int y = qRound(pointf.y());
    return (QPoint) {x, y};
}


QLine round_line(QLineF linef)
{
    QPoint p1 = round_point(linef.p1());
    QPoint p2 = round_point(linef.p2());
    return (QLine) {p1, p2};
}


void MainWindow::on_DrawLineButton_clicked()
{
    QLineF linef = {};

    if (get_line(linef))
        return;

    int a_i = ui->AlgorithmsChooser->currentIndex();
    switch (a_i) {
    case LIBRARY:
    {
        draw_line_lib_alg(linef);
    } break;
    case DDA:
    {
        draw_line_dda(linef.p1(), linef.p2());
    } break;
    case B_DOUBLE:
    {
        draw_line_bresenhams_double(linef.p1(), linef.p2());
    } break;
    case B_INT:
    {
        QLine line = round_line(linef);
        draw_line_bresenhams_int(line.p1(), line.p2());
    } break;
    case B_STEARS:
    {
        draw_line_bresenhams_step_elimination(linef.p1(), linef.p2());
    } break;
    case BY:
    {

    } break;
    default:
    {
        qDebug() << "WTF";
    }
    }
}


void MainWindow::on_ClearButton_clicked()
{
    Scene->clear();
}


void MainWindow::Tests()
{
    Scene->clear();
}


void MainWindow::on_DrawSunButton_clicked()
{
    qreal line_length = 0;
    qreal angle = 0;

    if (get_double(line_length, ui->InputFieldLineLength)) {
        ui->LabelCommunicator->setText("> Ошибка: поле длины отрезка пустое.");
        return;
    }

    if (get_double(angle, ui->InputFieldStep)) {
        ui->LabelCommunicator->setText("> Ошибка: поле шага изменения угла наклона пустое.");
        return;
    }

    angle = angle * M_PI / 180;

    int a_i = ui->AlgorithmsChooser->currentIndex();
    switch (a_i) {
    case LIBRARY:
    {
        draw_sun_lib_alg(line_length, angle);
    } break;
    case DDA:
    {
        draw_sun_dda(line_length, angle);
    } break;
    case B_DOUBLE:
    {
        draw_sun_bresenhams_double(line_length, angle);
    } break;
    case B_INT:
    {
        draw_sun_bresenhams_int(line_length, angle);
    } break;
    case B_STEARS:
    {

    } break;
    case BY:
    {

    } break;
    default:
    {
        qDebug() << "WTF";
    }
    }
}

