# Vektorsynthesizer mit modularen analogen Filtern

In diesem Masterprojekt soll ein Vektorsynthesizer auf Mikrocontroller-Basis entwickelt werden, der sowohl beliebige vom Nutzer bereitgestellte Wellenformen zur Klangerzeugung nutzen kann, als auch verschiedene analoge Filterboards (mit unterschiedlichen Filtercharakteristiken) modular verwenden kann. Bei der Konstruktion soll auf dokumentierte und verifizierte analoge Filterboards zurückgegriffen werden (Mutable Instruments Shruthi).

Die Mikrocontroller-Plattform ist vorzugsweise die ARM Cortex-M4-basierte Arduino- kompatible Teensy 3.x Plattform, die sowohl ausreichend Rechenleistung und spezifische Audio-Funktionen, als auch Möglichkeiten der Ansteuerung von notwendiger Peripherie (Steuerung der Filterparameter, Zugriff auf SD-Card für externe Wellenformen, Ansteuerung eines LC-Displays) bietet.

Bei der Implementierung soll besonderes Augenmerk auf die Echtzeit-Kontrolle der Synthese- Parameter gelegt werden. Der Synthesizer soll mit einem diesem Anspruch gerecht werdenden User-Interface ausgestattet werden.
