#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

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

    ////////////////////////// KH
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

    pointKHCation[0] = -1;
    pointKHCation[1] = -1;
    pointKHCation[2] = 3.65;
    pointKHCation[3] = 7.71;
    pointKHCation[4] = 4.05;
    pointKHCation[5] = 6.02;
    pointKHCation[6] = 2.50;
    pointKHCation[7] = 3.47;
    pointKHCation[8] = 4.61 / 2;
    pointKHCation[9] = 8.60 / 2;

    pointKHAnion[0] = -1;
    pointKHAnion[1] = -1;
    pointKHAnion[2] = 1.38;
    pointKHAnion[3] = 1.15;
    pointKHAnion[4] = 2.66 / 2;
    pointKHAnion[5] = 2.40 / 2;
    pointKHAnion[6] = 1.67;
    pointKHAnion[7] = 1.41;
    pointKHAnion[8] = 1.77;
    pointKHAnion[9] = 1.30;

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

    KHCationList << "false" ///0
                 << "false" ///1
                 << "Na+" ///2
                 << "K+" ///3
                 << "Ca++" ///4
                 << "Mg++" ///5
                 << "Ca++" ///6
                 << "Mg++" ///7
                 << "Na+" ///8
                 << "K+"; ///9

    KHAnionList << "false" ///0
                << "false" ///1
                << "HCO3-" ///2
                << "HCO3-" ///3
                << "HCO3-" ///4
                << "HCO3-" ///5
                << "CO3--" ///6
                << "CO3--" ///7
                << "CO3--" ///8
                << "CO3--"; ///9

    ////////////////////////////
    pointCa[0] = 2.50;
    pointCa[1] = 4.05;
    pointCa[2] = 4.30;
    pointCa[3] = 2.77;

    pointCaAnion[0] = 1.67;
    pointCaAnion[1] = 2.66 / 2;
    pointCaAnion[2] = 1.79;
    pointCaAnion[3] = 3.13 / 2;

    CaSaltList << "CaCO3" ///0
               << "Ca(HCO3)2" ///1
               << "CaSO4*2H2O" ///2
               << "CaCl2"; ///3

    CaAnionList << "CO3--" ///0
                << "HCO3-" ///1
                << "SO4--" ///2
                << "Cl-"; ///3



    pointMg[0] = 3.47;
    pointMg[1] = 6.02;
    pointMg[2] = 10.14;
    pointMg[3] = 3.98;

    pointMgAnion[0] = 1.41;
    pointMgAnion[1] = 2.40 / 2;
    pointMgAnion[2] = 2.57;
    pointMgAnion[3] = 2.69 / 2;

    MgSaltList << "MgCO3" ///0
               << "Mg(HCO3)2" ///1
               << "MgSO4*7H2O" ///2
               << "MgCl2"; ///3

    MgAnionList << "CO3--" ///0
                << "HCO3-" ///1
                << "SO4--" ///2
                << "Cl-"; ///3


    ///////////////////////////////////////


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

/*
 * Slot: GH Ca ratio spin
 * */
void MainWindow::on_doubleSpinBoxCaRation_editingFinished()
{
    on_doubleSpinBoxResult_editingFinished();
}

/*
 * Slot: GH Mg ratio spin
 * */
void MainWindow::on_doubleSpinBoxMgRation_editingFinished()
{
    on_doubleSpinBoxResult_editingFinished();
}

/*
 * Slot: GH result
 * */
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

/*
 * Slot: GH result select
 * */
void MainWindow::on_comboBoxMineralResult_currentIndexChanged(int index)
{
    comboBoxMineralResult_val = index;

    ui->doubleSpinBoxResult->setValue(calcMineral());
}

/*
 * Slot: KH spinbox
 * */
void MainWindow::on_doubleSpinBox_2_editingFinished()
{
    double dkh = ui->doubleSpinBox_2->value();
    ui->doubleSpinBox_3->setValue(dkh * pointKH[KHIndex]);

    ui->doubleSpinBox_KHCation->setValue(dkh * pointKH[KHIndex] / pointKHCation[KHIndex]);
    ui->doubleSpinBox_KHAnion->setValue(dkh * pointKH[KHIndex] / pointKHAnion[KHIndex]);

    lastKHEdit = 0;
}

/*
 * Slot: KH salt spinbox
 * */
void MainWindow::on_doubleSpinBox_3_editingFinished()
{
    double hco3 = ui->doubleSpinBox_3->value();
    ui->doubleSpinBox_2->setValue(hco3 / pointKH[KHIndex]);

    ui->doubleSpinBox_KHCation->setValue(hco3 / pointKHCation[KHIndex]);
    ui->doubleSpinBox_KHAnion->setValue(hco3 / pointKHAnion[KHIndex]);

    lastKHEdit = 1;
}

/*
 * Slot: KH salt select
 * */
void MainWindow::on_comboBoxKHSalt_currentIndexChanged(int index)
{
    KHIndex = index;

    ui->labelKHAnion->setEnabled(true);
    ui->labelKHCation->setEnabled(true);

    ui->doubleSpinBox_KHCation->setEnabled(true);
    ui->doubleSpinBox_KHAnion->setEnabled(true);

    ui->labelKHCation->setText(KHCationList[KHIndex]);
    ui->labelKHAnion->setText(KHAnionList[KHIndex]);

    if(KHIndex == 0 || KHIndex == 1) {
        ui->labelKHAnion->setEnabled(false);
        ui->labelKHCation->setEnabled(false);

        ui->doubleSpinBox_KHCation->setEnabled(false);
        ui->doubleSpinBox_KHAnion->setEnabled(false);

        ui->labelKHCation->setText("Cation");
        ui->labelKHAnion->setText("Anion");
    }

    if(lastKHEdit == 0) {
        on_doubleSpinBox_2_editingFinished();
    } else {
        on_doubleSpinBox_3_editingFinished();
    }
}

/*
 * Slot: GH Ca salts select
 * */
void MainWindow::on_comboBoxCaSalts_currentIndexChanged(int index)
{
    CaIndex = index;

    ui->label_CaAnion->setText(CaAnionList[CaIndex]);

    calcSaltByCa();
}

/*
 * Slot: GH Mg salts select
 * */
void MainWindow::on_comboBoxMgSalts_currentIndexChanged(int index)
{
    MgIndex = index;

    ui->label_MgAnion->setText(MgAnionList[MgIndex]);

    calcSaltByMg();
}

/*
 * Slot: GH Ca salt spinbox
 * */
void MainWindow::on_doubleSpinBox_CaSalt_editingFinished()
{
    calcCaBySalt();
}

/*
 * Slot: GH Mg salt spinbox
 * */
void MainWindow::on_doubleSpinBox_MgSalt_editingFinished()
{
    calcMgBySalt();
}

/*
 * GH
 * */
void MainWindow::calcSaltByCa() {
    double Ca = ui->doubleSpinBoxCa->value();

    ui->doubleSpinBox_CaSalt->setValue(Ca * pointCa[CaIndex]);

    ui->doubleSpinBox_CaAnion->setValue(Ca * pointCa[CaIndex] / pointCaAnion[CaIndex]);
}

/*
 * GH
 * */
void MainWindow::calcSaltByMg() {
    double Mg = ui->doubleSpinBoxMg->value();

    ui->doubleSpinBox_MgSalt->setValue(Mg * pointMg[MgIndex]);

    ui->doubleSpinBox_MgAnion->setValue(Mg * pointMg[MgIndex] / pointMgAnion[MgIndex]);
}

/*
 * GH
 * */
void MainWindow::calcCaBySalt() {
    double salt = ui->doubleSpinBox_CaSalt->value();

    ui->doubleSpinBoxCa->setValue(salt / pointCa[CaIndex]);

    ui->doubleSpinBoxResult->setValue(calcMineral());

    ui->doubleSpinBox_CaAnion->setValue(salt / pointCaAnion[CaIndex]);
}

/*
 * GH
 * */
void MainWindow::calcMgBySalt() {
    double salt = ui->doubleSpinBox_MgSalt->value();

    ui->doubleSpinBoxMg->setValue(salt / pointMg[MgIndex]);

    ui->doubleSpinBoxResult->setValue(calcMineral());

    ui->doubleSpinBox_MgAnion->setValue(salt / pointMgAnion[MgIndex]);
}

/*
 * Slot: KH Cation spinbox
 * */
void MainWindow::on_doubleSpinBox_KHCation_editingFinished()
{
    double cation = ui->doubleSpinBox_KHCation->value();

    ui->doubleSpinBox_3->setValue(cation * pointKHCation[KHIndex]);

    on_doubleSpinBox_3_editingFinished();
}

/*
 * Slot: KH Anion spinbox
 * */
void MainWindow::on_doubleSpinBox_KHAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_KHAnion->value();

    ui->doubleSpinBox_3->setValue(anion * pointKHAnion[KHIndex]);

    on_doubleSpinBox_3_editingFinished();
}

void MainWindow::on_doubleSpinBox_CaAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_CaAnion->value();

    ui->doubleSpinBox_CaSalt->setValue(anion * pointCaAnion[CaIndex]);

    calcCaBySalt();
}

void MainWindow::on_doubleSpinBox_MgAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_MgAnion->value();

    ui->doubleSpinBox_MgSalt->setValue(anion * pointMgAnion[MgIndex]);

    calcMgBySalt();
}
