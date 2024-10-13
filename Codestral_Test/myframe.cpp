#include "myframe.h"
#include "settingsmanager.h"
#include <wx/log.h>
#include <iostream>

enum {
    ID_Quit = 1,
    ID_Chat,
    ID_Settings,  // Menüpunkt für Einstellungen
    ID_Info
};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(ID_Quit, MyFrame::OnQuit)
EVT_BUTTON(ID_Chat, MyFrame::OnChat)
EVT_MENU(ID_Settings, MyFrame::OnSettings)  // Event für Einstellungen
EVT_MENU(ID_Info, MyFrame::OnInfo)
wxEND_EVENT_TABLE()

MyFrame::MyFrame(const wxString& title, SettingsManager* settingsManager)
    : wxFrame(nullptr, wxID_ANY, title, wxDefaultPosition, wxSize(800, 600)),
    chatController(new ChatController(settingsManager)),  // Verwende den SettingsManager
    settingsManager(settingsManager) {

    // Menüleiste erstellen
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(ID_Quit, "Beenden\tAlt-X", "Beendet das Programm");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(ID_Settings, "Einstellungen speichern", "API-Key und Temperatur konfigurieren");
    menuHelp->Append(ID_Info, "Info", "Informationen über das Programm");

    wxMenuBar* menuBar = new wxMenuBar();
    menuBar->Append(menuFile, "&Datei");
    menuBar->Append(menuHelp, "&Hilfe");
    SetMenuBar(menuBar);

    // Statusleiste erstellen
    statusBar = CreateStatusBar();
    SetStatusText("Bereit");

    // Layout-Management
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    // Textfelder für API-Key und Temperatur
    apiKeyTextBox = new wxTextCtrl(this, wxID_ANY, settingsManager->getApiKey(), wxDefaultPosition, wxSize(400, 30), wxTE_PASSWORD);
    temperatureTextBox = new wxTextCtrl(this, wxID_ANY, std::to_string(settingsManager->getTemperature()), wxDefaultPosition, wxSize(400, 30));

    // Textfeld für Benutzereingaben (Nachrichten)
    inputTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 100), wxTE_MULTILINE);  // Initialisiert

    // Antwortbox
    chatResponseTextBox = new wxTextCtrl(this, wxID_ANY, "", wxDefaultPosition, wxSize(400, 300), wxTE_MULTILINE | wxTE_READONLY);

    // Button zum Senden der Nachricht
    wxButton* sendButton = new wxButton(this, ID_Chat, "Nachricht senden");

    // Hinzufügen der Elemente zum Sizer
    sizer->Add(apiKeyTextBox, 0, wxEXPAND | wxALL, 10);
    sizer->Add(temperatureTextBox, 0, wxEXPAND | wxALL, 10);
    sizer->Add(inputTextBox, 0, wxEXPAND | wxALL, 10);  // Input Textbox
    sizer->Add(chatResponseTextBox, 1, wxEXPAND | wxALL, 10);  // Antwortbox
    sizer->Add(sendButton, 0, wxALIGN_CENTER | wxALL, 10);  // Senden-Button

    SetSizerAndFit(sizer);
}

void MyFrame::OnSettings(wxCommandEvent&) {
    try {
        wxLogMessage("Einstellungen Dialog wird geöffnet.");

        // Lade API-Key und Temperatur aus dem SettingsManager
        std::string apiKey = settingsManager->getApiKey();
        float temperature = settingsManager->getTemperature();

        // Debugging-Log: Überprüfe, ob die Werte korrekt geladen wurden
        std::cout << "Geladener API-Key: " << apiKey << std::endl;
        std::cout << "Geladene Temperatur: " << temperature << std::endl;

        // Zeige einen Dialog zum Eingeben des API-Keys
        wxTextEntryDialog apiKeyDialog(this, "Geben Sie den API-Key ein:", "API-Key", apiKey);

        if (apiKeyDialog.ShowModal() == wxID_OK) {
            // Verarbeite die API-Key-Eingabe
            std::string newApiKey = apiKeyDialog.GetValue().ToStdString();

            // Überprüfe, ob der API-Key leer ist
            if (newApiKey.empty()) {
                wxLogError("API-Key darf nicht leer sein.");
                return;
            }

            // Versuche, die Temperatur in float zu konvertieren
            float newTemperature;
            try {
                newTemperature = std::stof(temperatureTextBox->GetValue().ToStdString());  // Konvertiere Temperatur zu float
            }
            catch (const std::exception& e) {
                // Falls die Eingabe ungültig ist, eine Fehlermeldung anzeigen und den Vorgang abbrechen
                wxLogError("Ungültiger Temperaturwert. Bitte geben Sie eine Zahl zwischen 0.0 und 1.0 ein. Fehler: %s", e.what());
                return;  // Beende die Funktion frühzeitig, falls die Temperatur ungültig ist
            }

            // Überprüfe, ob die Temperatur im gültigen Bereich ist
            if (newTemperature < 0.0f || newTemperature > 1.0f) {
                wxLogError("Temperaturwert muss zwischen 0.0 und 1.0 liegen.");
                return;  // Beende die Funktion frühzeitig, falls der Wert außerhalb des gültigen Bereichs liegt
            }

            // Debugging-Log: Einstellungen werden gespeichert
            std::cout << "Speichere neue Einstellungen: API-Key = " << newApiKey << ", Temperatur = " << newTemperature << std::endl;

            // Speichere die neuen Einstellungen (API-Key und Temperatur)
            if (!settingsManager->saveSettings(newApiKey, newTemperature)) {
                wxLogError("Fehler beim Speichern der Einstellungen.");
            }
            else {
                SetStatusText("Einstellungen gespeichert.");
            }
        }
    }
    catch (const std::exception& e) {
        wxLogError("Fehler in OnSettings: %s", e.what());
    }
}

void MyFrame::OnQuit(wxCommandEvent&) {
    Close(true);
}

void MyFrame::OnChat(wxCommandEvent&) {
    // Lese API-Key und Temperatur aus den Textboxen
    std::string apiKey = std::string(apiKeyTextBox->GetValue().mb_str());

    // Überprüfe, ob der API-Key leer ist
    if (apiKey.empty()) {
        wxLogError("API-Key darf nicht leer sein.");
        return;
    }

    float temperature;
    try {
        temperature = std::stof(temperatureTextBox->GetValue().ToStdString());
    }
    catch (const std::exception& e) {
        wxLogError("Ungültiger Temperaturwert. Bitte geben Sie eine Zahl zwischen 0.0 und 1.0 ein. Fehler: %s", e.what());
        return;
    }

    if (temperature < 0.0f || temperature > 1.0f) {
        wxLogError("Temperaturwert muss zwischen 0.0 und 1.0 liegen.");
        return;
    }

    // Speichere API-Key und Temperatur in den SettingsManager
    if (!settingsManager->saveSettings(apiKey, temperature)) {
        wxLogError("Fehler beim Speichern der Einstellungen.");
        return;
    }

    // Sende die Nachricht
    std::string message = std::string(inputTextBox->GetValue().mb_str());
    if (message.empty()) {
        wxLogError("Nachricht darf nicht leer sein.");
        return;
    }

    std::string response = chatController->processChat(message);
    if (response.empty()) {
        wxLogError("Fehler beim Empfangen der Antwort.");
    }
    else {
        chatResponseTextBox->AppendText(wxString::FromUTF8(response) + "\n");
        SetStatusText("Antwort erhalten.");
    }
}

void MyFrame::OnInfo(wxCommandEvent&) {
    wxMessageBox(
        "Information über das Programm\n\n"
        "Der Temperatur-Wert bestimmt, wie kreativ oder zufällig die Antworten des Modells sein sollen. "
        "Ein Wert von 0.0 bedeutet, dass die Antworten sehr deterministisch und vorhersehbar sind, während ein Wert von 1.0 "
        "maximale Zufälligkeit und Kreativität zulässt. Der Wert ist auf 0.7 gesetzt, was eine gute Balance "
        "zwischen Kreativität und Konsistenz bietet.",
        "Info", wxOK | wxICON_INFORMATION, this
    );
}

