#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "language.h"

#include <QDoubleValidator>
#include <QtMath>
#include <QGraphicsTextItem>

#define SUCCESS 0
#define INPUT_ERROR 1
#define EPS 1e-6


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->HeaderPointLabel->setText(HEADER_POINT_LABEL);
    ui->HeaderPointsTableLabel->setText(HEADER_TABLE_LABEL);
    ui->HeaderTriangleLabel->setText(HEADER_TRIANGLE_LABEL);

    ui->ButtonAddPoint->setText(BUTTON_ADD_POINT);
    ui->ButtonDeleteAllPoints->setText(BUTTON_DELETE_ALL_POINTS);
    ui->ButtonDeleteCurrentPoint->setText(BUTTON_DELETE_CURRENT_POINT);
    ui->ButtonCompute->setText(BUTTON_COMPUTE);
    ui->ButtonClearTriangle->setText(BUTTON_CLEAR_TRIANGLE);

    QValidator *DoubleValidator = new QDoubleValidator;
    QLocale locale(QLocale::C);
    locale.setNumberOptions(QLocale::RejectGroupSeparator);
    DoubleValidator->setLocale(locale);

    ui->InputFieldPointX->setValidator(DoubleValidator);
    ui->InputFieldPointY->setValidator(DoubleValidator);
    ui->InputFieldTriangleX0->setValidator(DoubleValidator);
    ui->InputFieldTriangleY0->setValidator(DoubleValidator);
    ui->InputFieldTriangleX1->setValidator(DoubleValidator);
    ui->InputFieldTriangleY1->setValidator(DoubleValidator);
    ui->InputFieldTriangleX2->setValidator(DoubleValidator);
    ui->InputFieldTriangleY2->setValidator(DoubleValidator);

    QHeaderView *VerticalHeader = ui->TablePoints->verticalHeader();
    VerticalHeader->setStyleSheet("background-color: #8E847F;"
                                  "color: #3D201C;"
                                  "border: none;");
    ui->TablePoints->setVerticalHeader(VerticalHeader);
    ui->TablePoints->verticalHeader()->setVisible(true);
    ui->TablePoints->setRowCount(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}


int MainWindow::GetPointFromLineEdits(QPointF *NewPoint) {
    bool Validate;

    qreal X = ui->InputFieldPointX->text().toDouble(&Validate);
    if (Validate == false) {
        return INPUT_ERROR;
    }

    qreal Y = ui->InputFieldPointY->text().toDouble(&Validate);
    if (Validate == false) {
        return INPUT_ERROR;
    }

    NewPoint->setX(X);
    NewPoint->setY(Y);
    return SUCCESS;
}

void MainWindow::ClearNeeded()
{
    ui->LabelCommunicator->setText(DEFAULT_STRING_START);
    delete ui->GraphicsView->scene();
}

void MainWindow::on_XlineEditTextChanged(QString String)
{
    ClearNeeded();
    int row = ui->TablePoints->currentRow();
    if (row >= 0)
    {
        QLineEdit *XEdit = qobject_cast<QLineEdit*>(ui->TablePoints->cellWidget(row, 0));

        bool validator;
        String.toDouble(&validator);

        XEdit->setText(String);
        if (validator == false) {
            ui->LabelCommunicator->setText(INVALID_EDIT_VALIDATOR_ERR);
            return;
        }

        if (XEdit->text().length() == 0) {
            ui->LabelCommunicator->setText(INVALID_EDIT_EMPTY_FIELD);
            return;
        }
    }
}

void MainWindow::on_YlineEditTextChanged(QString String)
{
    ClearNeeded();
    int row = ui->TablePoints->currentRow();
    if (row >= 0)
    {
        QLineEdit *YEdit = qobject_cast<QLineEdit*>(ui->TablePoints->cellWidget(row, 1));

        bool validator;

        String.toDouble(&validator);
        YEdit->setText(String);
        if (validator == false) {
            ui->LabelCommunicator->setText(INVALID_EDIT_VALIDATOR_ERR);
            return;
        }

        if (YEdit->text().length() == 0) {
            ui->LabelCommunicator->setText(INVALID_EDIT_EMPTY_FIELD);
            return;
        }
    }
}

void MainWindow::on_ButtonAddPoint_clicked()
{
    ClearNeeded();
    ui->TablePoints->setCurrentItem(nullptr);

    QPointF NewPoint;
    if (GetPointFromLineEdits(&NewPoint) == SUCCESS) {
        QLineEdit *XEdit = new QLineEdit(ui->TablePoints);
        QLineEdit *YEdit = new QLineEdit(ui->TablePoints);

        QLocale locale(QLocale::C);
        locale.setNumberOptions(QLocale::RejectGroupSeparator);

        QValidator *DoubleValidatorX = new QDoubleValidator(XEdit);
        QValidator *DoubleValidatorY = new QDoubleValidator(YEdit);
        DoubleValidatorX->setLocale(locale);
        DoubleValidatorY->setLocale(locale);
        XEdit->setValidator(DoubleValidatorX);
        YEdit->setValidator(DoubleValidatorY);

        int RowCount = ui->TablePoints->rowCount();
        ui->TablePoints->setRowCount(RowCount + 1);

        connect(XEdit, SIGNAL(textChanged(QString)), this, SLOT(on_XlineEditTextChanged(QString)));
        connect(YEdit, SIGNAL(textChanged(QString)), this, SLOT(on_YlineEditTextChanged(QString)));

        XEdit->setText(QString::number(NewPoint.x()));
        YEdit->setText(QString::number(NewPoint.y()));

        ui->TablePoints->setCellWidget(RowCount, 0, XEdit);
        ui->TablePoints->setCellWidget(RowCount, 1, YEdit);

        ui->InputFieldPointX->clear();
        ui->InputFieldPointY->clear();
        ui->InputFieldPointX->setFocus();
    } else {
        if (ui->InputFieldPointX->text().length() == 0 ||
            ui->InputFieldPointY->text().length() == 0)
            ui->LabelCommunicator->setText(INVALID_POINT_EMPTY_FIELD);
        else
            ui->LabelCommunicator->setText(INVALID_POINT_VALIDATOR_ERR);
        return;
    }
}

void MainWindow::on_ButtonDeleteAllPoints_clicked()
{
    ClearNeeded();
    if (ui->TablePoints->rowCount()) {
        ui->TablePoints->setRowCount(0);
    } else {
        ui->LabelCommunicator->setText(INVALID_TABLE_CLEAR);
    }
}

void MainWindow::on_ButtonDeleteCurrentPoint_clicked()
{
    ClearNeeded();
    ui->TablePoints->removeRow(ui->TablePoints->currentRow());
}

QPointF ComputeCircleCenter(QPointF &A, QPointF &B, QPointF &C)
{
    QPointF CircleCenter;
    CircleCenter.setX(((qPow(A.x(), 2) + qPow(A.y(), 2)) * (B.y() - C.y()) +
                       (qPow(B.x(), 2) + qPow(B.y(), 2)) * (C.y() - A.y()) +
                       (qPow(C.x(), 2) + qPow(C.y(), 2)) * (A.y() - B.y())) /
                       (2 * (A.x() * (B.y() - C.y()) + B.x() * (C.y() - A.y()) + C.x() * (A.y() - B.y()))));
    CircleCenter.setY(((qPow(A.x(), 2) + qPow(A.y(), 2)) * (C.x() - B.x()) +
                       (qPow(B.x(), 2) + qPow(B.y(), 2)) * (A.x() - C.x()) +
                       (qPow(C.x(), 2) + qPow(C.y(), 2)) * (B.x() - A.x())) /
                       (2 * (A.x() * (B.y() - C.y()) + B.x() * (C.y() - A.y()) + C.x() * (A.y() - B.y()))));
    return CircleCenter;
}

bool PointsAreOnTheSameLine(QPointF &A, QPointF &B, QPointF &C)
{
    return qFabs(A.x() * (B.y() - C.y()) +
                 B.x() * (C.y() - A.y()) +
                 C.x() * (A.y() - B.y())) < EPS;
}

qreal ComputeRadius(NeededCircleDesc &Circle) {
    return qSqrt(qPow(Circle.Points[0].Coordinates.x() - Circle.Center.x(), 2) +
                 qPow(Circle.Points[0].Coordinates.y() - Circle.Center.y(), 2));
}

bool PointsAreEqual(QPointF A, QPointF B) {
    return ((qFabs(A.x() - B.x()) < EPS) && (qFabs(A.y() - B.y()) < EPS));
}

void MainWindow::DrawAnswer(QPointF *Triangle, NeededCircleDesc &Circle) {
    delete ui->GraphicsView->scene();
    QGraphicsScene *Scene = new QGraphicsScene();

    qreal spacing = 50;
    int dotRadius = 3;
    QRectF SceneRect = ui->GraphicsView->rect();
    SceneRect.setWidth(SceneRect.width() - spacing * 2);
    SceneRect.setHeight(SceneRect.height() - spacing * 2);

    qreal y_top = Triangle[0].y();
    qreal y_bottom = Triangle[0].y();
    qreal x_right = Triangle[0].x();
    qreal x_left = Triangle[0].x();

    for (int i = 1; i < POINTS_N; ++i) {
        if (y_top < Triangle[i].y())
            y_top = Triangle[i].y();

        if (y_bottom > Triangle[i].y())
            y_bottom = Triangle[i].y();

        if (x_right < Triangle[i].x())
            x_right = Triangle[i].x();

        if (x_left > Triangle[i].x())
            x_left = Triangle[i].x();
    }

    qreal CircleRadius = ComputeRadius(Circle);
    if (y_top < Circle.Center.y() + CircleRadius)
        y_top = Circle.Center.y() + CircleRadius;

    if (y_bottom > Circle.Center.y() - CircleRadius)
        y_bottom = Circle.Center.y() - CircleRadius;

    if (x_right < Circle.Center.x() + CircleRadius)
        x_right = Circle.Center.x() + CircleRadius;

    if (x_left > Circle.Center.x() - CircleRadius)
        x_left = Circle.Center.x() - CircleRadius;


    qreal ky = SceneRect.height() / (y_top - y_bottom);
    qreal kx = SceneRect.width() / (x_right - x_left);

    if (kx > ky) {
        kx = ky;
        ky = -ky;
    } else {
        ky = -kx;
        kx = kx;
    }

    QPointF TriangleScaled[POINTS_N] = {{Triangle[0].x() * kx, Triangle[0].y() * ky},
                                        {Triangle[1].x() * kx, Triangle[1].y() * ky},
                                        {Triangle[2].x() * kx, Triangle[2].y() * ky}};

    NeededCircleDesc CircleScaled = {{Circle.Center.x() * kx, Circle.Center.y() * ky},

                                     {Circle.TriangleLine[0],
                                      Circle.TriangleLine[1]},

                                    {{{Circle.Points[0].Coordinates.x() * kx,
                                       Circle.Points[0].Coordinates.y() * ky}, Circle.Points[0].n},
                                     {{Circle.Points[1].Coordinates.x() * kx,
                                       Circle.Points[1].Coordinates.y() * ky}, Circle.Points[1].n},
                                     {{Circle.Points[2].Coordinates.x() * kx,
                                       Circle.Points[2].Coordinates.y() * ky}, Circle.Points[2].n}},

                                    Circle.AngleOY};

    QPen TrianglePen(Qt::darkGreen, 5);
    QPen CirclePen(Qt::darkRed, 5);
    QPen DotPen(Qt::darkRed, 6);
    QPen LinePen(Qt::yellow, 3, Qt::DashDotLine);

    // Triangle lines
    Scene->addLine(QLineF(TriangleScaled[0].x(), TriangleScaled[0].y(),
                          TriangleScaled[1].x(), TriangleScaled[1].y()), TrianglePen);
    Scene->addLine(QLineF(TriangleScaled[1].x(), TriangleScaled[1].y(),
                          TriangleScaled[2].x(), TriangleScaled[2].y()), TrianglePen);
    Scene->addLine(QLineF(TriangleScaled[0].x(), TriangleScaled[0].y(),
                          TriangleScaled[2].x(), TriangleScaled[2].y()), TrianglePen);

    // Triangle points
    QGraphicsTextItem *text;
    text = Scene->addText(QString("A (%1,%2)").arg(QString::number(Triangle[0].x()),
                                                   QString::number(Triangle[0].y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(TriangleScaled[0].x(), TriangleScaled[0].y());

    text = Scene->addText(QString("B (%1,%2)").arg(QString::number(Triangle[1].x()),
                                                   QString::number(Triangle[1].y())), QFont());
    text->setDefaultTextColor(Qt::black);
    text->setPos(TriangleScaled[1].x(), TriangleScaled[1].y());

    text = Scene->addText(QString("C (%1,%2)").arg(QString::number(Triangle[2].x()),
                                                   QString::number(Triangle[2].y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(TriangleScaled[2].x(), TriangleScaled[2].y());

    // Circle
    qreal CircleScaledRadius = ComputeRadius(CircleScaled);
    Scene->addEllipse(CircleScaled.Center.x() - CircleScaledRadius,
                      CircleScaled.Center.y() - CircleScaledRadius,
                      CircleScaledRadius * 2, CircleScaledRadius * 2, CirclePen);

    // Circle points
    text = Scene->addText(QString("%1 (%2,%3)").arg(QString::number(Circle.Points[0].n),
                                                    QString::number(Circle.Points[0].Coordinates.x()),
                                                    QString::number(Circle.Points[0].Coordinates.y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(CircleScaled.Points[0].Coordinates.x(), CircleScaled.Points[0].Coordinates.y());
    Scene->addEllipse(CircleScaled.Points[0].Coordinates.x() - dotRadius,
                      CircleScaled.Points[0].Coordinates.y() - dotRadius,
                      dotRadius * 2, dotRadius * 2, DotPen);

    text = Scene->addText(QString("%1 (%2,%3)").arg(QString::number(Circle.Points[1].n),
                                                    QString::number(Circle.Points[1].Coordinates.x()),
                                                    QString::number(Circle.Points[1].Coordinates.y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(CircleScaled.Points[1].Coordinates.x(), CircleScaled.Points[1].Coordinates.y());
    Scene->addEllipse(CircleScaled.Points[1].Coordinates.x() - dotRadius,
                      CircleScaled.Points[1].Coordinates.y() - dotRadius,
                      dotRadius * 2, dotRadius * 2, DotPen);

    text = Scene->addText(QString("%1 (%2,%3)").arg(QString::number(Circle.Points[2].n),
                                                    QString::number(Circle.Points[2].Coordinates.x()),
                                                    QString::number(Circle.Points[2].Coordinates.y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(CircleScaled.Points[2].Coordinates.x(), CircleScaled.Points[2].Coordinates.y());
    Scene->addEllipse(CircleScaled.Points[2].Coordinates.x() - dotRadius,
                      CircleScaled.Points[2].Coordinates.y() - dotRadius,
                      dotRadius * 2, dotRadius * 2, DotPen);

    // Needed line
    text = Scene->addText(QString("O (%1,%2)").arg(QString::number(Circle.Center.x()),
                                                 QString::number(Circle.Center.y())));
    text->setDefaultTextColor(Qt::black);
    text->setPos(CircleScaled.Center.x(), CircleScaled.Center.y());
    Scene->addEllipse(CircleScaled.Center.x() - dotRadius,
                      CircleScaled.Center.y() - dotRadius,
                      dotRadius * 2, dotRadius * 2, DotPen);

    QPointF TrianglePointNeeded = TriangleScaled[CircleScaled.TriangleLine[0]];
    if (PointsAreEqual(CircleScaled.Center, TrianglePointNeeded))
        TrianglePointNeeded = TriangleScaled[CircleScaled.TriangleLine[1]];

    Scene->addLine(QLineF(CircleScaled.Center.x(),
                          CircleScaled.Center.y(),
                          TrianglePointNeeded.x(),
                          TrianglePointNeeded.y()),
                   LinePen);


    ui->GraphicsView->setScene(Scene);
}

int MainWindow::GetAndCheckPointsInput(QVector<UsersPoint> *PointsVector)
{
    for (int row = 0; row < ui->TablePoints->rowCount(); ++row) {
        QLineEdit *XEdit = qobject_cast<QLineEdit*>(ui->TablePoints->cellWidget(row, 0));
        QLineEdit *YEdit = qobject_cast<QLineEdit*>(ui->TablePoints->cellWidget(row, 1));

        if (XEdit->text().length() == 0 || YEdit->text().length() == 0) {
            ui->LabelCommunicator->setText(INVAILID_NTH_POINT_EMPTY_START + QString::number(row + 1) + INVAILID_NTH_POINT_EMPTY_END);
            return INPUT_ERROR;
        }

        bool validatorX;
        bool validatorY;
        qreal X = XEdit->text().toDouble(&validatorX);
        qreal Y = YEdit->text().toDouble(&validatorY);

        if (validatorX == false || validatorY == false) {
            ui->LabelCommunicator->setText(INVAILID_NTH_POINT_VALID_START + QString::number(row + 1) + INVAILID_NTH_POINT_VALID_END);
            return INPUT_ERROR;
        }

        PointsVector->append({{X, Y}, row + 1});
    }

    if (PointsVector->count() < 3) {
        ui->LabelCommunicator->setText(NOT_ENOUGH_POINTS_FOR_CIRCLE);
        return INPUT_ERROR;
    }

    return SUCCESS;
}

int MainWindow::GetAndCheckTriangleInput(QPointF *Triangle)
{
    if (ui->InputFieldTriangleX0->text().length() == 0 ||
        ui->InputFieldTriangleY0->text().length() == 0) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_EMPTY_FIELD_START "A" INVALID_TRIANGLE_EMPTY_FIELD_END);
        return INPUT_ERROR;
    }

    if (ui->InputFieldTriangleX1->text().length() == 0 ||
        ui->InputFieldTriangleY1->text().length() == 0) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_EMPTY_FIELD_START "B" INVALID_TRIANGLE_EMPTY_FIELD_END);
        return INPUT_ERROR;
    }

    if (ui->InputFieldTriangleX2->text().length() == 0 ||
        ui->InputFieldTriangleY2->text().length() == 0) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_EMPTY_FIELD_START "C" INVALID_TRIANGLE_EMPTY_FIELD_END);
        return INPUT_ERROR;
    }

    bool validator[6];
    QPointF tmp[3];

    tmp[0].setX(ui->InputFieldTriangleX0->text().toDouble(&validator[0]));
    tmp[0].setY(ui->InputFieldTriangleY0->text().toDouble(&validator[1]));
    tmp[1].setX(ui->InputFieldTriangleX1->text().toDouble(&validator[2]));
    tmp[1].setY(ui->InputFieldTriangleY1->text().toDouble(&validator[3]));
    tmp[2].setX(ui->InputFieldTriangleX2->text().toDouble(&validator[4]));
    tmp[2].setY(ui->InputFieldTriangleY2->text().toDouble(&validator[5]));

    if (validator[0] == false || validator[1] == false) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_VAL_ERR_START "A" INVALID_TRIANGLE_VAL_ERR_END);
        return INPUT_ERROR;
    }

    if (validator[2] == false || validator[3] == false) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_VAL_ERR_START "B" INVALID_TRIANGLE_VAL_ERR_END);
        return INPUT_ERROR;
    }

    if (validator[4] == false || validator[5] == false) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE_VAL_ERR_START "C" INVALID_TRIANGLE_VAL_ERR_END);
        return INPUT_ERROR;
    }

    Triangle[0] = tmp[0];
    Triangle[1] = tmp[1];
    Triangle[2] = tmp[2];

    if (PointsAreOnTheSameLine(Triangle[0], Triangle[1], Triangle[2])) {
        ui->LabelCommunicator->setText(INVALID_TRIANGLE);
        return INPUT_ERROR;
    }

    return SUCCESS;
}

qreal ComputeAngleWithOY(QPointF &A, QPointF &B) {
    qreal x1;
    qreal y1;
    if (qFabs(A.x() - B.x()) < EPS) {
        if (A.y() - B.y() > EPS)
            y1 = A.y() - B.y();
        else
            y1 = B.y() - A.y();

        x1 = 0;
    } else if (A.x() - B.x() > EPS) {
        y1 = A.y() - B.y();
        x1 = A.x() - B.x();
    } else {
        y1 = B.y() - A.y();
        x1 = B.x() - A.x();
    }

    qreal x2 = 0;
    qreal y2 = 1;

    return qAcos((x1 * x2 + y1 * y2) / (qSqrt(x1 * x1 + y1 * y1) * qSqrt(x2 * x2 + y2 * y2))) * 180 / M_PI;
}

void MainWindow::on_ButtonCompute_clicked()
{
    ClearNeeded();
    QVector<UsersPoint> PointsVector;
    QVector<NeededCircleDesc> NeededCirclesVector;
    QPointF Triangle[POINTS_N];

    if (GetAndCheckPointsInput(&PointsVector) != SUCCESS)
        return;

    if (GetAndCheckTriangleInput(Triangle) != SUCCESS)
        return;

    for (int i = 0; i < PointsVector.count() - 2; ++i) {
        for (int j = i + 1; j < PointsVector.count() - 1; ++j) {
            for (int k = j + 1; k < PointsVector.count(); ++k) {
                NeededCircleDesc NewCircleDesc = {};
                NewCircleDesc.Points[0] = PointsVector.at(i);
                NewCircleDesc.Points[1] = PointsVector.at(j);
                NewCircleDesc.Points[2] = PointsVector.at(k);

                if (!PointsAreOnTheSameLine(NewCircleDesc.Points[0].Coordinates,
                                            NewCircleDesc.Points[1].Coordinates,
                                            NewCircleDesc.Points[2].Coordinates)) {
                    NewCircleDesc.Center = ComputeCircleCenter(NewCircleDesc.Points[0].Coordinates,
                                                               NewCircleDesc.Points[1].Coordinates,
                                                               NewCircleDesc.Points[2].Coordinates);

                    for (int tr_i = 0; tr_i < POINTS_N - 1; ++tr_i) {
                        for (int tr_j = tr_i + 1; tr_j < POINTS_N; ++tr_j) {
                            if (PointsAreOnTheSameLine(Triangle[tr_i], Triangle[tr_j], NewCircleDesc.Center)) {
                                NewCircleDesc.TriangleLine[0] = tr_i;
                                NewCircleDesc.TriangleLine[1] = tr_j;

                                QPointF TrianglePointNeeded = Triangle[tr_i];
                                if (PointsAreEqual(NewCircleDesc.Center, TrianglePointNeeded))
                                    TrianglePointNeeded = Triangle[tr_j];

                                NewCircleDesc.AngleOY = ComputeAngleWithOY(NewCircleDesc.Center, TrianglePointNeeded);
                                NeededCirclesVector.append(NewCircleDesc);
                            }
                        }
                    }
                }
            }
        }
    }

    if (NeededCirclesVector.count()) {
        qreal MinCircleAngle = NeededCirclesVector.at(0).AngleOY;
        NeededCircleDesc ResultCircleCenterDesc = NeededCirclesVector.at(0);
        for (int i = 1; i < NeededCirclesVector.count(); ++i) {
            qreal Angle = NeededCirclesVector.at(i).AngleOY;
            if (Angle < MinCircleAngle) {
                MinCircleAngle = Angle;
                ResultCircleCenterDesc = NeededCirclesVector.at(i);
            }
        }

        ui->LabelCommunicator->setText("Cреди всех найденных окружностей, которые могут быть построены хотя бы на трех данных точках "
                                       " и центр каждой из которых лежит на прямой, проходящей через сторону данного треугольника, была выбрана та,"
                                       " у которой угол между вышеуказанной прямой и осью Oy минимален."
                                       " Она построена по точкам " +
                                       QString("%1 (%2,%3), ").arg(QString::number(ResultCircleCenterDesc.Points[0].n),
                                                                   QString::number(ResultCircleCenterDesc.Points[0].Coordinates.x()),
                                                                   QString::number(ResultCircleCenterDesc.Points[0].Coordinates.y())) +
                                       QString("%1 (%2,%3), ").arg(QString::number(ResultCircleCenterDesc.Points[1].n),
                                                                   QString::number(ResultCircleCenterDesc.Points[1].Coordinates.x()),
                                                                   QString::number(ResultCircleCenterDesc.Points[1].Coordinates.y())) +
                                       QString("%1 (%2,%3).").arg(QString::number(ResultCircleCenterDesc.Points[2].n),
                                                                   QString::number(ResultCircleCenterDesc.Points[2].Coordinates.x()),
                                                                   QString::number(ResultCircleCenterDesc.Points[2].Coordinates.y())) +
                                       " Сторона треугольника, через которую проходит прямая: " +
                                       (ResultCircleCenterDesc.TriangleLine[0] + 'A') +
                                       (ResultCircleCenterDesc.TriangleLine[1] + 'A') +
                                       ". Угол между вышеуказанной прямой и осью Оy — " +
                                       QString::number(MinCircleAngle) + "°.");
        DrawAnswer(Triangle, ResultCircleCenterDesc);
    } else {
        ui->LabelCommunicator->setText(NO_NEEDED_CIRCLES_FOUND);
    }
}



void MainWindow::on_InputFieldPointX_returnPressed()
{
    ui->InputFieldPointY->setFocus();
}

void MainWindow::on_InputFieldPointY_returnPressed()
{
    ui->ButtonAddPoint->click();
}

void MainWindow::on_InputFieldTriangleX0_returnPressed()
{
    ui->InputFieldTriangleY0->setFocus();
}

void MainWindow::on_InputFieldTriangleY0_returnPressed()
{
    ui->InputFieldTriangleX1->setFocus();
}

void MainWindow::on_InputFieldTriangleX1_returnPressed()
{
    ui->InputFieldTriangleY1->setFocus();
}

void MainWindow::on_InputFieldTriangleY1_returnPressed()
{
    ui->InputFieldTriangleX2->setFocus();
}

void MainWindow::on_InputFieldTriangleX2_returnPressed()
{
    ui->InputFieldTriangleY2->setFocus();
}

void MainWindow::on_InputFieldTriangleY2_returnPressed()
{
    ui->ButtonCompute->click();
}

void MainWindow::on_ButtonClearTriangle_clicked()
{
    ClearNeeded();
    ui->TablePoints->setCurrentItem(nullptr);
    ui->InputFieldTriangleX0->clear();
    ui->InputFieldTriangleX1->clear();
    ui->InputFieldTriangleX2->clear();
    ui->InputFieldTriangleY0->clear();
    ui->InputFieldTriangleY1->clear();
    ui->InputFieldTriangleY2->clear();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleX0_textChanged(const QString &arg1)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleY0_textChanged(const QString &arg1)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleX1_textChanged(const QString &arg1)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleY1_textChanged(const QString &arg1)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleX2_textChanged(const QString &arg1)
{
    ClearNeeded();
}


void MainWindow::on_InputFieldTriangleY2_textChanged(const QString &arg1)
{
    ClearNeeded();
}

