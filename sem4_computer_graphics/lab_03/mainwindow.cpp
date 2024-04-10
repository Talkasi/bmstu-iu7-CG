#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtMath>
#include <QDebug>
#include <QColorDialog>
#include <QPainterPath>
#include <QPainter>
#include <QStyle>
#include <QtCharts>
#include <sys/time.h>


enum algorithms {
    LIB,
    DDA,
    BRESENHAM_D,
    BRESENHAM_I,
    BRESENHAM_ANTIALIASING,
    WU,
    N_ALGORITHMS
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    set_input_validators();

    int width = ui->picture->width();
    int height = ui->picture->height();

    pxp = QPixmap(width, height);
    pxp.fill(Qt::transparent);
    ui->picture->setPixmap(pxp);

    set_picture_style();
}


MainWindow::~MainWindow()
{
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


void MainWindow::set_picture_style()
{
    ui->picture->setStyleSheet("border-radius: 0px;\nbackground-color: #" +
                               QString::asprintf("%02X%02X%02X",
                               BackgroundColor.red(), BackgroundColor.green(), BackgroundColor.blue()) + ";\n");
}


void MainWindow::set_line_color_button_style()
{
    ui->LineColorButton->setStyleSheet("border-radius: 12px;\nbackground-color: #" +
                                       QString::asprintf("%02X%02X%02X",
                                       LineColor.red(), LineColor.green(), LineColor.blue()) + ";\n");
}


void MainWindow::set_bg_color_button_style()
{
    ui->BackgroundColorButton->setStyleSheet("border-radius: 12px;\nbackground-color: #" +
                                             QString::asprintf("%02X%02X%02X",
                                             BackgroundColor.red(), BackgroundColor.green(), BackgroundColor.blue()) + ";\n");
}


void MainWindow::resize_scene_rect()
{
    ui->LabelCommunicator->setText("> ");
    int spacing = 0;
    QRect SceneRect = ui->picture->rect();
    int height = SceneRect.height() - spacing * 2;
    int width = SceneRect.width() - spacing * 2;

    pxp = QPixmap(width, height);
    pxp.fill(Qt::transparent);
    ui->picture->setPixmap(pxp);
}


void MainWindow::showEvent(QShowEvent *event)
{
    resize_scene_rect();
    set_picture_style();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    resize_scene_rect();
    set_picture_style();
}


void MainWindow::on_LineColorButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    QColorDialog ColorDialog;
    LineColor = ColorDialog.getColor();
    set_line_color_button_style();
}


void MainWindow::on_BackgroundColorButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    QColorDialog ColorDialog;
    BackgroundColor = ColorDialog.getColor();
    set_bg_color_button_style();
    set_picture_style();
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
        ui->LabelCommunicator->setText("> Предупреждение: отрезок вырожден в точку.");
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
    ui->LabelCommunicator->setText("> ");
    QPainter painter(&this->pxp);
    QBrush BgBrush = QBrush(BackgroundColor);
    painter.setPen(LineColor);
    painter.setBackground(BgBrush);

    QLineF linef = {};
    if (get_line(linef))
        return;

    int a_i = ui->AlgorithmsChooser->currentIndex();
    switch (a_i) {
    case LIB:
    {
        draw_line_lib(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    case DDA:
    {
        draw_line_dda(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    case BRESENHAM_D:
    {
        draw_line_bresenham_d(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    case BRESENHAM_I:
    {
        draw_line_bresenham_i(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    case BRESENHAM_ANTIALIASING:
    {
        draw_line_bresenham_antialiasing(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    case WU:
    {
        draw_line_Wu(painter, linef.p1(), linef.p2(), DRAW);
    } break;
    default:
    {
        qDebug() << "Something went wrong here.";
    }
    }

    ui->picture->setPixmap(pxp);
}


void MainWindow::on_ClearButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    pxp.fill(Qt::transparent);
    ui->picture->setPixmap(pxp);

    LineColor = QColor(0, 0, 0, 255);
    BackgroundColor = QColor(255, 255, 255, 255);

    set_bg_color_button_style();
    set_line_color_button_style();
    set_picture_style();
}


void MainWindow::on_DrawSunButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    QPainter painter(&this->pxp);
    QBrush BgBrush = QBrush(BackgroundColor);
    painter.setPen(LineColor);
    painter.setBackground(BgBrush);

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

    if (line_length == 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть равна 0.");
        return;
    }

    if (line_length < 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть отрицательной.");
        return;
    }

    if (qFabs(angle) < EPS) {
        ui->LabelCommunicator->setText("> Ошибка: шаг изменения угла наклона не может быть равен нулю.");
        return;
    }


    angle = angle * M_PI / 180;

    int a_i = ui->AlgorithmsChooser->currentIndex();
    switch (a_i) {
    case LIB:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_lib);
    } break;
    case DDA:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_dda);
    } break;
    case BRESENHAM_D:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_bresenham_d);
    } break;
    case BRESENHAM_I:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_bresenham_i);
    } break;
    case BRESENHAM_ANTIALIASING:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_bresenham_antialiasing);
    } break;
    case WU:
    {
        draw_spectrum(painter, line_length, angle, &draw_line_Wu);
    } break;
    default:
    {
        qDebug() << "Something went wrong here.";
    }
    }

    ui->picture->setPixmap(pxp);
}

unsigned long long current_time()
{
    struct timespec ts = {};
    clockid_t clkid;
    clkid = CLOCK_REALTIME;
    clock_gettime(clkid, &ts);

    return ts.tv_sec * 1000000000 + ts.tv_nsec;
}



#define N_TESTS 5000
void MainWindow::on_CompareTimeButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    QPainter painter(&this->pxp);
    QBrush BgBrush = QBrush(BackgroundColor);
    painter.setPen(LineColor);
    painter.setBackground(BgBrush);

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

    if (line_length == 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть равна 0.");
        return;
    }

    if (line_length < 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть отрицательной.");
        return;
    }

    if (qFabs(angle) < EPS) {
        ui->LabelCommunicator->setText("> Ошибка: шаг изменения угла наклона не может быть равен нулю.");
        return;
    }

    unsigned long long start_time;
    unsigned long long times[N_ALGORITHMS] = {};
    qreal avg_times[N_ALGORITHMS] = {};

    QPointF Start = {0, 0};
    QPointF End = {0, line_length};
    for (int i = 0; i < N_TESTS; ++i) {
        start_time = current_time();
        draw_line_bresenham_antialiasing(painter, Start, End, TIME);
        times[BRESENHAM_ANTIALIASING] += current_time() - start_time;

        start_time = current_time();
        draw_line_dda(painter, Start, End, TIME);
        times[DDA] += current_time() - start_time;

        start_time = current_time();
        draw_line_bresenham_d(painter, Start, End, TIME);
        times[BRESENHAM_D] += current_time() - start_time;

        start_time = current_time();
        draw_line_bresenham_i(painter, Start, End, TIME);
        times[BRESENHAM_I] += current_time() - start_time;


        start_time = current_time();
        draw_line_Wu(painter, Start, End, TIME);
        times[WU] += current_time() - start_time;

        rotate_point(End, Start, angle * M_PI / 180);
    }

    avg_times[DDA] = times[DDA] / (qreal)N_TESTS;
    avg_times[BRESENHAM_D] = times[BRESENHAM_D] / (qreal)N_TESTS;
    avg_times[BRESENHAM_I] = times[BRESENHAM_I] / (qreal)N_TESTS;
    avg_times[BRESENHAM_ANTIALIASING] = times[BRESENHAM_ANTIALIASING] / (qreal)N_TESTS;
    avg_times[WU] = times[WU] / (qreal)N_TESTS;

    qreal max_time = avg_times[1];
    qreal min_time = avg_times[1];
    for (int i = 2; i < N_ALGORITHMS; ++i) {
        if (avg_times[i] > max_time)
            max_time = avg_times[i];
        if (avg_times[i] < min_time)
            min_time = avg_times[i];
    }

    qreal steper = (max_time - min_time) * 0.1;

    QBarSet *set0 = new QBarSet("Алгоритмы построения отрезков");
    *set0 << avg_times[BRESENHAM_D]
          << avg_times[BRESENHAM_ANTIALIASING]
          << avg_times[BRESENHAM_I]
          << avg_times[DDA]
          << avg_times[WU];

    QBarSeries *series = new QBarSeries();
    series->append(set0);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(QString::asprintf("Среднее время построения отрезка длиной %.2lf, в наносекундах", line_length));
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QStringList categories;
    categories << "ЦДА" << "Брезенхем с действ." << "Брезенхем с целоч." << "Брезенхем с устр. ступенч." << "Ву";
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, max_time + steper);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    QDialog *Dialog = new QDialog(this);
    QGridLayout *layout = new QGridLayout;

    layout->addWidget(chartView);
    Dialog->setLayout(layout);
    Dialog->resize(420, 300);
    Dialog->show();
}

void MainWindow::on_CompareStearsButton_clicked()
{
    ui->LabelCommunicator->setText("> ");
    QPainter painter(&this->pxp);
    QBrush BgBrush = QBrush(BackgroundColor);
    painter.setPen(LineColor);
    painter.setBackground(BgBrush);

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

    if (line_length == 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть равна 0.");
        return;
    }

    if (line_length < 0) {
        ui->LabelCommunicator->setText("> Ошибка: длина отрезка не может быть отрицательной.");
        return;
    }

    if (qFabs(angle) < EPS) {
        ui->LabelCommunicator->setText("> Ошибка: шаг изменения угла наклона не может быть равен нулю.");
        return;
    }

    if (qFabs(angle) > 90) {
        ui->LabelCommunicator->setText("> Ошибка: ступенчатость анализируется до 90 градусов. Угол должен быть меньше 90");
        return;
    }


    QChartView *wdg = new QChartView;
    wdg->setMinimumWidth(1000);

    QChart *chrt = new QChart;
    chrt->setTitle("График ступенчатости");


    int line_width = 3;
    QLineSeries *dda_series = new QLineSeries();
    dda_series->setName("ЦДА");
    dda_series->setPen(QPen(Qt::yellow, line_width, Qt::SolidLine));

    QLineSeries *bresenham_d_series = new QLineSeries();
    bresenham_d_series->setName("Брезенхем на вещественных числах");
    bresenham_d_series->setPen(QPen(Qt::red, line_width, Qt::DashLine));

    QLineSeries *bresenham_i_series = new QLineSeries();
    bresenham_i_series->setName("Брезенхем на целых числах");
    bresenham_i_series->setPen(QPen(Qt::green, line_width, Qt::DashDotLine));

    QLineSeries *bresenham_antialiasing_series = new QLineSeries();
    bresenham_antialiasing_series->setName("Брезенхем с устранением ступенчатости");
    bresenham_antialiasing_series->setPen(QPen(Qt::blue, line_width, Qt::DashDotDotLine));

    QLineSeries *wu_series = new QLineSeries();
    wu_series->setName("By");
    wu_series->setPen(QPen(Qt::cyan, line_width, Qt::DotLine));


    QPointF Start = {0, 0};
    QPointF End = {0, line_length};

    int n_steps = qFloor(90 / angle);
    for(int i = 0; i <= n_steps; i += 1)
    {
        dda_series->append(i, draw_line_dda(painter, Start, End, STAIRS));
        bresenham_d_series->append(i, draw_line_bresenham_d(painter, Start, End, STAIRS));
        bresenham_i_series->append(i, draw_line_bresenham_i(painter, Start, End, STAIRS));
        bresenham_antialiasing_series->append(i, draw_line_bresenham_antialiasing(painter, Start, End, STAIRS));
        wu_series->append(i, draw_line_Wu(painter, Start, End, STAIRS));

        rotate_point(End, Start, angle * M_PI / 180);
    }


    chrt->addSeries(dda_series);
    chrt->addSeries(bresenham_d_series);
    chrt->addSeries(bresenham_i_series);
    chrt->addSeries(bresenham_antialiasing_series);
    chrt->addSeries(wu_series);

    chrt->createDefaultAxes();
    wdg->setChart(chrt);
    wdg->show();
}
