#ifndef CHATCONTROLLER_H
#define CHATCONTROLLER_H

#include <string>
#include "settingsmanager.h"  // Korrekt eingebundene Datei

class ChatController {
public:
    ChatController(SettingsManager* settingsManager);

    std::string processChat(const std::string& message);

private:
    SettingsManager* settingsManager;  // Verweis auf den SettingsManager
};

#endif // CHATCONTROLLER_H
