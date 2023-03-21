# Einleitung
Das Framework soll (mir) die Entwicklung von ESP Devices erleichtern. Es einen Webserver zur Konfiguration und stellt einen Websocket so wie eine Rest-API zum zugriff auf die Daten bereit. Die Funktionen setzen sich aus einzelnen Element-Klassen zusammen, die Parent-Klasse stellt Grundfunktionen für die Konfiguration und den Datenaustausch bereit.  
Inzukunft ist noch eine Handler-Funktion für die Elemente angedacht um diese dynamisch zu erstellen und die Abarbeitung und den Datenaustausch zu steuern.  
Die Enwicklung erfolgt mit der PlatformIO-IDE integriert in Visual Studio Code.

## JSON-Websocket
Der Websocket ist über http://ip-adresse/ws erreichbar. Das Modul sendet zyklisch an alle mit dem Websocket verbundenen Clients den kompletten Datensatz im JSON-Format.
Es ist zu beachten dass die verwendete Hardware nur eine begrenzte Anzahl Websocket-Verbindungen zur Verfügung stellt.
## Rest-API
Die Rest-API nimmt alle HTTP-Methoden unter der Adresse http://ip-adresse/api entgegen. Die Verarbeitung der Daten erfolgt im Anwenderprogramm und kann flexibel angepasst werden.
## WiFi
Als Netzwerkschnittstelle fungiert das standardmäsige WiFi des ESP. Dies hat den Vorteil dass kein extra Netzwerkkabel in den Verteilerschrank gezogen werden muss.
## Konfiguration
Alle Einstellungen können über die integrierte Webseite erfolgen.

# WiFi Konfiguration
Das Netzwerk lässt sich über eine eigene Webseite konfigurieren werden oder durch [hochladen der System-Konfiguration](#dateien-hochladen) als **sysConfig.json**.
```json
{
  "wifi": [
    {
      "ssid":"your_ssid",
      "pass":"your_password",
      "dhcp":true,
      "ip":"x.x.x.x",
      "gateway":"x.x.x.x",
      "subnet":"x.x.x.x"
    }
  ]
}
```
Die Konfiguration kann jedoch nicht zurück gelesen werden. Auf der WiFi-Webseite werden alle konfigurierten Informationen ausser das Passwort angezeigt.
## AP Mode
Wenn das Modul sich nicht mit dem angegebenen Netzwerk verbinden kann oder noch kein Netzwerk konfiguriert wurde, erstellt es einen SoftAP um die Konfiguration zu ermöglichen. Der SoftAP ist nicht für den Dauerbetrieb gedacht, denn nach einem 5 Minuten Timeout versucht das Modul erneut die Verbindung herzustellen. Dies soll dazu dienen dass sich das Modul selbstständig wieder mit dem Netzwerk verbindet, auch wenn dieses zum Boot-Zeitpunkt nicht erreichbar war.  
> SSID: JCA_IOT_*ESP-ChipID*  
> PASS: JCA_IOT_DEFAULTAP  
> DEV-IP: http://192.168.1.1  
> HOST-IP: DHCP

# System Funktionen
Die System-Funktionen und auch die  WiFi Einstellungen sind Passwort geschützt. Das Passwort ist hart codiert und dient eher dazu dass keine Gäste im WLAN etwas "zerstören" können.  
> User: Admin  
> Pass: Admin
## Konfigration sichern
Hier läst sich die aktuelle ModBus Konfiguration als **config.json** herunterladen. Dies kann zur Datensicherung oder zum Editieren/Erweitern der Konfiguration dienen.
## Dateien hochladen
Über die Upload-Funktion können beliebige Dateien hochgeladen werden. Auf diesem Weg kann die Webseite oder deren Erscheinug angepasst werden oder auch Konfigurations-Dateien direkt geschrieben werden.
Es sollte darauf geachtet werden dass der Speicher des Kontrollers eingeschränkt ist, default sind 2MB.
## Firmware Update
Auf diesem Weg können einfach von der Arduino IDE exportierte Binarys hochgeladen werden und der Kontroller neu geflasht werden. Die Dateien und somit auch die Konfiguration sind davon nicht betroffen.
## Restart
Reboot tut immer Gut.
# Abhänigkeiten
Die verwendeten Bibliotheken können alle über den Libraray Manager der Arduino IDE bezogen werden
- [ArduinoJson](https://github.com/bblanchon/ArduinoJson)
- [ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP) (ESP8266)
- [AsyncTCP](https://github.com/me-no-dev/AsyncTCP) (ESP32)
- [ESPAsyncWebServer](https://github.com/me-no-dev/ESPAsyncWebServer)
- [ModbusRTU](https://github.com/emelianov/modbus-esp8266)

Für das Designe der Webseite wurde auf das kompakte CSS-Framework [Pico.css](https://picocss.com/) zurück gegiffen. Wirklich ein einfachs und dennoch schön anzusehendes Framework mit dem sogar ein Nicht-Webentwickler wie ich gut klar kommt.

