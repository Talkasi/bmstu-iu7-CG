#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPainterPath>
#include <QDoubleValidator>
#include <QtMath>
#include <QDebug>
#include <cstring>

#define EPS 1e-6

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QValidator *DoubleValidator = new QDoubleValidator;
    QLocale locale_double(QLocale::C);
    locale_double.setNumberOptions(QLocale::RejectGroupSeparator);
    DoubleValidator->setLocale(locale_double);

    QValidator *IntValidator = new QIntValidator;

    ui->InputFieldRotationAngle->setValidator(DoubleValidator);
    ui->InputFieldRotationRx->setValidator(IntValidator);
    ui->InputFieldRotationRy->setValidator(IntValidator);

    ui->InputFieldScalingKx->setValidator(DoubleValidator);
    ui->InputFieldScalingKy->setValidator(DoubleValidator);
    ui->InputFieldScalingMx->setValidator(IntValidator);
    ui->InputFieldScalingMy->setValidator(IntValidator);

    ui->InputFieldTransferDx->setValidator(IntValidator);
    ui->InputFieldTransferDy->setValidator(IntValidator);
}

void MainWindow::showEvent(QShowEvent *) {
    delete ui->GraphicsView->scene();
    QGraphicsScene *Scene = new QGraphicsScene;
    QRectF SceneRect = ui->GraphicsView->rect();
    SceneRect.setWidth(SceneRect.width() - spacing);
    SceneRect.setHeight(SceneRect.height() - spacing);
    Scene->setSceneRect(SceneRect);
    ui->GraphicsView->centerOn(SceneRect.width() / 2, SceneRect.height() / 2);
    ui->GraphicsView->setScene(Scene);
    CalcDefaultFigurePoints(Scene);
    DrawImage();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CalcDefaultFigurePoints(QGraphicsScene *Scene)
{
    Figure DefaultFigure = {};
    qreal SideLength = 140;
    qreal EllipseWidth = 45;

    qreal wHalf = Scene->width() / 2 - SideLength / 2;
    qreal hHalf = Scene->height() / 2 - SideLength / 2;
    DefaultFigure.MainPoints[0] = {wHalf + 0, hHalf + 0};
    DefaultFigure.MainPoints[1] = {wHalf + 0, hHalf + SideLength};
    DefaultFigure.MainPoints[2] = {wHalf + SideLength, hHalf + SideLength};
    DefaultFigure.MainPoints[3] = {wHalf + SideLength, hHalf + 0};
    DefaultFigure.MainPoints[4] = {wHalf + SideLength / 2, hHalf + 0};


    qreal dx = EllipseWidth * 2 / qSqrt(5);
    qreal dy = EllipseWidth / qSqrt(5);
    DefaultFigure.EllipsePoints[0] = {wHalf + SideLength / 4 + dx, hHalf + SideLength / 2 + dy};
    DefaultFigure.EllipsePoints[1] = {wHalf + SideLength * 3 / 4 - dx, hHalf + SideLength / 2 + dy};
    DefaultFigure.EllipsePoints[2] = {wHalf + SideLength / 2, hHalf + SideLength - dx};

    FigureHistory.append(DefaultFigure);
    CurrentFHIndex = 0;
}

int FiguresAreEqual(Figure &A, Figure &B)
{
    if (std::memcmp(A.EllipsePoints, B.EllipsePoints, sizeof(QPointF) * N_ELLIPSE_POINTS) != 0)
        return 0;

    if (std::memcmp(A.MainPoints, B.MainPoints, sizeof(QPointF) * N_MAIN_POINTS) != 0)
        return 0;

    return 1;
}


void MainWindow::DrawFigure(const Figure &Figure, QGraphicsScene *Scene, QPen &Pen)
{
    for (int i = 0; i < N_MAIN_POINTS - 1; ++i) {
        Scene->addLine(QLineF(Figure.MainPoints[i],
                              Figure.MainPoints[(i + 1) % (N_MAIN_POINTS - 1)]), Pen);
    }

    Scene->addLine(QLineF(Figure.MainPoints[N_MAIN_POINTS - 1],
                          Figure.MainPoints[1]), Pen);
    Scene->addLine(QLineF(Figure.MainPoints[N_MAIN_POINTS - 1],
                          Figure.MainPoints[2]), Pen);

    QPainterPath Path;
    Path.moveTo(Figure.MainPoints[1].x(), Figure.MainPoints[1].y());
    Path.quadTo(Figure.EllipsePoints[0], Figure.MainPoints[4]);
    Path.quadTo(Figure.EllipsePoints[1], Figure.MainPoints[2]);
    Path.quadTo(Figure.EllipsePoints[2], Figure.MainPoints[1]);
    Scene->addPath(Path, Pen);

    QPen PointPen = QPen(Qt::darkGreen, 3);
    Scene->addEllipse(Scene->width() / 2, Scene->height() / 2, 2, 2, PointPen);


    qreal x_left = Figure.MainPoints[0].x();
    qreal x_right = Figure.MainPoints[0].x();
    qreal y_top = Figure.MainPoints[0].y();
    qreal y_bottom = Figure.MainPoints[0].y();
    for (int i = 1; i < N_MAIN_POINTS; ++i) {
        if (Figure.MainPoints[i].x() < x_left)
            x_left = Figure.MainPoints[i].x();

        if (Figure.MainPoints[i].x() > x_right)
            x_right = Figure.MainPoints[i].x();

        if (Figure.MainPoints[i].y() < y_top)
            y_top = Figure.MainPoints[i].y();

        if (Figure.MainPoints[i].y() > y_bottom)
            y_bottom = Figure.MainPoints[i].y();
    }

    for (int i = 0; i < N_ELLIPSE_POINTS; ++i) {
        if (Figure.EllipsePoints[i].x() < x_left)
            x_left = Figure.EllipsePoints[i].x();

        if (Figure.EllipsePoints[i].x() > x_right)
            x_right = Figure.EllipsePoints[i].x();

        if (Figure.EllipsePoints[i].y() < y_top)
            y_top = Figure.EllipsePoints[i].y();

        if (Figure.EllipsePoints[i].y() > y_bottom)
            y_bottom = Figure.EllipsePoints[i].y();
    }


    ui->LabelCenterScene->setText(QString("Центр сцены находится на координатах (%1,%2), ").arg(\
                                          QString::number(qRound(Scene->width() / 2)),
                                          QString::number(qRound(Scene->height() / 2))) +
                                  QString("центр фигуры — на координатах (%1,%2).").arg(\
                                          QString::number(qRound(x_left + (x_right - x_left) / 2)),
                                          QString::number(qRound(y_top + (y_bottom - y_top) / 2))));
}

void MainWindow::on_ButtonDrawDefaultImage_clicked()
{
    CurrentFHIndex = 0;
    FigureHistory.resize(1);
    DrawImage();
}

void MainWindow::DrawImage()
{
    ui->LabelCommunicator->setText(">");
    ui->ButtonPrevious->setEnabled(CurrentFHIndex > 0);
    ui->ButtonNext->setEnabled(CurrentFHIndex < FigureHistory.count() - 1);

    QGraphicsScene *Scene = ui->GraphicsView->scene();
    Scene->clear();

    if (CurrentFHIndex > 0) {
        QPen GreyPen = QPen(Qt::gray, 3);
        DrawFigure(FigureHistory.first(), Scene, GreyPen);
    }

    QPen BlackPen = QPen(Qt::black, 2);
    DrawFigure(FigureHistory.at(CurrentFHIndex), Scene, BlackPen);
}


bool FigureFitsInScene(Figure &F, QRectF Rect)
{
    for (int i = 0; i < N_MAIN_POINTS; ++i) {
        qreal x = F.MainPoints[i].x();
        qreal y = F.MainPoints[i].y();

        if (!(Rect.x() <= x && x <= Rect.x() + Rect.width()) ||
            !(Rect.y() <= y && y <= Rect.y() + Rect.height()))
            return false;
    }

    for (int i = 0; i < N_ELLIPSE_POINTS; ++i) {
        qreal x = F.EllipsePoints[i].x();
        qreal y = F.EllipsePoints[i].y();

        if (!(Rect.x() <= x && x <= Rect.x() + Rect.width()) ||
            !(Rect.y() <= y && y <= Rect.y() + Rect.height()))
            return false;
    }

    return true;
}


void MainWindow::on_ButtonTransfer_clicked()
{
    if (ui->InputFieldTransferDx->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Dx пустое.");
        return;
    }

    if (ui->InputFieldTransferDy->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Dy пустое.");
        return;
    }

    int dx = ui->InputFieldTransferDx->text().toInt();
    int dy = ui->InputFieldTransferDy->text().toInt();

    if (dx != 0 || dy != 0) {
        FigureHistory.resize(CurrentFHIndex + 1);
        Figure CurrentFigure = FigureHistory.last();
        for (int i = 0; i < N_MAIN_POINTS; ++i) {
            CurrentFigure.MainPoints[i].setX(CurrentFigure.MainPoints[i].x() + dx);
            CurrentFigure.MainPoints[i].setY(CurrentFigure.MainPoints[i].y() + dy);
        }

        for (int i = 0; i < N_ELLIPSE_POINTS; ++i) {
            CurrentFigure.EllipsePoints[i].setX(CurrentFigure.EllipsePoints[i].x() + dx);
            CurrentFigure.EllipsePoints[i].setY(CurrentFigure.EllipsePoints[i].y() + dy);
        }

        FigureHistory.append(CurrentFigure);
        ++CurrentFHIndex;

        DrawImage();
    }
}


void MainWindow::on_ButtonScaling_clicked()
{
    if (ui->InputFieldScalingMx->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Mx пустое.");
        return;
    }

    if (ui->InputFieldScalingMy->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле My пустое.");
        return;
    }

    if (ui->InputFieldScalingKx->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Kx пустое.");
        return;
    }

    if (ui->InputFieldScalingKy->text().length() == 0)
    {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Ky пустое.");
        return;
    }

    int Mx = ui->InputFieldScalingMx->text().toInt();
    int My = ui->InputFieldScalingMy->text().toInt();

    bool validatorX;
    bool validatorY;

    qreal kx = ui->InputFieldScalingKx->text().toDouble(&validatorX);
    qreal ky = ui->InputFieldScalingKy->text().toDouble(&validatorY);

    if (validatorX == false) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Данные поля Kx не могут быть переведены в вещественный тип.");
    }

    if (validatorY == false) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Данные поля Ky не могут быть переведены в вещественный тип.");
    }

    if (!(1 - EPS < kx && kx < 1 + EPS) ||
        !(1 - EPS < ky && ky < 1 + EPS)) {
        FigureHistory.resize(CurrentFHIndex + 1);
        Figure CurrentFigure = FigureHistory.last();

        for (int i = 0; i < N_MAIN_POINTS; ++i) {
            CurrentFigure.MainPoints[i].setX(CurrentFigure.MainPoints[i].x() * kx + (1 - kx) * Mx);
            CurrentFigure.MainPoints[i].setY(CurrentFigure.MainPoints[i].y() * ky + (1 - ky) * My);
        }

        for (int i = 0; i < N_ELLIPSE_POINTS; ++i) {
            CurrentFigure.EllipsePoints[i].setX(CurrentFigure.EllipsePoints[i].x() * kx + (1 - kx) * Mx);
            CurrentFigure.EllipsePoints[i].setY(CurrentFigure.EllipsePoints[i].y() * ky + (1 - ky) * My);
        }

        FigureHistory.append(CurrentFigure);
        ++CurrentFHIndex;

        DrawImage();
    }
}


void MainWindow::on_ButtonRotation_clicked()
{
    if (ui->InputFieldRotationRx->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Rx пустое.");
        return;
    }

    if (ui->InputFieldRotationRy->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Ry пустое.");
        return;
    }

    if (ui->InputFieldRotationAngle->text().length() == 0) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Поле Угол пустое.");
        return;
    }

    bool validator;

    int xc = ui->InputFieldRotationRx->text().toInt();
    int yc = ui->InputFieldRotationRy->text().toInt();
    qreal t = ui->InputFieldRotationAngle->text().toDouble(&validator) * M_PI / 180;

    if (validator == false) {
        ui->LabelCommunicator->setText("> Произошла ошибка. Данные поля Угол не могут быть переведены в вещественный тип.");
        return;
    }

    if (qFabs(t) > EPS) {
        FigureHistory.resize(CurrentFHIndex + 1);
        Figure CurrentFigure = FigureHistory.last();

        for (int i = 0; i < N_MAIN_POINTS; ++i) {
            qreal x = CurrentFigure.MainPoints[i].x();
            qreal y = CurrentFigure.MainPoints[i].y();

            qreal NewX = xc + (x - xc) * qCos(t) - (y - yc) * qSin(t);
            qreal NewY = yc + (y - yc) * qCos(t) + (x - xc) * qSin(t);

            CurrentFigure.MainPoints[i].setX(NewX);
            CurrentFigure.MainPoints[i].setY(NewY);
        }

        for (int i = 0; i < N_ELLIPSE_POINTS; ++i) {
            qreal x = CurrentFigure.EllipsePoints[i].x();
            qreal y = CurrentFigure.EllipsePoints[i].y();

            qreal NewX = xc + (x - xc) * qCos(t) - (y - yc) * qSin(t);
            qreal NewY = yc + (y - yc) * qCos(t) + (x - xc) * qSin(t);

            CurrentFigure.EllipsePoints[i].setX(NewX);
            CurrentFigure.EllipsePoints[i].setY(NewY);
        }

        FigureHistory.append(CurrentFigure);
        ++CurrentFHIndex;
    }

    DrawImage();
}


void MainWindow::on_InputFieldTransferDx_returnPressed()
{
    ui->InputFieldTransferDy->setFocus();
}

void MainWindow::on_InputFieldTransferDy_returnPressed()
{
    ui->ButtonTransfer->click();
}

void MainWindow::on_InputFieldScalingMx_returnPressed()
{
    ui->InputFieldScalingMy->setFocus();
}

void MainWindow::on_InputFieldScalingMy_returnPressed()
{
    ui->InputFieldScalingKx->setFocus();
}

void MainWindow::on_InputFieldScalingKx_returnPressed()
{
    ui->InputFieldScalingKy->setFocus();
}

void MainWindow::on_InputFieldScalingKy_returnPressed()
{
    ui->ButtonScaling->click();
}

void MainWindow::on_InputFieldRotationRx_returnPressed()
{
    ui->InputFieldRotationRy->setFocus();
}

void MainWindow::on_InputFieldRotationRy_returnPressed()
{
    ui->InputFieldRotationAngle->setFocus();
}

void MainWindow::on_InputFieldRotationAngle_returnPressed()
{
    ui->ButtonRotation->click();
}

void MainWindow::on_ButtonPrevious_clicked()
{
    --CurrentFHIndex;
    DrawImage();
}


void MainWindow::on_ButtonNext_clicked()
{
    ++CurrentFHIndex;
    DrawImage();
}

