#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <string>
#include "tinyxml2.h"

class SettingsManager {
public:
    SettingsManager(const std::string& filename);

    bool loadSettings();  // Lädt die Einstellungen aus der XML-Datei
    bool saveSettings(const std::string& apiKey, float temperature);  // Speichert die Einstellungen in der XML-Datei

    std::string getApiKey() const;
    float getTemperature() const;

private:
    std::string filename;  // Dateiname der XML-Datei
    std::string apiKey;  // Gespeicherter API-Key
    float temperature;  // Gespeicherte Temperatur
};

#endif // SETTINGSMANAGER_H
