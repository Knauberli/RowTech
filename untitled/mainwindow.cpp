#include "mainwindow.h"
#include "ui_mainwindow.h"


#include <QString>
#include <QStringList>
#include <QApplication>
#include <QDateTime>

#include <QDebug>

// Werte für ComboBoxen
// Müssen in txt Datei ausgelagert werden --> Dann können mehrere Nutzer hinzugefügt werden

QStringList MeasurementType = {"Schlagzahl (spm)","Gesamtdistanz","Distanz pro Schlag","Trainingsdauer","Zeit / 500 Meter", "Split"};
QStringList Users = {"TestUser","Michael","TestUser 2"};
QStringList Boats = {"Tigerente","Sprinter","Fun42"};



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // ComboBoxen mit Werten füllen
    this->ui->comboBox_1->addItems(MeasurementType);
    this->ui->comboBox_2->addItems(MeasurementType);
    this->ui->comboBox_3->addItems(MeasurementType);
    this->ui->comboBox_4->addItems(MeasurementType);
    this->ui->comboBox_5->addItems(Users);
    this->ui->comboBox_6->addItems(Boats);
    // Standardwerte setzen
    // Müssen später aus Textdatei eingelesen werden
    this->ui->comboBox_1->setCurrentIndex(0);
    this->ui->comboBox_2->setCurrentIndex(1);
    this->ui->comboBox_3->setCurrentIndex(2);
    this->ui->comboBox_4->setCurrentIndex(3);
    this->ui->comboBox_5->setCurrentIndex(1);
    this->ui->comboBox_6->setCurrentIndex(1);

    // Timer um Trainingsdauer zu Updaten

    UpdateTrainingsdauer = new QTimer(this);
    connect(UpdateTrainingsdauer, SIGNAL(timeout()),this, SLOT(UpdateMeasurementTrainingsdauer()));

}

MainWindow::~MainWindow()
{
    delete ui;
}






// ======================================
// Einstellungen Start
// ======================================

// ComboBox für Anzeigewert oben links
void MainWindow::on_comboBox_1_currentIndexChanged(int index)
{
    this->ui->lab_DataType_topleft->setText(MeasurementType.at(index));
}
// ComboBox für Anzeigewert oben rechts
void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    this->ui->lab_DataType_topright->setText(MeasurementType.at(index));
}
// ComboBox für Anzeigewert unten links
void MainWindow::on_comboBox_3_currentIndexChanged(int index)
{
    this->ui->lab_DataType_bottomleft->setText(MeasurementType.at(index));
}
// ComboBox für Anzeigewert unten rechts
void MainWindow::on_comboBox_4_currentIndexChanged(int index)
{
    this->ui->lab_DataType_bottomright->setText(MeasurementType.at(index));
}
// User auswählen
void MainWindow::on_comboBox_5_currentIndexChanged(int index)
{
    this->ui->lab_User->setText(Users.at(index));
}
// Select Boat
void MainWindow::on_comboBox_6_currentIndexChanged(int index)
{
    this->ui->lab_Boat->setText(Boats.at(index));
}
// Add User
void MainWindow::on_pB_AddUser_clicked()
{
    ui->pB_AddUser->setText("tbd");
}
// Add Boat
void MainWindow::on_pB_AddBoat_clicked()
{
    ui->pB_AddBoat->setText("tbd");
}

// ======================================
// Start / Stop
// ======================================

// Start Training
void MainWindow::on_pB_StartTraining_clicked()
{
    ui->tab->setCurrentIndex(0);
    Trainingsdauer.start();
    UpdateTrainingsdauer->start(500);
    ui->pB_StartTraining->setEnabled(false);
}

void MainWindow::UpdateMeasurementTrainingsdauer()
{
    ui->lab_Data_bottomright->setText(QDateTime::fromTime_t((Trainingsdauer.elapsed()/1000)).toUTC().toString("hh:mm:ss"));

}


// Stop Training
void MainWindow::on_pB_StopTraining_clicked()
{
    UpdateTrainingsdauer->stop();
    ui->pB_StartTraining->setEnabled(true);
}



void MainWindow::on_pB_Off_clicked()
{
    // if boolsche Variable für Messwertaufzeichnung false
    // sudo shutdown -r


}
