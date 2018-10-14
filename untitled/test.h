#ifndef TEST_H
#define TEST_H

#include <Qdir>
#include <QFile>
#include <QDebug>
#include <QString>

using namespace std;

int SetUserSettings(QString Username, int cB_index);
QString GetInitUser(QString Username);
void UserSettingsInitSave(QString NameInitDatei, QString LastUser);
void SaveUserSettings(QString test, int cb1,int cb2,int cb3,int cb4, int cb5, int cb6);
void ChangeUserSettings(QString Username,int cb1,int cb2,int cb3,int cb4, int cb5, int cb6);

class test
{
public:
    test();
};

#endif // TEST_H
