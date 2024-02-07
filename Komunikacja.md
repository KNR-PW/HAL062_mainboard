Komunikacja
===========
Komunikacja wewnatrz Lazika odbywa sie zgodnie po CANie, a z uzytkownikiem
po UARCie. Ramka UARTowa sklada sie z 19 8-bitowych znakow i jest stalej dlugosci.
uint8_t wysylane sa w reprezentacji HEX za pomoca tekstu, wiec kazda liczba
zajmuje 2 znaki. UWAGA! Jesli wysylamy singed int8 to i tak wysylamy go jako uint8_t
i po drugiej stronie jest on z powrotem interpretowany jak signed. Ponizej znajduje sie dokladny sklad kazdej ramki:
* data[0] - '#'(35) - oznaczenie poczatku ramki
* data[1-2] - ID ramki (0-255)
* data[3-18] - dane: 8 zmiennych 8-bitowych

Jesli ramka ma mniej niz 8 znakow pozostale pola wypelniane sa znakiem 'x'.
ID ramek zostaje takie samo podczas calej komunikacji.

Przyklad:
* ID = 71
* dlugosc ramki - 3 znaki
* data[0...2] = {0,11,253}

Wynikowa ramka:
* #47000BFDxxxxxxxxxx

Podzial id ramek:
* Platforma 0-127
* Manipulator 128-191
* Laboratiorium 192-255

Spis komend
===========

## Sterowniki silnikow (ID 20-39)
1. Predkosci referencyjne.
Wysylane jako signed int8 w zakresie +/- 100, gdzie 100 to max do przodu, 0 bez ruchu
* ID = 20
* data[0] - speed prawy przod
* data[1] - speed prawy srodek
* data[2] - speed prawy tyl
* data[3] - speed lewy przod 
* data[4] - speed lewy srodek
* data[5] - speed lewy tyl

2. START/STOP.
Wszytskie sterowniki odbieraja ta sama ramke i ustawiaja flagi w
oparciu o dane z niej 
* ID = 21
* data[0] - start/stop (start = 0x01, stop - wszystko inne (domyslnie 0x00) - powtorzone 3 razy dla bezpieczenstwa)
* data[1] - start/stop
* data[2] - start/stop
* data[3] - prad maksymalny (100 - 100% czyli 50A, 0 - 0% - 0A)
* data[4] - predkosc maksymalna (analogicznie jak z pradem)

4. Polecenie resetu sterowników
* ID - 28
* data[0] 
		data = 0  - nic nie robi
		data = 1  - resetuje statusy błędów
		data = 2  - prze��cza w tryb pracy bez sprz�enie od pr�dko�ci (PWM MODE)
		data = 3  - resetuje ca�y sterownik




##Plytka do kamer
1. START/STOP/Pozycja
* ID - 45
* data[0] - pozycja w pionie -100  - 100 (zadajesz pozycj�)
* data[1] - polecenie przezu� w lewo/ prawo -100  - 100  (zadajesz �e ma si� przesun��, nie trzyma pozycji)
* data[3] - zabij to g�wno (jak wy�lesz 0x01 to wy��czy nap�dy) (na wypadek jakby co� posz�o nie tak mocno), (jak wy�lesz 0x02 to zresetujesz p�ytk�)


##Magnetometr
Float jako 4 bajty
* ID = 50
* data [0] - MSB
* ...
* data[3] - LSB
##Autonomia
Mryganie lampkami
* ID = 51
* data [0] - 1 - zapala lampke, cokowlwiek innego - gasi


## Manipulator (ID 128-191)
ramki wewnetrzne manipulatora 141-191 

Tryby manipulatora:
*  0  - Disable
*  1  - Hold
*  2  - Katy
*  3  - predkosci zloczowe
*  4  - xyz... global
*  5  - predkosci global
*  6  - predkosci tool
*  7  - delta pozycji tool
*  8  - predkosci global bez 6
*  9  - predkosci tool bez 6
*  10  - delta pozycji tool bez 6
*  11  - predkosci global 123, 456 - predkosci zlaczowe
*  12  - predkosci tool 123,  456 - predkosci zlaczowe

Ramki Zewnetrzne:
1. START/STOP
* ID - 128
* data[0] - start/stop (start = 0x01, stop - wszystko inne (domyslnie 0x00) - powtorzone 3 razy dla bezpieczenstwa)
* data[1] - start/stop
* data[3]-data[4] - Mode
* data[5]-data[6] - 0 normalnie, 1 DoF6 w trybie predkosci, DoF456 w trybie predkosci

2. Dane pomiarowe. Katy. 157-162 opisane w ramkach wewnetrznych

3. Ramka od operatora 
* ID - 129
* data [0] - angle1 - MSB 
* data [1] - ...
* data [2] - ...
* data [3] - angle1 - LSB
* data [4] - angle2 - MSB 
* data [5] - ...
* data [6] - ...
* data [7] - angle2 - LSB
* ID - 130
* data [0] - angle3 - MSB 
* data [1] - ...
* data [2] - ...
* data [3] - angle3 - LSB
* data [4] - angle4 - MSB 
* data [5] - ...
* data [6] - ...
* data [7] - angle4 - LSB
* ID - 131
* data [0] - angle5 - MSB 
* data [1] - ...
* data [2] - ...
* data [3] - angle5 - LSB
* data [4] - angle6 - MSB 
* data [5] - ...
* data [6] - ...
* data [7] - angle6 - LSB

Ramki Wewnetrzne:
1. Ramka od koncentratora do kazdego z DoFów i chwytaka
* ID - 151-157
* data [0] - start/stop 
* data [1] - start/stop
* data [2] - mode
* data [3] - mode
* data [4] - data - MSB 
* data [5] - ...
* data [6] - ...
* data [7] - data - LSB

2. Ramka od kazdego z DoFów do koncentratora z pomiarem polozenia katowego
* ID - 158-163
* data [0] - angle - MSB 
* data [1] - ...
* data [2] - ...
* data [3] - angle - LSB
