#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_input_validators();

    connect(ui->picture, &CustomLabel::mousePressed, this, &MainWindow::add_point_to_table);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_input_validators()
{
    QValidator *DoubleValidator = new QDoubleValidator;

    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    DoubleValidator->setLocale(locale);

    ui->InputFieldX->setValidator(DoubleValidator);
    ui->InputFieldY->setValidator(DoubleValidator);
}

void MainWindow::on_ButtonBlackLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - черный)");
    ui->picture->set_line_color(Qt::black);
}

void MainWindow::on_ButtonWhiteLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - белый)");
    ui->picture->set_line_color(Qt::white);
}

void MainWindow::on_ButtonYellowLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - жёлтый)");
    ui->picture->set_line_color(Qt::yellow);
}

void MainWindow::on_ButtonRedLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - красный)");
    ui->picture->set_line_color(Qt::red);
}

void MainWindow::on_ButtonOrangeLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - оранжевый)");
    ui->picture->set_line_color(QColorConstants::Svg::orange);
}

void MainWindow::on_ButtonPurpleLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - фиолетовый)");
    ui->picture->set_line_color(QColorConstants::Svg::purple);
}

void MainWindow::on_ButtonBlueLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - синий)");
    ui->picture->set_line_color(Qt::blue);
}

void MainWindow::on_ButtonGreenLine_clicked()
{
    ui->LIneColorLabel->setText("Цвет линии (текущий цвет - зелёный)");
    ui->picture->set_line_color(Qt::green);
}

void MainWindow::on_ButtonBlackBG_clicked()
{
    ui->picture->set_bg_color(Qt::black);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonWhiteBG_clicked()
{
    ui->picture->set_bg_color(Qt::white);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonYellowBG_clicked()
{
    ui->picture->set_bg_color(Qt::yellow);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonRedBG_clicked()
{
    ui->picture->set_bg_color(Qt::red);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonOrangeBG_clicked()
{
    ui->picture->set_bg_color(QColorConstants::Svg::orange);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonPurpleBG_clicked()
{
    ui->picture->set_bg_color(QColorConstants::Svg::purple);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonBlueBG_clicked()
{
    ui->picture->set_bg_color(Qt::blue);
    ui->picture->set_picture_style();
}

void MainWindow::on_ButtonGreenBG_clicked()
{
    ui->picture->set_bg_color(Qt::green);
    ui->picture->set_picture_style();
}

int get_int_from_input(int &number, QLineEdit *input)
{
    QString str = input->text();
    if (str.length() == 0)
        return 1;

    number = str.toInt();
    return 0;
}

QPoint round_point(QPointF pointf)
{
    int x = qRound(pointf.x());
    int y = qRound(pointf.y());
    return (QPoint){x, y};
}

void MainWindow::on_FillFigureButton_clicked()
{
}

void MainWindow::on_ClearButton_clicked()
{
    ClearNeeded();
    ui->picture->clear_pixmap();
    ui->TablePoints->setRowCount(0);
}

void MainWindow::ClearNeeded()
{
    ui->LabelCommunicator->setText("> ");
}

int MainWindow::get_point(QPoint &point)
{
    int x;
    int y;

    if (get_int_from_input(x, ui->InputFieldX)) {
        ui->LabelCommunicator->setText("Ошибка. Поле `X` ввода координат точки пустое.");
        return 0;
    }

    if (get_int_from_input(y, ui->InputFieldY)) {
        ui->LabelCommunicator->setText("Ошибка. Поле `Y` ввода координат точки пустое.");
        return 0;
    }

    point.setX(x);
    point.setY(y);

    return 1;
}

void MainWindow::add_point_to_table(const QPoint &new_point)
{
    ui->TablePoints->setCurrentItem(nullptr);
    ui->TablePoints->insertRow(ui->TablePoints->rowCount());
    ui->TablePoints->setItem(ui->TablePoints->rowCount() - 1, 0,
                             new QTableWidgetItem(QString::asprintf("%d", new_point.x())));
    ui->TablePoints->setItem(ui->TablePoints->rowCount() - 1, 1,
                             new QTableWidgetItem(QString::asprintf("%d", new_point.y())));
}

void MainWindow::on_AddPointButton_clicked()
{
    ClearNeeded();

    QPoint new_point;
    if (!get_point(new_point))
        return;

    emit ui->picture->mousePressed(new_point);
}
