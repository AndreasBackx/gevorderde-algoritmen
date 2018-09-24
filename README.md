[![Build Status](https://travis-ci.org/MartenBE/gevorderde-algoritmen.svg?branch=master)](https://travis-ci.org/MartenBE/gevorderde-algoritmen)


Persoonlijke speeltuin voor het vak Gevorderde Algoritmen (E765015)

*Deze repository kan fouten en onjuistheden bevatten*

-----

### Hoe werken met CMake projecten (tl;dr)

1. **Bestanden toevoegen in `CMakeLists.txt`**

2. **project genereren**

```
./generate_makefile.sh 
```
  - Heeft internet nodig om googletest binnen te halen
  - Maakt een map `build` aan met de nodige project-bestanden voor jouw IDE
  - Moet elke keer opnieuw worden uitgevoerd als je bestanden toegevoegd of verwijderd van het project (dus telkens na stap 1)

3. **Elke keer als je wil compileren**

```
make
./unittest
``` 

  - Zorg ervoor dat je hiervoor in de map `build` zit
  - `./unittest` zorgt ervoor dat de unittesten worden uitgevoerd
  
4. **Als je een clean build wil doen**

```
rm -rf build
```

- Begin dan opnieuw vanaf stap 2

-----

### Hoe werken met CMake projecten (uitgebreid)

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

- De folder `opgave` bevat de originele opgave van https://intranet.tiwi.ugent.be/
- `main.cpp`, `componentengraaf.h` zijn de eigen codebestanden met de oplossing
- `takkenlijst.txt`, `woordenlijst.txt` zijn input-bestanden voor de eigen codebestanden met de oplossing
- `test_componentengraaf.cpp` bevat de unittests voor `componentengraaf.h`: deze tests worden uitgevoerd met het googletest framework (https://github.com/google/googletest)
- `CMakeLists.txt`, `CMakeLists.txt.in`, `generate_makefile.sh` worden gebruikt om het project te builden en het project en tests te runnen

#### Eigen project maken

Om je eigen project te maken moet je in `CMakeLists.txt` aangeven welke codebestanden bij het project horen:

```
set(SOURCES
    # voeg hier alle cpp-bestanden toe nodig om het main-programma te compileren (indien aanwezig)
    )

set(ADDITIONAL_INPUT_FILES
    # voeg hier eventuele extra input bestanden toe die door het main-programma worden gebruikt
    )

set(TEST_SOURCES
    # voeg hier alle cpp-bestanden toe nodig om de googletest unittesten te compileren
    )

set(ADDITIONAL_TEST_FILES
    # voeg hier eventuele extra input bestanden toe die door de googletest-bestanden worden gebruikt
    )
```

Bijvoorbeeld:

```
set(SOURCES
    main.cpp
    )

set(ADDITIONAL_INPUT_FILES
    takkenlijst.txt
    woordenlijst.txt
    )

set(TEST_SOURCES
    test_componentengraaf.cpp
    )

set(ADDITIONAL_TEST_FILES
    )
```

Telkens dit wordt aangepast, moet je het project opnieuw genereren.

#### Het project genereren en de code compileren

Het buildsysteem wordt gegenereerd door CMake (https://cmake.org/). CMake laat toe om afhankelijk van de wensen van de gebruiker build- of projectfiles te genereren voor jouw editor of buildsysteem naar keuze (make, Visual Studio, XCode, ...).

De volgende stappen genereren via CMake op linux een make project dat dan op de commandline kan worden uitgevoerd. Indien je een ander IDE of besturingssysteem gebruikt, kan je eenvoudig met behulp van CMake bv. een Visual Studio project op Windows genereren om zo te ontwikkelen (zie onderaan). 

```
./generate_makefile.sh
```

De bestanden voor jouw IDE (de makefiles in ons geval) worden gegenereerd in de map `build`. Deze map word zelf door `./generate_makefile.sh` aangemaakt.

Nu kan je na elke aanpassing in een code bestand telkens `make` en `./unittest` uitvoeren in de map `build` om te compileren en de unittests te runnen (`make && ./unittest` doet dit in een enkel commando). De unittests zitten in een eigen executable genaamd `unittest`.

Indien je het project compleet opnieuw van scratch wil builden, hoef je simpelweg de gegenereerde folder `build` te verwijderen en de stappen in de lijst opnieuw uit te voeren. Let op dat `./generate_makefile.sh` een internet connectie nodig heeft om het googletest framework binnen te halen (voor `make` en `./unittest` is dit niet nodig).

De commando's die je dus nodig hebt zijn:

```
./generate_makefile.sh 
cd ./build
make
./unittest
``` 

#### Windows (Visual Studio)

CMake werkt ook op windows en genereert dat Visual Studio projecten (indien dat is geïnstalleerd). Je genereert dit als volgt:

1. Maak zelf een `build` map aan
2. Ga naar de `build` map
3. Voer op de commandline `cmake ..` uit
4. open het gegenereerde .sln-bestand
5. Eenmaal geopend duid je het subproject met jouw code als hoofdproject aan

Noot: het kan zijn dat de header files niet zijn opgenomen in het visual studio project. In dat geval kan je de .h-files ook toevoegen aan `CMakeLists.txt`.

### Extra: developpen met clang tools

Voor de gebruikers van (Neo)Vim, Emacs, Visual Studio Code, ... met clang tools wordt er een `compile_commands.json` aangemaakt inclusief informatie voor het helpen bij formatting, intellisense, autocompletion voor zowel header- als cpp-files. Dit gebeurt via https://github.com/Sarcasm/compdb, wat wel zelf nog geïnstalleerd moet worden via pip (`pip install compdb`).

-----

### ToDo's

#### 2018-2019

- TBA

#### 2017-2018

- Stroomnetwerken: statistieken genereren en probleem oplossen
- Boyer Moore 2e suffix en extra varianten 1e suffix implementeren
- Zoveel mogelijk linken naar de algemene libraries

#### 2016-2017

Het is een puinhoop -> alles eens uitkuisen en zoveel mogelijk linken naar de algemene libraries
