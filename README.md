# SystemC_FSM
Kurzes einführendes Beispiel für SystemC anhand einer Fußgängerampel

## Installation
Am Beispiel für Visual Studio

* Herunterladen der Libraries
  * https://www.accellera.org/downloads/standards/systemc
  * Für installierte Version von Visual Studio Projekt erstellen

## Projekt erstellen

* VC++ Verzeichnisse
    * Includeverzeichnis:  …\systemc-2.3.3\systemc-2.3.3\src
    * Libaryverzeichnis:	…\systemc-2.3.3\systemc-2.3.3\msvc10\SystemC\Debug
* C/C++
    * Codegenerierung
      * Laufzeitgenerierung: Multithread-Debug-DLL (/MDd)
    * Befehlzeile:           \vmg
* Linker
    * Eingabe
      * Zusätzliche Abhängigkeiten:	systemc.lib
