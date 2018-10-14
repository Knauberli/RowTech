#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "test.h"
#include "strokedetector.h"

// Werte für ComboBoxen
// Müssen in txt Datei ausgelagert werden --> Dann können mehrere Nutzer hinzugefügt werden

QStringList MeasurementType = {"Schlagzahl (spm)","Gesamtdistanz","Distanz pro Schlag","Trainingsdauer","Zeit / 500 Meter", "Split"};
QStringList Users = {"TestUser","Michael","TestUser2"};
QStringList Boats = {"Tigerente","Sprinter","Fun42"};
QString LastUser;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Default User in variable speichern
    QString DefaultUser;
    DefaultUser = GetInitUser("InitSettings");
    // ComboBoxen mit Werten füllen
    this->ui->comboBox_1->addItems(MeasurementType);
    this->ui->comboBox_2->addItems(MeasurementType);
    this->ui->comboBox_3->addItems(MeasurementType);
    this->ui->comboBox_4->addItems(MeasurementType);
    this->ui->comboBox_5->addItems(Users);
    this->ui->comboBox_6->addItems(Boats);
    // Lade UserSettings
    this->ui->comboBox_1->setCurrentIndex(SetUserSettings(DefaultUser,0));
    this->ui->comboBox_2->setCurrentIndex(SetUserSettings(DefaultUser,1));
    this->ui->comboBox_3->setCurrentIndex(SetUserSettings(DefaultUser,2));
    this->ui->comboBox_4->setCurrentIndex(SetUserSettings(DefaultUser,3));
    this->ui->comboBox_5->setCurrentIndex(SetUserSettings(DefaultUser,4));
    this->ui->comboBox_6->setCurrentIndex(SetUserSettings(DefaultUser,5));


    // Timer um Trainingsdauer zu Updaten

    UpdateTrainingsdauer = new QTimer(this);
    connect(UpdateTrainingsdauer, SIGNAL(timeout()),this, SLOT(UpdateMeasurementTrainingsdauer()));


    qDebug() << GetInitUser("InitSettings");
    StrokeDetector *str = new StrokeDetector(this);// Qt will delete Strokedetector on shutdown hopefully
    connect(str,SIGNAL(StrokeUpdate(quint8)),this,SLOT(spm_update(quint8)));




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


// Wenn sich Userindex ändert -> Laden der Einstellungsdateien des aktuellen Users
void MainWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    QString SelectedUser;
    SelectedUser = this->ui->comboBox_5->currentText();
    // Lade Einstellungen aus UserDatei
    this->ui->comboBox_1->setCurrentIndex(SetUserSettings(SelectedUser,0));
    this->ui->comboBox_2->setCurrentIndex(SetUserSettings(SelectedUser,1));
    this->ui->comboBox_3->setCurrentIndex(SetUserSettings(SelectedUser,2));
    this->ui->comboBox_4->setCurrentIndex(SetUserSettings(SelectedUser,3));
    this->ui->comboBox_5->setCurrentIndex(SetUserSettings(SelectedUser,4));
    this->ui->comboBox_6->setCurrentIndex(SetUserSettings(SelectedUser,5));
    // Speichere den aktuellen Usernamen in init Datei
    UserSettingsInitSave("InitSettings", this->ui->comboBox_5->currentText());
}

void MainWindow::on_pB_SaveSettings_clicked()
{
    ChangeUserSettings(this->ui->lab_User->text(), this->ui->comboBox_1->currentIndex(), this->ui->comboBox_2->currentIndex(),this->ui->comboBox_3->currentIndex(),this->ui->comboBox_4->currentIndex(),this->ui->comboBox_5->currentIndex(),this->ui->comboBox_6->currentIndex());
}
void MainWindow::spm_update(quint8 spm){
    qDebug() << "Update Spm";
    this->ui->lab_Data_topleft->setText(QString::number(spm));
}
