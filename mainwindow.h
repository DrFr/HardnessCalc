#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_doubleSpinBoxIN_valueChanged(double arg1);

    void on_doubleSpinBoxOUT_valueChanged(double arg1);

    void on_comboBoxIN_currentIndexChanged(int index);

    void on_comboBoxOUT_currentIndexChanged(int index);

    void on_doubleSpinBoxOUT_editingFinished();

    void on_doubleSpinBoxIN_editingFinished();

    void on_doubleSpinBoxCa_editingFinished();

    void on_doubleSpinBoxMg_editingFinished();

    void on_doubleSpinBoxCaRation_editingFinished();

    void on_doubleSpinBoxMgRation_editingFinished();

    void on_doubleSpinBoxResult_editingFinished();

    void on_comboBoxMineralResult_currentIndexChanged(int index);

    void on_doubleSpinBox_2_editingFinished();

    void on_doubleSpinBox_3_editingFinished();

    void on_comboBoxKHSalt_currentIndexChanged(int index);

    void on_comboBoxCaSalts_currentIndexChanged(int index);

    void on_comboBoxMgSalts_currentIndexChanged(int index);

    void on_doubleSpinBox_CaSalt_editingFinished();

    void on_doubleSpinBox_MgSalt_editingFinished();

    void on_doubleSpinBox_KHCation_editingFinished();

    void on_doubleSpinBox_KHAnion_editingFinished();

    void on_doubleSpinBox_CaAnion_editingFinished();

    void on_doubleSpinBox_MgAnion_editingFinished();

    void on_doubleSpinBox_AlkalinityIN_editingFinished();

    void on_doubleSpinBox_AlkalinityOUT_editingFinished();

    void on_doubleSpinBox_tankValue_editingFinished();

    void on_spinBox_CaSaltTank_editingFinished();

    void on_spinBox_MgSaltTank_editingFinished();

    void on_comboBox_caMass_currentIndexChanged(const QString &arg1);

    void on_comboBox_mgMass_currentIndexChanged(const QString &arg1);

    void on_comboBox_KHMass_currentIndexChanged(const QString &arg1);

    void on_spinBox_KHSaltTank_editingFinished();

    void on_doubleSpinBox_OtherSalt_editingFinished();

    void on_comboBoxOtherSalts_currentIndexChanged(int index);

    void on_doubleSpinBox_OtherAnion_editingFinished();

    void on_doubleSpinBox_OtherCation_editingFinished();

    void on_comboBox_OtherMass_currentIndexChanged(const QString &arg1);

    void on_doubleSpinBox_OtherSaltTank_editingFinished();

private:
    Ui::MainWindow *ui;

    double currentIN, currentOUT,
           currentINpoint, currentOUTpoint;

    QMap<int, double> point, pointKH, pointCa,
    pointMg, pointKHCation, pointKHAnion,
    pointCaAnion, pointMgAnion, pointOtherAnion, pointOtherCation;

    QMap<QString, double> MolarMass;

    QStringList KHCationList, KHAnionList, CaAnionList, MgAnionList,
    OtherCationList, OtherAnionList;

    double currentCa, currentMg,
           currentCaRatio, currentMgRatio;

    double tankValue;

    int comboBoxMineralResult_val, KHIndex, CaIndex, MgIndex, OtherIndex;

    int lastKHEdit; // 0 for doubleSpinBox_2 or 1 for doubleSpinBox_3

    double count(double data, double p1, double p2);

    double calcMineral();

    void calcSaltByCa();
    void calcSaltByMg();

    void calcCaBySalt();
    void calcMgBySalt();

    void calcCaSaltForTank();
    void calcMgSaltForTank();
    void calcKHSaltForTank();

    void calcCaSaltFromTank();
    void calcMgSaltFromTank();
    void calcKHSaltFromTank();

    void calcOtherBySalt();
    void calcOtherSaltForTank();
    void calcOtherSaltFromTank();
};

#endif // MAINWINDOW_H
