# Labor 1: LED‐Ansteuerung

## Zielstellung:
In der Laborvorlesung zur Einführung in den Aufbau der Laborhardware sind die grundsätzlichen Zusammenhänge zwischen Eingabegeräten (Schalter/Taster) und einfachen Digitalsignalen (LED on/off) erläutert worden. Im dazugehörigen Laborversuch werden diese Zusammenhänge vertieft und praktisch angewandt. Die einfachsten APIs zur Nutzung von digitalen GPIOs sind DigitalIn und DigitalOut. Werden mehrere digitale Signale gemeinsam angesteuert ist die Verwendung der API BusOut oft hilfreich.

## Aufgabenstellung:
Auf dem Velleman TM - Board befinden sich sechs LEDs und fünf Schalter/Taster. Im Laborversuch 1 werden die LEDs in unterschiedlichen Konfigurationen als Lauflicht programmiert.

Mit dem Schiebeschalter SW1, im Bild oben rechts, wird die Laufrichtung der Leuchtpunkte festgelegt. Der Status
der Drucktasten SW2 und SW3 bestimmt die Art und Weise der LED‐Ausgaben und mit den Tasten SW4 und SW5
wird die Geschwindigkeit der umlaufenden „Punkte“ festgelegt.
- Nach dem POR und ohne SW2/SW3 blinken alle LEDs mit einer Ein‐ bzw. Ausschaltzeit von 300 Millisekunden.
- SW2 aktiviert den umlaufenden Leuchtpunkt – RED1, ORANGE1, GREEN1, GREEN2, ORANGE2, RED2, ... - es ist immer nur eine LED eingeschaltet.
- SW3 aktiviert das Leuchtband – anschalten RED1, ORANGE1, GREEN1,... bis alle leuchten, danach RED1, ORANGE1, GREEN1, ... nacheinander wieder ausschalten.
- SW4 erhöht die Blinkfrequenz auf 50ms Ein‐ bzw. Ausschaltzeit
- SW5 verlängert die Blinkschaltzeit auf 2 Sekunden
- SW1 legt die „Umlaufrichtung“ der Leuchtsignale fest.
- Gleichzeitiger Tastendruck auf SW2 und SW3 oder SW4 und SW5 werden als nicht „Fehlbetätigung“ der Tasten erkannt und nicht ausgewertet.

## Zur Bewertung:
- Vor dem Labortermin sendet jedes Team mir Ihre Datei „main.cpp“ als Anhang der Email. Bitte keinesfalls den Quelltext in die Email direkt einkopieren. Ich erwarte eine sinnvolle Formatierung des Quelltextes. Am einfachsten erreichen Sie dies, wenn Sie aus dem „Programm Workspace“ die main.cpp exportieren (CRTL‐E). Prüfen Sie die Formatierung Ihrer Datei mit dem Notepad‐Editor!
- In jedem Quellfile sind die Namen der Teammitglieder im Kommentartext verzeichnet. Termin für die main.cpp ist der 09.05.2022 um 12.00 Uhr – Betreff: "MPRO SL SS2022 Laboraufgabe 1"
- Teams die keine Lösung an mich gesandt haben, werden vom Labor ausgeschlossen. Das Gleiche gilt für nahezu identische Lösungen von unterschiedlichen Gruppen. Ich untersuche nicht, wer von wem kopiert hat! Ich schließe gerechterweise alle betroffenen Gruppen vom Labor aus.