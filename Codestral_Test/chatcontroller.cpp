#include "chatcontroller.h"
#include "apicommunication.h"

ChatController::ChatController(SettingsManager* settingsManager) : settingsManager(settingsManager) {}

std::string ChatController::processChat(const std::string& message) {
    // Überprüfe, ob API-Key und Temperatur gültig sind
    if (!settingsManager->getApiKey().empty() && settingsManager->getTemperature() >= 0.0f && settingsManager->getTemperature() <= 1.0f) {
        return SendChatRequest(message, settingsManager->getApiKey(), settingsManager->getTemperature());
    }
    else {
        return "Ungültige Einstellungen.";
    }
}
