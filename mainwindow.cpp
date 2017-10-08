#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <cmath>

double round100(double val) {
    return round(val * 100) / 100;
}

double mgToGramms(double val, QString *label) {
    if(val >= 1000) {
        *label = "g";
        return round100(val / 1000);
    }
    *label = "mg";
    return val;
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
    OtherIndex = 0;

    lastKHEdit = 0;

    tankValue = 100;

    ////////////////////////////Molar Mass

    MolarMass["Na"] = 22.98976928;
    MolarMass["K"] = 39.0983;
    MolarMass["Ca"] = 40.078;
    MolarMass["Mg"] = 24.304;
    MolarMass["Cl"] = 35.446;
    MolarMass["N"] = 14.00643;
    MolarMass["C"] = 12.0096;
    MolarMass["O"] = 15.99903;
    MolarMass["S"] = 32.059;
    MolarMass["H"] = 1.00784;

    MolarMass["HCO3"] = MolarMass["H"] + MolarMass["C"] + MolarMass["O"] * 3;
    MolarMass["CO3"] = MolarMass["C"] + MolarMass["O"] * 3;
    MolarMass["SO4"] = MolarMass["S"] + MolarMass["O"] * 4;
    MolarMass["NO3"] = MolarMass["N"] + MolarMass["O"] * 3;
    MolarMass["H2O"] = MolarMass["H"] * 2 + MolarMass["O"];

    MolarMass["K2SO4"] = MolarMass["K"] * 2 + MolarMass["SO4"];
    MolarMass["Na2SO4"] = MolarMass["Na"] * 2 + MolarMass["SO4"];
    MolarMass["NaCl"] = MolarMass["Na"] + MolarMass["Cl"];
    MolarMass["KCl"] = MolarMass["K"] + MolarMass["Cl"];
    MolarMass["KNO3"] = MolarMass["K"] + MolarMass["NO3"];
    MolarMass["NaHCO3"] = MolarMass["Na"] + MolarMass["HCO3"];
    MolarMass["KHCO3"] = MolarMass["K"] + MolarMass["HCO3"];
    MolarMass["Na2CO3"] = MolarMass["Na"] * 2 + MolarMass["CO3"];
    MolarMass["K2CO3"] = MolarMass["K"] * 2 + MolarMass["CO3"];

    MolarMass["Ca(HCO3)2"] = MolarMass["Ca"] + MolarMass["HCO3"] * 2;
    MolarMass["Mg(HCO3)2"] = MolarMass["Mg"] + MolarMass["HCO3"] * 2;
    MolarMass["CaCO3"] = MolarMass["Ca"] + MolarMass["CO3"];
    MolarMass["MgCO3"] = MolarMass["Mg"] + MolarMass["CO3"];
    MolarMass["CaCl2"] = MolarMass["Ca"] + MolarMass["Cl"] * 2;
    MolarMass["MgCl2"] = MolarMass["Mg"] + MolarMass["Cl"] * 2;
    MolarMass["CaSO4"] = MolarMass["Ca"] + MolarMass["SO4"];
    MolarMass["MgSO4"] = MolarMass["Mg"] + MolarMass["SO4"];
    MolarMass["Ca(NO3)2"] = MolarMass["Ca"] + MolarMass["NO3"] * 2;
    MolarMass["Mg(NO3)2"] = MolarMass["Mg"] + MolarMass["NO3"] * 2;

    MolarMass["MgSO4*7H2O"] = MolarMass["MgSO4"] + 7 * MolarMass["H2O"];
    MolarMass["CaSO4*2H2O"] = MolarMass["CaSO4"] + 2 * MolarMass["H2O"];
    MolarMass["Ca(NO3)2*4H2O"] = MolarMass["Ca(NO3)2"] + 4 * MolarMass["H2O"];
    MolarMass["CaCl2*6H2O"] = MolarMass["Ca(NO3)2"] + 6 * MolarMass["H2O"];
    MolarMass["Mg(NO3)2*6H2O"] = MolarMass["Mg(NO3)2"] + 6 * MolarMass["H2O"];


    ////////////////////////////
    QStringList hardList, KHSaltList, CaSaltList, MgSaltList, OtherSaltList;

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

    ////////////////////////// other
    OtherSaltList << "K2SO4"
                  << "Na2SO4"
                  << "KCl"
                  << "NaCl"
                  << "KNO3"
                     ;

    OtherAnionList << "SO4--"
                   << "SO4--"
                   << "Cl-"
                   << "Cl-"
                   << "NO3-"
                      ;

    OtherCationList << "K-"
                    << "Na-"
                    << "K-"
                    << "Na-"
                    << "K"
                       ;

    pointOtherAnion[0] = MolarMass["K2SO4"] / MolarMass["SO4"];//1.189439147;//k2so4
    pointOtherAnion[1] = MolarMass["Na2SO4"] / MolarMass["SO4"];//1.478678685;//na2so4
    pointOtherAnion[2] = MolarMass["KCl"] / MolarMass["Cl"];//1.256680585;//kcl
    pointOtherAnion[3] = MolarMass["NaCl"] / MolarMass["Cl"];//1.648585716;//nacl
    pointOtherAnion[4] = MolarMass["KNO3"] / MolarMass["NO3"];//1.146738443;//kno3

    pointOtherCation[0] = MolarMass["K2SO4"] / MolarMass["K"] / 2;//12.557479969 / 2;//k2so4
    pointOtherCation[1] = MolarMass["Na2SO4"] / MolarMass["Na"] / 2;//6.178168073 / 2;//na2so4
    pointOtherCation[2] = MolarMass["KCl"] / MolarMass["K"];//4.895892639;//kcl
    pointOtherCation[3] = MolarMass["NaCl"] / MolarMass["Na"];//2.541816256;//nacl
    pointOtherCation[4] = MolarMass["KNO3"] / MolarMass["K"];//7.81484673;//kno3

    ////////////////////////// KH
    pointKH[0] = 21.8;
    pointKH[1] = 10.7145;
    pointKH[2] = 30.019672131;//30; ///nahco3
    pointKH[3] = 35.768862671;//25.05; ///khco3
    pointKH[4] = 28.959773254;//28.96; ///ca(hco3)2
    pointKH[5] = 26.141811696;//26.14; ///mg(hco3)2
    pointKH[6] = 17.870630941;//17.86; ///caco3
    pointKH[7] = 15.045147498;//15.05; ///mgco3
    pointKH[8] = 18.913122874;//18.91; ///na2co3
    pointKH[9] = 13.955288859;//13.96; ////k2co3

    pointKHCation[0] = -1;
    pointKHCation[1] = -1;
    pointKHCation[2] = MolarMass["NaHCO3"] / MolarMass["Na"];//3.653986182;//3.65;
    pointKHCation[3] = MolarMass["KHCO3"] / MolarMass["K"];//2.560725147;//7.706146203;//7.71;
    pointKHCation[4] = MolarMass["Ca(HCO3)2"] / MolarMass["Ca"];//4.044789161;//4.05;
    pointKHCation[5] = MolarMass["Mg(HCO3)2"] / MolarMass["Mg"];//6.020945523;//6.02;
    pointKHCation[6] = MolarMass["CaCO3"] / MolarMass["Ca"];//2.497247617;//2.50;
    pointKHCation[7] = MolarMass["MgCO3"] / MolarMass["Mg"];//3.469004691;//3.47;
    pointKHCation[8] = MolarMass["Na2CO3"] / MolarMass["Na"] / 2;//4.610259403 / 2;//4.61 / 2;
    pointKHCation[9] = MolarMass["K2CO3"] / MolarMass["K"] / 2;//8.595373861 / 2;//8.60 / 2;

    pointKHAnion[0] = -1;
    pointKHAnion[1] = -1;
    pointKHAnion[2] = MolarMass["NaHCO3"] / MolarMass["HCO3"];//1.376791713;//1.38;
    pointKHAnion[3] = MolarMass["KHCO3"] / MolarMass["HCO3"];//1.640773517;//1.14911694;//1.15;
    pointKHAnion[4] = MolarMass["Ca(HCO3)2"] / MolarMass["HCO3"] / 2;//2.656859932 / 2;//2.66 / 2;
    pointKHAnion[5] = MolarMass["Mg(HCO3)2"] / MolarMass["HCO3"] / 2;//2.398331348 / 2;//2.40 / 2;
    pointKHAnion[6] = MolarMass["CaCO3"] / MolarMass["CO3"];//1.667892197;//1.67;
    pointKHAnion[7] = MolarMass["MgCO3"] / MolarMass["CO3"];//1.405021507;//1.41;
    pointKHAnion[8] = MolarMass["Na2CO3"] / MolarMass["CO3"];//1.766283059;//1.77;
    pointKHAnion[9] = MolarMass["K2CO3"] / MolarMass["CO3"];//1.303242855;//1.30;

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
    pointCa[0] = MolarMass["CaCO3"] / MolarMass["Ca"];//2.497247617;//2.50;
    pointCa[1] = MolarMass["Ca(HCO3)2"] / MolarMass["Ca"];//4.044789161;//4.05;
    pointCa[2] = MolarMass["CaSO4*2H2O"] / MolarMass["Ca"];//4.295686911;//4.30;
    pointCa[3] = MolarMass["CaCl2"] / MolarMass["Ca"];//2.768850741;//2.77;
    pointCa[4] = MolarMass["CaSO4"] / MolarMass["Ca"];//3.396704426;//3.40;
    pointCa[5] = MolarMass["Ca(NO3)2"] / MolarMass["Ca"];//4.094142422;//4.09;
    pointCa[6] = MolarMass["Ca(NO3)2*4H2O"] / MolarMass["Ca"];//5.892107391;//5.89;
    pointCa[7] = MolarMass["CaCl2*6H2O"] / MolarMass["Ca"];//5.465798194;//5.89;

    pointCaAnion[0] = MolarMass["CaCO3"] / MolarMass["CO3"];//1.667892197;//1.67;
    pointCaAnion[1] = MolarMass["Ca(HCO3)2"] / MolarMass["HCO3"] / 2;//2.656859932 / 2;//2.66 / 2;
    pointCaAnion[2] = MolarMass["CaSO4*2H2O"] / MolarMass["SO4"];//1.792330695;//1.79;
    pointCaAnion[3] = MolarMass["CaCl2"] / MolarMass["Cl"] / 2;//3.130677651 / 2;//3.13 / 2;
    pointCaAnion[4] = MolarMass["CaSO4"] / MolarMass["SO4"];//1.417239602;//1.42;
    pointCaAnion[5] = MolarMass["Ca(NO3)2"] / MolarMass["NO3"] / 2;//2.646382657 / 2;//2.65 / 2;
    pointCaAnion[6] = MolarMass["Ca(NO3)2*4H2O"] / MolarMass["NO3"] / 2;//3.80855603 / 2;//3.81 / 2;
    pointCaAnion[7] = MolarMass["CaCl2*6H2O"] / MolarMass["Cl"] / 2;//6.18005586 / 2;//3.81 / 2;

    CaSaltList << "CaCO3" ///0
               << "Ca(HCO3)2" ///1
               << "CaSO4*2H2O" ///2
               << "CaCl2" ///3
               << "CaSO4" ///4
               << "Ca(NO3)2" ///5
               << "Ca(NO3)2*4H2O" ///6
               << "CaCl2*6H2O"; ///7

    CaAnionList << "CO3--" ///0
                << "HCO3-" ///1
                << "SO4--" ///2
                << "Cl-" ///3
                << "SO4--" ///4
                << "NO3-" ///5
                << "NO3-" ///6
                << "Cl-"; ///7



    pointMg[0] = MolarMass["MgCO3"] / MolarMass["Mg"];//3.469004691;//3.47;
    pointMg[1] = MolarMass["Mg(HCO3)2"] / MolarMass["Mg"];//6.020945523;//6.02;
    pointMg[2] = MolarMass["MgSO4*7H2O"] / MolarMass["Mg"];//10.140803571;//10.14;
    pointMg[3] = MolarMass["MgCl2"] / MolarMass["Mg"];//3.916886109;//3.98;
    pointMg[4] = MolarMass["Mg(NO3)2"] / MolarMass["Mg"];//6.102330481;//6.10;
    pointMg[5] = MolarMass["Mg(NO3)2*6H2O"] / MolarMass["Mg"];//10.549674951;//10.55;

    pointMgAnion[0] = MolarMass["MgCO3"] / MolarMass["CO3"];//1.405021507;//1.41;
    pointMgAnion[1] = MolarMass["Mg(HCO3)2"] / MolarMass["HCO3"] / 2;//2.398331348 / 2;//2.40 / 2;
    pointMgAnion[2] = MolarMass["MgSO4*7H2O"] / MolarMass["SO4"];//2.565840218;//2.57;
    pointMgAnion[3] = MolarMass["MgCl2"] / MolarMass["Cl"] / 2;//2.685662698 / 2;//2.69 / 2;
    pointMgAnion[4] = MolarMass["Mg(NO3)2"] / MolarMass["NO3"] / 2;//2.391977746 / 2;//2.39 / 2;
    pointMgAnion[5] = MolarMass["Mg(NO3)2*6H2O"] / MolarMass["NO3"] / 2;//4.135237806 / 2;//4.14 / 2;

    MgSaltList << "MgCO3" ///0
               << "Mg(HCO3)2" ///1
               << "MgSO4*7H2O" ///2
               << "MgCl2" ///3
               << "Mg(NO3)2" ///4
               << "Mg(NO3)2*6H2O"; ///5

    MgAnionList << "CO3--" ///0
                << "HCO3-" ///1
                << "SO4--" ///2
                << "Cl-" ///3
                << "NO3-" ///4
                << "NO3-"; ///5


    ///////////////////////////////////////

    QStringList mass;
    mass << "mg" << "g";

    ui->comboBox_caMass->addItems(mass);
    ui->comboBox_mgMass->addItems(mass);
    ui->comboBox_KHMass->addItems(mass);
    ui->comboBox_OtherMass->addItems(mass);


    ui->comboBoxOtherSalts->addItems(OtherSaltList);


    ui->comboBoxAlkalinityIN->addItem("мг-экв/л");
    ui->comboBoxAlkalinityOUT->addItem("dKH");

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

    ui->doubleSpinBox_tankValue->setValue(tankValue);
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

    double hardnessCa = currentCa / 20.04;
    double hardnessMg = currentMg / 12.15;
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

    return (hardnessCa + hardnessMg)
            * point[comboBoxMineralResult_val];
}

/*
 * Slot: Converter out
 * */
void MainWindow::on_doubleSpinBoxOUT_editingFinished()
{
    ui->doubleSpinBoxIN->setValue(count(currentOUT,
                                        currentINpoint, currentOUTpoint));
}

/*
 * Slot: Converter in
 * */
void MainWindow::on_doubleSpinBoxIN_editingFinished()
{
    ui->doubleSpinBoxOUT->setValue(count(currentIN,
                                         currentOUTpoint, currentINpoint));
}

/*
 * Slot: GH Ca ion
 * */
void MainWindow::on_doubleSpinBoxCa_editingFinished()
{
    ui->doubleSpinBoxResult->setValue(calcMineral());

    calcSaltByCa();
}

/*
 * Slot: GH Mg ion
 * */
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

    double Ca = round100(z * ratioCa);
    double Mg = round100(z * ratioMg);

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

    double substance = round100(dkh * pointKH[KHIndex]);
    double cation = round100(substance / pointKHCation[KHIndex]);
    double anion = round100(substance / pointKHAnion[KHIndex]);

    ui->doubleSpinBox_3->setValue(substance);

    ui->doubleSpinBox_KHCation->setValue(cation);
    ui->doubleSpinBox_KHAnion->setValue(anion);

    calcKHSaltForTank();

    lastKHEdit = 0;
}

/*
 * Slot: KH salt spinbox
 * */
void MainWindow::on_doubleSpinBox_3_editingFinished()
{
    double hco3 = ui->doubleSpinBox_3->value();

    double kh = round100(hco3 / pointKH[KHIndex]);
    double cation = round100(hco3 / pointKHCation[KHIndex]);
    double anion = round100(hco3 / pointKHAnion[KHIndex]);

    ui->doubleSpinBox_2->setValue(kh);

    ui->doubleSpinBox_KHCation->setValue(cation);
    ui->doubleSpinBox_KHAnion->setValue(anion);

    calcKHSaltForTank();

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

    double salt = round100(Ca * pointCa[CaIndex]);

    ui->doubleSpinBox_CaSalt->setValue(salt);

    ui->doubleSpinBox_CaAnion->setValue(round100(salt / pointCaAnion[CaIndex]));

    calcCaSaltForTank();
}

/*
 * GH
 * */
void MainWindow::calcSaltByMg() {
    double Mg = ui->doubleSpinBoxMg->value();

    double salt = round100(Mg * pointMg[MgIndex]);

    ui->doubleSpinBox_MgSalt->setValue(salt);

    ui->doubleSpinBox_MgAnion->setValue(round100(salt / pointMgAnion[MgIndex]));

    calcMgSaltForTank();
}

/*
 * GH
 * */
void MainWindow::calcCaBySalt() {
    double salt = ui->doubleSpinBox_CaSalt->value();

    ui->doubleSpinBoxCa->setValue(round100(salt / pointCa[CaIndex]));

    ui->doubleSpinBoxResult->setValue(calcMineral());

    ui->doubleSpinBox_CaAnion->setValue(round100(salt / pointCaAnion[CaIndex]));

    calcCaSaltForTank();
}

/*
 * GH
 * */
void MainWindow::calcMgBySalt() {
    double salt = ui->doubleSpinBox_MgSalt->value();

    ui->doubleSpinBoxMg->setValue(round100(salt / pointMg[MgIndex]));

    ui->doubleSpinBoxResult->setValue(calcMineral());

    ui->doubleSpinBox_MgAnion->setValue(round100(salt / pointMgAnion[MgIndex]));

    calcMgSaltForTank();
}

void MainWindow::calcCaSaltForTank() {
    QString caLabel = "mg";
    double ca = mgToGramms(round100(ui->doubleSpinBox_CaSalt->value() * tankValue), &caLabel);

    ui->spinBox_CaSaltTank->setValue(ca);
    ui->comboBox_caMass->setCurrentText(caLabel);
    ui->label_CaTank->setText("per " + QString::number(tankValue) + "L");
}

void MainWindow::calcMgSaltForTank() {
    QString mgLabel  = "mg";
    double mg = mgToGramms(round100(ui->doubleSpinBox_MgSalt->value() * tankValue), &mgLabel);

    ui->spinBox_MgSaltTank->setValue(mg);
    ui->comboBox_mgMass->setCurrentText(mgLabel);
    ui->label_MgTank->setText("per " + QString::number(tankValue) + "L");
}

void MainWindow::calcKHSaltForTank() {
    QString khLabel  = "mg";
    double salt = mgToGramms(round100(ui->doubleSpinBox_3->value() * tankValue), &khLabel);

    ui->spinBox_KHSaltTank->setValue(salt);
    ui->comboBox_KHMass->setCurrentText(khLabel);
    ui->label_KHTank->setText("per " + QString::number(tankValue) + "L");
}

void MainWindow::calcCaSaltFromTank() {
    double caTank = ui->spinBox_CaSaltTank->value();
    if(ui->comboBox_caMass->currentText() == "g") {
        caTank = round100(caTank * 1000);
    }
    caTank = round100(caTank  / tankValue);
    ui->doubleSpinBox_CaSalt->setValue(caTank);
    calcCaBySalt();
}

void MainWindow::calcMgSaltFromTank() {
    double mgTank = ui->spinBox_MgSaltTank->value();
    if(ui->comboBox_mgMass->currentText() == "g") {
        mgTank = round100(mgTank * 1000);
    }
    mgTank = round100(mgTank  / tankValue);
    ui->doubleSpinBox_MgSalt->setValue(mgTank);
    calcMgBySalt();
}

void MainWindow::calcKHSaltFromTank() {
    double saltTank = ui->spinBox_KHSaltTank->value();
    if(ui->comboBox_KHMass->currentText() == "g") {
        saltTank = round100(saltTank * 1000);
    }
    saltTank = round100(saltTank  / tankValue);
    ui->doubleSpinBox_3->setValue(saltTank);
    on_doubleSpinBox_3_editingFinished();
}

void MainWindow::calcOtherBySalt() {
    double salt = ui->doubleSpinBox_OtherSalt->value();

    ui->doubleSpinBox_OtherAnion->setValue(round100(salt / pointOtherAnion[OtherIndex]));

    ui->doubleSpinBox_OtherCation->setValue(round100(salt / pointOtherCation[OtherIndex]));

    calcOtherSaltForTank();
}

void MainWindow::calcOtherSaltForTank() {
    QString label = "mg";
    double salt = mgToGramms(round100(ui->doubleSpinBox_OtherSalt->value() * tankValue), &label);

    ui->doubleSpinBox_OtherSaltTank->setValue(salt);
    ui->comboBox_OtherMass->setCurrentText(label);
    ui->label_OtherTank->setText("per " + QString::number(tankValue) + "L");
}

void MainWindow::calcOtherSaltFromTank() {
    double saltTank = ui->doubleSpinBox_OtherSaltTank->value();
    if(ui->comboBox_OtherMass->currentText() == "g") {
        saltTank = round100(saltTank * 1000);
    }
    saltTank = round100(saltTank  / tankValue);
    ui->doubleSpinBox_OtherSalt->setValue(saltTank);
    calcOtherBySalt();
}

/*
 * Slot: KH Cation spinbox
 * */
void MainWindow::on_doubleSpinBox_KHCation_editingFinished()
{
    double cation = ui->doubleSpinBox_KHCation->value();

    double substance = round100(cation * pointKHCation[KHIndex]);

    ui->doubleSpinBox_3->setValue(substance);

    on_doubleSpinBox_3_editingFinished();
}

/*
 * Slot: KH Anion spinbox
 * */
void MainWindow::on_doubleSpinBox_KHAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_KHAnion->value();

    double substance = round100(anion * pointKHAnion[KHIndex]);

    ui->doubleSpinBox_3->setValue(substance);

    on_doubleSpinBox_3_editingFinished();
}

/*
 * Slot: GH Ca anion
 * */
void MainWindow::on_doubleSpinBox_CaAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_CaAnion->value();

    ui->doubleSpinBox_CaSalt->setValue(round100(anion * pointCaAnion[CaIndex]));

    calcCaBySalt();
}

/*
 * Slot: GH Mg anion
 * */
void MainWindow::on_doubleSpinBox_MgAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_MgAnion->value();

    ui->doubleSpinBox_MgSalt->setValue(round100(anion * pointMgAnion[MgIndex]));

    calcMgBySalt();
}

/*
 * Slot: Alkalinity converter in
 * */
void MainWindow::on_doubleSpinBox_AlkalinityIN_editingFinished()
{
    ui->doubleSpinBox_AlkalinityOUT->setValue(count(ui->doubleSpinBox_AlkalinityIN->value(),
                                         point[1], point[0]));
}

/*
 * Slot: Alkalinity converter out
 * */
void MainWindow::on_doubleSpinBox_AlkalinityOUT_editingFinished()
{
    ui->doubleSpinBox_AlkalinityIN->setValue(count(ui->doubleSpinBox_AlkalinityOUT->value(),
                                         point[0], point[1]));
}

/**
 * @brief MainWindow::on_doubleSpinBox_tankValue_editingFinished
 * Slot: Setting tank value
 */
void MainWindow::on_doubleSpinBox_tankValue_editingFinished()
{
    tankValue = ui->doubleSpinBox_tankValue->value();

    calcCaSaltForTank();
    calcMgSaltForTank();
    calcKHSaltForTank();
    calcOtherSaltForTank();
}

void MainWindow::on_spinBox_CaSaltTank_editingFinished()
{
    calcCaSaltFromTank();
}

void MainWindow::on_spinBox_MgSaltTank_editingFinished()
{
    calcMgSaltFromTank();
}

void MainWindow::on_comboBox_caMass_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(ui->spinBox_CaSaltTank->value() > 0) {
        calcCaSaltFromTank();
    }
}

void MainWindow::on_comboBox_mgMass_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(ui->spinBox_MgSaltTank->value() > 0) {
        calcMgSaltFromTank();
    }
}

void MainWindow::on_comboBox_KHMass_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(ui->spinBox_KHSaltTank->value() > 0) {
        calcKHSaltFromTank();
    }
}

void MainWindow::on_spinBox_KHSaltTank_editingFinished()
{
    calcKHSaltFromTank();
}

void MainWindow::on_doubleSpinBox_OtherSalt_editingFinished()
{
    calcOtherBySalt();
}

void MainWindow::on_comboBoxOtherSalts_currentIndexChanged(int index)
{
    OtherIndex = index;

    ui->label_OtherCation->setText(OtherCationList[OtherIndex]);
    ui->label_OtherAnion->setText(OtherAnionList[OtherIndex]);

    calcOtherBySalt();
}

void MainWindow::on_doubleSpinBox_OtherAnion_editingFinished()
{
    double anion = ui->doubleSpinBox_OtherAnion->value();

    double substance = round100(anion * pointOtherAnion[OtherIndex]);

    ui->doubleSpinBox_OtherSalt->setValue(substance);

    calcOtherBySalt();
}

void MainWindow::on_doubleSpinBox_OtherCation_editingFinished()
{
    double cation = ui->doubleSpinBox_OtherCation->value();

    double substance = round100(cation * pointOtherCation[OtherIndex]);

    ui->doubleSpinBox_OtherSalt->setValue(substance);

    calcOtherBySalt();
}

void MainWindow::on_comboBox_OtherMass_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    if(ui->doubleSpinBox_OtherSaltTank->value() > 0) {
        calcOtherSaltFromTank();
    }
}

void MainWindow::on_doubleSpinBox_OtherSaltTank_editingFinished()
{
    calcOtherSaltFromTank();
}
