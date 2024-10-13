---
# Chat mit Codestral

Ein einfaches GUI-basiertes Chat-Programm in C++ unter Verwendung von wxWidgets, das über die Codestral-API kommuniziert. Die Anwendung ermöglicht es Benutzern, eine Unterhaltung mit einem KI-Assistenten zu führen, wobei die API-Schlüssel und Parameter in der Benutzeroberfläche eingegeben und gespeichert werden können.

## Codestral API-Key

Um diese Anwendung nutzen zu können, benötigst du einen **Codestral API-Key**. Diesen kannst du **kostenlos** bei Mistral erhalten. Aktuell ist die Nutzung der Codestral-API noch kostenfrei. Eine Registrierung mit deinem Namen und deiner Telefonnummer ist allerdings erforderlich.

Du kannst dich über folgenden Link registrieren und deinen API-Key erhalten:
[Codestral API Console](https://console.mistral.ai/codestral)

## Inhalt
- [Installation](#installation)
- [Verwendung](#verwendung)
- [Funktionen](#funktionen)
- [Projektstruktur](#projektstruktur)
- [Technologien](#technologien)
- [Lizenz](#lizenz)

## Installation

### Voraussetzungen
- **C++20** (oder höher)
- **wxWidgets** Bibliothek (Version 3.2 oder höher)
- **CURL** Bibliothek für die HTTP-Kommunikation
- **nlohmann/json** Bibliothek für die Verarbeitung von JSON-Daten
- **tinyxml2** für die XML-Konfiguration
- **vcpkg** für die Paketverwaltung (optional)

### Schritte

1. **wxWidgets installieren**
   - Installiere wxWidgets, entweder manuell oder über ein Paketverwaltungstool wie vcpkg.

2. **CURL und andere Abhängigkeiten installieren**
   - Stelle sicher, dass **CURL** installiert ist, um HTTP-Anfragen zu versenden.
   - Benutze `vcpkg` für die Installation von `nlohmann/json` und `tinyxml2`:
     ```bash
     vcpkg install curl nlohmann-json tinyxml2
     ```

3. **Projekt kompilieren**
   - Nutze Visual Studio oder ein anderes C++-Build-System, um das Projekt zu kompilieren. Stelle sicher, dass die Abhängigkeiten korrekt eingebunden sind.

4. **Settings-Datei**
   - Lege eine `settings.xml`-Datei im Hauptverzeichnis des Projekts ab, die API-Schlüssel und andere Konfigurationen enthält:
     ```xml
     <Settings>
         <ApiKey>DEIN_API_KEY</ApiKey>
         <Temperature>0.7</Temperature>
     </Settings>
     ```

## Verwendung

1. **Starten der Anwendung**: Die Anwendung wird mit einer grafischen Benutzeroberfläche gestartet.
2. **API-Key Eingabe**: Trage den API-Key direkt in die GUI ein. Alternativ kann der API-Key auch aus der `settings.xml`-Datei geladen werden.
3. **Nachricht Senden**: Gib eine Nachricht in das entsprechende Textfeld ein und klicke auf "Nachricht senden". Die Nachricht wird an die Codestral-API gesendet und die Antwort wird im Ausgabebereich angezeigt.
4. **Temperatur Einstellung**: Die Temperatur beeinflusst die Kreativität der Antworten des Modells. Du kannst den Temperaturwert zwischen 0.0 und 1.0 anpassen.

## Funktionen

- **Chat-System**: Der Benutzer kann Nachrichten über eine grafische Oberfläche (GUI) eingeben und an die Codestral-API senden. Die Antwort des Modells wird in der GUI angezeigt.
- **API-Key Verwaltung**: Der Benutzer kann den API-Key und die Temperatur über die GUI eingeben, speichern und ändern.
- **Dynamische Anpassung**: Die Temperaturanpassung erlaubt es dem Benutzer, die Kreativität des Modells zu steuern.
- **XML-basierte Konfigurationsdatei**: Die Einstellungen (API-Key und Temperatur) werden in einer `settings.xml`-Datei gespeichert und können von dort aus geladen werden.

## Projektstruktur

```bash
├── apicommunication.cpp   # Logik für die Kommunikation mit der Codestral API
├── apicommunication.h     # Header für API-Kommunikation
├── chatcontroller.cpp     # Steuert die Logik des Chat-Systems
├── chatcontroller.h       # Header für Chat-Controller
├── main.cpp               # Einstiegspunkt der Anwendung, Initialisierung von wxWidgets
├── myframe.cpp            # Hauptframe der GUI-Anwendung
├── myframe.h              # Header für die GUI-Logik
├── settings.xml           # Beispiel für die Konfigurationsdatei
├── settingsmanager.cpp     # Verwalten von API-Schlüssel und Temperatureinstellungen
├── settingsmanager.h      # Header für SettingsManager
```

## Technologien

- **wxWidgets**: Verwendet für die GUI-Erstellung.
- **CURL**: Für die HTTP-Kommunikation mit der Codestral-API.
- **nlohmann/json**: Zum Parsen und Erstellen von JSON-Objekten.
- **tinyxml2**: Zum Speichern und Laden von Einstellungen in einer XML-Datei.
- **C++20**: Die Anwendung nutzt moderne C++-Funktionen, einschließlich Smart Pointern und neuen Sprachfeatures.

## Lizenz

Dieses Projekt steht unter der MIT-Lizenz.

---
