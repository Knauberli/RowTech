#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QElapsedTimer>
#include <QString>
#include <QStringList>
#include <QApplication>
#include <QDateTime>
#include "test.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void UpdateMeasurementTrainingsdauer();

private slots:
    void on_comboBox_1_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_pB_AddUser_clicked();

    void on_pB_AddBoat_clicked();

    void on_comboBox_5_currentIndexChanged(int index);

    void on_comboBox_6_currentIndexChanged(int index);

    void on_pB_StartTraining_clicked();

    void on_pB_StopTraining_clicked();

    void on_pB_Off_clicked();

    void on_comboBox_5_currentIndexChanged(const QString &arg1);

    void on_pB_SaveSettings_clicked();

    void spm_update(quint8 spm);
private:
    Ui::MainWindow *ui;
    QTimer *UpdateTrainingsdauer;
    QElapsedTimer Trainingsdauer;
};

#endif // MAINWINDOW_H
