#include <wx/log.h>
#include "settingsmanager.h"
#include <filesystem>
#include <iostream>

SettingsManager::SettingsManager(const std::string& filename)
    : filename(filename), apiKey("default-api-key"), temperature(0.7f) {
    if (!loadSettings()) {
        std::cerr << "Einstellungen konnten nicht geladen werden. Eine neue Datei wird erstellt." << std::endl;
        // Erstelle die XML-Datei mit Standardwerten, wenn sie nicht existiert
        if (!saveSettings(apiKey, temperature)) {
            std::cerr << "Fehler beim Erstellen der Standard-XML-Datei." << std::endl;
        }
    }
}

bool SettingsManager::loadSettings() {
    tinyxml2::XMLDocument doc;

    // Überprüfen, ob die Datei geladen werden kann
    tinyxml2::XMLError eResult = doc.LoadFile(filename.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Fehler beim Laden der XML-Datei: " << filename << ". Fehlercode: " << eResult << std::endl;
        return false;  // Datei konnte nicht geladen werden
    }

    // Root-Element "Settings" überprüfen
    tinyxml2::XMLElement* root = doc.FirstChildElement("Settings");
    if (!root) {
        std::cerr << "Fehler: 'Settings' Element nicht gefunden." << std::endl;
        return false;
    }

    // API-Key laden
    tinyxml2::XMLElement* keyElement = root->FirstChildElement("ApiKey");
    if (keyElement && keyElement->GetText()) {
        apiKey = keyElement->GetText();
    }
    else {
        std::cerr << "API-Key konnte nicht geladen werden. Standardwert wird verwendet." << std::endl;
    }

    // Temperatur laden
    tinyxml2::XMLElement* tempElement = root->FirstChildElement("Temperature");
    if (tempElement) {
        tempElement->QueryFloatText(&temperature);
    }
    else {
        std::cerr << "Temperatur konnte nicht geladen werden. Standardwert wird verwendet." << std::endl;
    }

    return true;
}
bool SettingsManager::saveSettings(const std::string& newApiKey, float newTemperature) {
    std::cout << "Speichere Einstellungen: API-Key = " << newApiKey << ", Temperatur = " << newTemperature << std::endl;

    // Update der internen Werte
    apiKey = newApiKey;
    temperature = newTemperature;

    // Überprüfe, ob das Verzeichnis existiert, und erstelle es, falls nicht
    std::string directoryPath = "./";  // Hier kannst du den gewünschten Pfad angeben
    if (!std::filesystem::exists(directoryPath)) {
        if (!std::filesystem::create_directory(directoryPath)) {
            wxLogError("Fehler beim Erstellen des Verzeichnisses: %s", directoryPath.c_str());
            return false;
        }
    }

    tinyxml2::XMLDocument doc;

    // Root-Element "Settings" erstellen
    tinyxml2::XMLElement* root = doc.NewElement("Settings");
    if (!root) {
        std::cerr << "Fehler: Root-Element konnte nicht erstellt werden." << std::endl;
        return false;
    }
    doc.InsertFirstChild(root);

    // API-Key hinzufügen
    tinyxml2::XMLElement* keyElement = doc.NewElement("ApiKey");
    if (!keyElement) {
        std::cerr << "Fehler: API-Key Element konnte nicht erstellt werden." << std::endl;
        return false;
    }
    keyElement->SetText(apiKey.c_str());
    root->InsertEndChild(keyElement);

    // Temperatur hinzufügen
    tinyxml2::XMLElement* tempElement = doc.NewElement("Temperature");
    if (!tempElement) {
        std::cerr << "Fehler: Temperatur-Element konnte nicht erstellt werden." << std::endl;
        return false;
    }
    tempElement->SetText(temperature);
    root->InsertEndChild(tempElement);

    // Volle Pfadangabe der Datei
    std::string fullPath = directoryPath + "settings.xml";
    std::cout << "Versuche, Datei zu speichern in: " << fullPath << std::endl;

    // XML-Datei speichern
    tinyxml2::XMLError eResult = doc.SaveFile(fullPath.c_str());
    if (eResult != tinyxml2::XML_SUCCESS) {
        std::cerr << "Fehler beim Speichern der XML-Datei: " << fullPath << ". Fehlercode: " << eResult << std::endl;
        wxLogError("Fehler beim Speichern der Einstellungen. Fehlercode: %d", eResult);
        return false;
    }

    std::cout << "Einstellungen erfolgreich gespeichert in: " << fullPath << std::endl;
    return true;
}



std::string SettingsManager::getApiKey() const {
    return apiKey;
}

float SettingsManager::getTemperature() const {
    return temperature;
}
