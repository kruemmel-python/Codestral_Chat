// apicommunication.cpp
#include "apicommunication.h"
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <iostream>

std::string SendChatRequest(const std::string& message, const std::string& apiKey, float temperature) {
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "curl_easy_init() failed." << std::endl;
        return "";
    }

    std::unique_ptr<CURL, decltype(&curl_easy_cleanup)> curlHandle(curl, curl_easy_cleanup);
    CURLcode res;
    std::string readBuffer;

    // Setze die URL der API
    res = curl_easy_setopt(curl, CURLOPT_URL, "https://codestral.mistral.ai/v1/chat/completions");
    if (res != CURLE_OK) {
        std::cerr << "Failed to set URL: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    // Erstelle das JSON-Objekt mit nlohmann/json
// Erstelle das JSON-Objekt mit nlohmann/json
    nlohmann::json jsonData;
    jsonData["model"] = "codestral-latest";
    jsonData["temperature"] = temperature;  // Temperature-Wert hinzufügen
    jsonData["messages"] = {
        {
            {"role", "system"},
            {"content",
                "Du bist ein äußerst kenntnisreicher C++-Assistent, der sich darauf konzentriert, sauberen, effizienten und gut dokumentierten Code zu erstellen. "
                "Befolge immer die besten Praktiken und berücksichtige moderne C++-Standards, einschließlich C++20-Funktionen, wenn relevant."
            }
        },
        {
            {"role", "system"},
            {"content",
                "Dein Hauptziel ist es, C++-Code zu schreiben, der lesbar, wartbar und für die Leistung optimiert ist. Gib immer "
                "ausführliche Kommentare ab, die die Gründe für wichtige Entscheidungen erklären, und stelle sicher, dass der Code so strukturiert ist, dass er "
                "gängigen Designmustern entspricht, wo dies angebracht ist."
            }
        },
        {
            {"role", "system"},
            {"content",
                "Achte beim Generieren von Code darauf, dass er den SOLID-Prinzipien folgt, häufige Fallstricke wie Speicherlecks oder Race Conditions vermeidet und eine ordnungsgemäße Fehlerbehandlung umfasst. Konzentriere dich darauf, plattformunabhängigen Code zu schreiben, es sei denn, es wird ausdrücklich anders gewünscht."
            }
        },
        {
            {"role", "system"},
            {"content",
                "Wenn die Aufgabe die Schnittstelle zu externen Bibliotheken umfasst, verwende moderne Werkzeuge wie vcpkg für das Abhängigkeitsmanagement und priorisiere immer die Codesicherheit, indem du unsichere Funktionen und Praktiken vermeidest."
            }
        },
        {
            {"role", "system"},
            {"content",
                "Stelle sicher, dass der ausgegebene C++-Code gut organisiert ist, beginnend mit den notwendigen Headern und der Verwendung von Namespaces. Strebe immer nach Klarheit statt Komplexität und vermeide unnötige Abstraktionen oder Überengineering."
            }
        },
        {
            {"role", "system"},
            {"content",
                "Wenn die Aufgabe mit objektorientiertem Design zu tun hat, berücksichtige immer die Prinzipien der Kapselung, Vererbung und Polymorphie. Verwende moderne C++-Funktionen wie Smart Pointer (z.B. std::shared_ptr, std::unique_ptr) und constexpr zur Optimierung."
            }
        },
        {
            {"role", "user"},
            {"content", message}
        }
    };


    std::string jsonString = jsonData.dump();
    res = curl_easy_setopt(curl, CURLOPT_POSTFIELDS, jsonString.c_str());
    if (res != CURLE_OK) {
        std::cerr << "Failed to set POST data: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    // HTTP-Header setzen
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    std::string authHeader = "Authorization: Bearer " + apiKey;
    headers = curl_slist_append(headers, authHeader.c_str());

    res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    res = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    res = curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    // Führe die Anfrage aus
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Request failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }

    curl_slist_free_all(headers);

    long http_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code != 200) {
        std::cerr << "Received HTTP error code: " << http_code << std::endl;
        return "";
    }

    // Antwort formatieren
    try {
        nlohmann::json responseJson = nlohmann::json::parse(readBuffer);
        if (responseJson.contains("choices") && responseJson["choices"].is_array() && !responseJson["choices"].empty()) {
            if (responseJson["choices"][0].contains("message") && !responseJson["choices"][0]["message"].is_null()) {
                return responseJson["choices"][0]["message"]["content"].get<std::string>();
            }
        }
        std::cerr << "Invalid response format." << std::endl;
        return "Error: Invalid response format.";
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to parse JSON response: " << e.what() << std::endl;
        return "Error: Failed to parse response.";
    }
}

size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
    std::string* data = static_cast<std::string*>(userdata);
    data->append(ptr, size * nmemb);
    return size * nmemb;
}
