#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

QPair<QString, double> makePair(QString s, double d) {
    return QPair<QString, double>(s, d);
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentIN(0), currentOUT(0),
    currentCa(0), currentMg(0),
    currentCaRatio(3), currentMgRatio(1)
{
    ui->setupUi(this);

    setWindowTitle("Hardness Calculator");

    setWindowIcon(QIcon(":new/prefix1/icon.png"));

    comboBoxMineralResult_val = 0;
    CaIndex = 0;
    MgIndex = 0;
    KHIndex = 0;

    lastKHEdit = 0;


    QStringList hardList, KHSaltList, CaSaltList, MgSaltList;

    point[0] = 1;
    point[1] = 2.8;
    point[2] = 3.51;
    point[3] = 5;
    point[4] = 50.04;

    hardList << "Ж" ///0
             << "dGH" ///1
             << "Clark" ///2
             << "F" ///3
             << "ppm"; ///4

    pointKH[0] = 21.8;
    pointKH[1] = 10.72;
    pointKH[2] = 30;
    pointKH[3] = 25.05;
    pointKH[4] = 28.96;
    pointKH[5] = 26.14;
    pointKH[6] = 17.86;
    pointKH[7] = 15.05;
    pointKH[8] = 18.91;
    pointKH[9] = 13.96;

    KHSaltList << "HCO3-" ///0
               << "CO3--" ///1
               << "NaHCO3" ///2
               << "KHCO3" ///3
               << "Ca(HCO3)2" ///4
               << "Mg(HCO3)2" ///5
               << "CaCO3" ///6
               << "MgC03" ///7
               << "Na2CO3" ///8
               << "K2CO3"; ///9

    pointCa[0] = 2.50;
    pointCa[1] = 4.05;
    pointCa[2] = 4.30;
    pointCa[3] = 2.77;

    CaSaltList << "CaCO3" ///0
               << "Ca(HCO3)2" ///1
               << "CaSO4*2H2O" ///2
               << "CaCl2"; ///3

    pointMg[0] = 3.47;
    pointMg[1] = 6.02;
    pointMg[2] = 10.14;
    pointMg[3] = 3.98;

    MgSaltList << "MgCO3" ///0
               << "Mg(HCO3)2" ///1
               << "MgSO4*7H2O" ///2
               << "MgCl2"; ///3



    ui->comboBoxCaSalts->addItems(CaSaltList);

    ui->comboBoxMgSalts->addItems(MgSaltList);

    ui->comboBoxIN->addItems(hardList);
    ui->comboBoxOUT->addItems(hardList);
    ui->comboBoxOUT->setCurrentIndex(ui->comboBoxOUT->findText("dGH"));
    ui->comboBoxMineralResult->addItems(hardList);
    ui->comboBoxMineralResult->
            setCurrentIndex(ui->comboBoxMineralResult->findText("dGH"));

    ui->comboBoxKHSalt->addItems(KHSaltList);
    ui->comboBoxKHSalt->setCurrentIndex(ui->comboBoxKHSalt->findText("HCO3-"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_doubleSpinBoxIN_valueChanged(double arg1)
{
    currentIN = arg1;
}

void MainWindow::on_doubleSpinBoxOUT_valueChanged(double arg1)
{
    currentOUT = arg1;
}

void MainWindow::on_comboBoxIN_currentIndexChanged(int index)
{
    currentINpoint = point[index];

    ui->doubleSpinBoxOUT->setValue(count(currentIN,
                                         currentOUTpoint, currentINpoint));
}

void MainWindow::on_comboBoxOUT_currentIndexChanged(int index)
{
    currentOUTpoint = point[index];

    ui->doubleSpinBoxOUT->setValue(count(currentIN,
                                         currentOUTpoint, currentINpoint));
}

double MainWindow::count(double data, double p1, double p2) {
    return data * (p1 / p2);
}
double MainWindow::calcMineral() {
    currentCa = ui->doubleSpinBoxCa->value();
    currentMg = ui->doubleSpinBoxMg->value();

    currentCaRatio = ui->doubleSpinBoxCaRation->value();
    currentMgRatio = ui->doubleSpinBoxMgRation->value();

    double hardlessCa = currentCa / 20.04;
    double hardlessMg = currentMg / 12.15;
    double ratio = currentCa / currentMg;
    if(currentCa >= currentMg) {
        if(currentMg != 0) {
            ui->doubleSpinBoxCaRation->setValue(ratio);
            ui->doubleSpinBoxMgRation->setValue(1);
        } else {
            ui->doubleSpinBoxCaRation->setValue(1);
            ui->doubleSpinBoxMgRation->setValue(0);
        }
    } else {
        if(currentCa != 0) {
            ratio = currentMg / currentCa;
            ui->doubleSpinBoxCaRation->setValue(1);
            ui->doubleSpinBoxMgRation->setValue(ratio);
        } else {
            ui->doubleSpinBoxCaRation->setValue(0);
            ui->doubleSpinBoxMgRation->setValue(1);
        }
    }

    return (hardlessCa + hardlessMg)
            * point[comboBoxMineralResult_val];
}

void MainWindow::on_doubleSpinBoxOUT_editingFinished()
{
    ui->doubleSpinBoxIN->setValue(count(currentOUT,
                                        currentINpoint, currentOUTpoint));
}

void MainWindow::on_doubleSpinBoxIN_editingFinished()
{
    ui->doubleSpinBoxOUT->setValue(count(currentIN,
                                         currentOUTpoint, currentINpoint));
}

void MainWindow::on_doubleSpinBoxCa_editingFinished()
{
    ui->doubleSpinBoxResult->setValue(calcMineral());

    calcSaltByCa();
}

void MainWindow::on_doubleSpinBoxMg_editingFinished()
{
    ui->doubleSpinBoxResult->setValue(calcMineral());

    calcSaltByMg();
}

void MainWindow::on_doubleSpinBoxCaRation_editingFinished()
{
    on_doubleSpinBoxResult_editingFinished();
}

void MainWindow::on_doubleSpinBoxMgRation_editingFinished()
{
    on_doubleSpinBoxResult_editingFinished();
}

void MainWindow::on_doubleSpinBoxResult_editingFinished()
{
    double hardless = ui->doubleSpinBoxResult->value()
            / point[comboBoxMineralResult_val];
    double ratioCa = ui->doubleSpinBoxCaRation->value();
    double ratioMg = ui->doubleSpinBoxMgRation->value();

    double x = round((ratioCa / 20.04) * 1000) / 1000;
    double y = round((ratioMg / 12.15) * 1000) / 1000;
    double z = round((hardless / (x + y)) * 1000) / 1000;

    double Ca = round((z * ratioCa) * 100) / 100;
    double Mg = round((z * ratioMg) * 100) / 100;

    ui->doubleSpinBoxCa->setValue(Ca);
    ui->doubleSpinBoxMg->setValue(Mg);

    calcSaltByCa();
    calcSaltByMg();
}

void MainWindow::on_comboBoxMineralResult_currentIndexChanged(int index)
{
    comboBoxMineralResult_val = index;

    ui->doubleSpinBoxResult->setValue(calcMineral());
}

void MainWindow::on_doubleSpinBox_2_editingFinished()
{
    double dkh = ui->doubleSpinBox_2->value();
    ui->doubleSpinBox_3->setValue(dkh * pointKH[KHIndex]);

    lastKHEdit = 0;
}

void MainWindow::on_doubleSpinBox_3_editingFinished()
{
    double hco3 = ui->doubleSpinBox_3->value();
    ui->doubleSpinBox_2->setValue(hco3 / pointKH[KHIndex]);

    lastKHEdit = 1;
}

void MainWindow::on_comboBoxKHSalt_currentIndexChanged(int index)
{
    KHIndex = index;

    if(lastKHEdit == 0) {
        on_doubleSpinBox_2_editingFinished();
    } else {
        on_doubleSpinBox_3_editingFinished();
    }
}

void MainWindow::on_comboBoxCaSalts_currentIndexChanged(int index)
{
    CaIndex = index;

    calcSaltByCa();
}

void MainWindow::on_comboBoxMgSalts_currentIndexChanged(int index)
{
    MgIndex = index;

    calcSaltByMg();
}

void MainWindow::on_doubleSpinBox_CaSalt_editingFinished()
{
    calcCaBySalt();
}

void MainWindow::on_doubleSpinBox_MgSalt_editingFinished()
{
    calcMgBySalt();
}

void MainWindow::calcSaltByCa() {
    double Ca = ui->doubleSpinBoxCa->value();

    ui->doubleSpinBox_CaSalt->setValue(Ca * pointCa[CaIndex]);
}

void MainWindow::calcSaltByMg() {
    double Mg = ui->doubleSpinBoxMg->value();

    ui->doubleSpinBox_MgSalt->setValue(Mg * pointMg[MgIndex]);
}

void MainWindow::calcCaBySalt() {
    double salt = ui->doubleSpinBox_CaSalt->value();

    ui->doubleSpinBoxCa->setValue(salt / pointCa[CaIndex]);

    ui->doubleSpinBoxResult->setValue(calcMineral());
}

void MainWindow::calcMgBySalt() {
    double salt = ui->doubleSpinBox_MgSalt->value();

    ui->doubleSpinBoxMg->setValue(salt / pointMg[MgIndex]);

    ui->doubleSpinBoxResult->setValue(calcMineral());
}
