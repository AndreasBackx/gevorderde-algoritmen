[![Build Status](https://travis-ci.org/MartenBE/gevorderde-algoritmen.svg?branch=master)](https://travis-ci.org/MartenBE/gevorderde-algoritmen)


Persoonlijke speeltuin voor het vak Gevorderde Algoritmen (E765015)
*Deze repository kan fouten en onjuistheden bevatten*

-----

### Hoe te builden:

#### Folder structuur

Elke labo folder heeft de volgende layout; bv. labo-6-samenhangende-componenten

```
├── CMakeLists.txt
├── CMakeLists.txt.in
├── componentengraaf.h
├── generate_makefile.sh
├── main.cpp
├── opgave
│   ├── graaf.h
│   ├── keten.cpp
│   ├── opgave06.html
│   ├── takkenlijst
│   └── woordenlijst
├── takkenlijst.txt
├── test_componentengraaf.cpp
└── woordenlijst.txt
```

- De folder `opgave` bevat de originele opgave vanop https://intranet.tiwi.ugent.be/
- `main.cpp`, `componentengraaf.h` zijn de eigen codebestanden met de oplossing
- `takkenlijst.txt`, `woordenlijst.txt` zijn input-bestanden voor de eigen codebestanden met de oplossing
- `test_componentengraaf.cpp` bevat de unittests voor `componentengraaf.h`: deze tests worden uitgevoerd met het googletest framework (https://github.com/google/googletest)
- `CMakeLists.txt`, `CMakeLists.txt.in`, `generate_makefile.sh` worden gebruikt om het project te builden en het project en tests te runnen

#### Het project builden

Het buildsysteem wordt gegenereerd door CMake (https://cmake.org/). CMake laat toe om afhankelijk van de wensen van de gebruiker build- of projectfiles te genereren voor jouw editor of buildsysteem naar keuze (make, Visual Studio, XCode, ...).

De volgende stappen genereren via CMake op linux een make project dat dan op de commandline kan worden uitgevoerd. Indien je een ander IDE of besturingssysteem gebruikt, kan je eenvoudig met behulp van CMake bv. een Visual Studio project op Windows genereren om zo te developpen.

// TODO: verder aanvullen

-----

### TODO

- Stroomnetwerken: statistieken genereren en probleem oplossen
- Boyer Moore 2e suffix en extra varianten 1e suffix implementeren
