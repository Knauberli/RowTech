#ifndef BASICIO_H
#define BASICIO_H

#include <string>
class basicio
{
public:
    basicio();
private:
    // Anlegen der txt Dateien für Comboboxen
    void CreateExternalFile();
    // Checken ob txt Dateien vorhanden
    // Measurements, Boats, Users, StandardSettings, UserSettings
    void CheckForExternalFile();
    // Neuen Eintrag in External file hinzufügen
    void AddToExternalFile(string NewEntry, string Path);
    // Gesamten External File einlesen
    void ReadExternalFile(string Path);
    // Einzelne Zeile von External File einlesen
    void ReadFromExternalFile(int line, string Path);
};

#endif // BASICIO_H
