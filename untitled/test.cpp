#include "test.h"

void SaveUserSettings(QString test, int cb1,int cb2,int cb3,int cb4, int cb5, int cb6);
int SetUserSettings(QString Username, int cB_index);
QString GetInitUser(QString Username);
void UserSettingsInitSave(QString NameInitDatei, QString LastUser);
void ChangeUserSettings(QString Username,int cb1,int cb2,int cb3,int cb4, int cb5, int cb6);


test::test()
{

}

void SaveUserSettings(QString test, int cb1,int cb2,int cb3,int cb4, int cb5, int cb6)
    {
    QString qstrUserFilename;
    qstrUserFilename = test + ".txt";
    QFile UserSettingsFile(qstrUserFilename);

    if(!UserSettingsFile.exists())
    {
        qDebug() << UserSettingsFile.fileName() << " does not exist";
    }
    else
    {
        UserSettingsFile.remove();
    }
    if(UserSettingsFile.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream txtStream(&UserSettingsFile);
        txtStream << cb1 << ", " << cb2 << ", " << cb3 << ", " << cb4 << ", " << cb5 << ", " << cb6;
        UserSettingsFile.close();
    }
    else
    {
        return;
    }
     }

void ChangeUserSettings(QString Username,int cb1,int cb2,int cb3,int cb4, int cb5, int cb6)
{
    QString qstrUserFilename;
    qstrUserFilename = Username + ".txt";

    QFile UserSettingsFile(qstrUserFilename);

    if(!UserSettingsFile.exists())
    {
        qDebug() << UserSettingsFile.fileName() << " does not exist";
    }
    else
    {
        UserSettingsFile.remove();
    }
    if(UserSettingsFile.open(QIODevice::ReadOnly | QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream txtStream(&UserSettingsFile);
        txtStream << cb1 << ", " << cb2 << ", " << cb3 << ", " << cb4 << ", " << cb5 << ", " << cb6;
        UserSettingsFile.close();
    }
    else
    {
        return;
    }
}

int SetUserSettings(QString Username, int cB_index)
{
    QString qstrUserFilename;
    QString UserSettings;
    int value;

    qstrUserFilename = Username + ".txt";

    QFile UserSettingsFile(qstrUserFilename);

    if(UserSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream txtStream(&UserSettingsFile);
        UserSettings = txtStream.readAll();
        QRegExp rx(",");
        QStringList list = UserSettings.split(rx, QString::SkipEmptyParts);
        foreach (QString str, list) {
            str = str.trimmed();
        }
        value = list.at(cB_index).toInt();
        UserSettingsFile.close();
        return value;
    }
    else
    {
        return 0;
    }
}

// Speichere den letzten ausgewählten Usernamen als Init Usernamen
void UserSettingsInitSave(QString NameInitDatei, QString LastUser)
{
    QString qstrUserFilename;
    qstrUserFilename = NameInitDatei + ".txt";
    QFile UserSettingsFile(qstrUserFilename);

    if(!UserSettingsFile.exists())
    {
        QTextStream txtStream(&UserSettingsFile);
        txtStream << LastUser;
        UserSettingsFile.close();
    }
    else
    {
        UserSettingsFile.remove();
    }
    if(UserSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream txtStream(&UserSettingsFile);
        txtStream << LastUser;
        UserSettingsFile.close();
    }
    else
    {
        return;
    }
}


// Lese den InitUsernamen aus InitDatei
QString GetInitUser(QString Username)
{
    QString qstrUserFilename;
    QString UserSettings;
    qstrUserFilename = Username + ".txt";
    QFile UserSettingsFile(qstrUserFilename);
    QString LastUser;

    if(UserSettingsFile.open(QIODevice::ReadWrite | QIODevice::Text))
    {
        QTextStream txtStream(&UserSettingsFile);
        UserSettings = txtStream.readAll();
        QRegExp rx(",");
        QStringList list = UserSettings.split(rx, QString::SkipEmptyParts);
        LastUser = list.at(0);

        UserSettingsFile.close();
        return LastUser;
    }
    else
    {
        return 0;
    }
}
