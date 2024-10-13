#ifndef MYFRAME_H
#define MYFRAME_H

#include <wx/wx.h>
#include "chatcontroller.h"
#include "settingsmanager.h"

class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title, SettingsManager* settingsManager);

    void OnQuit(wxCommandEvent& event);
    void OnChat(wxCommandEvent& event);
    void OnSettings(wxCommandEvent& event);  // Neuer Menüpunkt für Einstellungen
    void OnInfo(wxCommandEvent& event);

private:
    wxTextCtrl* inputTextBox;
    wxTextCtrl* chatResponseTextBox;
    wxTextCtrl* apiKeyTextBox;
    wxTextCtrl* temperatureTextBox;
    wxStatusBar* statusBar;

    ChatController* chatController;
    SettingsManager* settingsManager;  // Hinzufügen des SettingsManagers

    wxDECLARE_EVENT_TABLE();
};

#endif
