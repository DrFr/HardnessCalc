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

private:
    Ui::MainWindow *ui;

    double currentIN, currentOUT,
           currentINpoint, currentOUTpoint;

    QMap<int, double> point, pointKH, pointCa,
    pointMg, pointKHCation, pointKHAnion,
    pointCaAnion, pointMgAnion;

    QStringList KHCationList, KHAnionList, CaAnionList, MgAnionList;

    double currentCa, currentMg,
           currentCaRatio, currentMgRatio;

    int comboBoxMineralResult_val, KHIndex, CaIndex, MgIndex;

    int lastKHEdit; // 0 for doubleSpinBox_2 or 1 for doubleSpinBox_3

    double count(double data, double p1, double p2);

    double calcMineral();

    void calcSaltByCa();
    void calcSaltByMg();

    void calcCaBySalt();
    void calcMgBySalt();
};

#endif // MAINWINDOW_H
