#include <wx/wx.h>
#include "myframe.h"
#include "settingsmanager.h"

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    SettingsManager settingsManager("settings.xml");
    settingsManager.loadSettings();

    MyFrame* frame = new MyFrame("Chat mit Codestral", &settingsManager);
    frame->Show(true);

    return true;
}
