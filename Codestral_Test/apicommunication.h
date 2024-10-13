// apicommunication.h
#ifndef APICOMMUNICATION_H
#define APICOMMUNICATION_H

#include <string>

// Funktion für die API-Kommunikation
std::string SendChatRequest(const std::string& message, const std::string& apiKey, float temperature);

// Callback-Funktion für CURL
size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata);

#endif // APICOMMUNICATION_H
