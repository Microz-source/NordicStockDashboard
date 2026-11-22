# Nordic Stock Dashboard

Nordic Stock Dashboard er et program laget med Qt, C++ og QML. Programmet henter historiske aksjekurser fra EODHD og viser dem i en graf og en tabell.

Prosjektet er laget som en del av Arbeidskrav 2 i programmering og prosjektering.

## Funksjoner

- Viser aksjekurser for Mowi ASA, SpareBank 1 SMN og Yara International ASA.
- Viser dato, åpningskurs og sluttkurs i en tabell.
- Viser historiske sluttkurser i en graf.
- Lar brukeren velge 7, 30 eller 90 dager.
- Lar brukeren sortere tabellen etter dato.
- Har en knapp for å oppdatere aksjekursene.
- Viser en melding mens data hentes.
- Viser feilmeldinger hvis noe går galt.
- Håndterer raske bytter mellom aksjer.

## Teknologier

Programmet er laget med C++, Qt 6, QML og CMake. Aksjekursene hentes fra EODHD gjennom et REST API.

## Hvordan kjøre programmet

1. Last ned prosjektet fra GitHub.
2. Åpne `CMakeLists.txt` i Qt Creator.
3. Bruk Qt 6 med en kompatibel C++-kompilator.
4. Legg inn din egen API-nøkkel fra EODHD i Qt Creator under **Projects → Run → Environment**, med navnet `EODHD_API_TOKEN`.
5. Bygg og kjør programmet.

Programmet er utviklet med Qt 6.11.1 og MinGW 64-bit.

API-nøkkelen skal ikke legges direkte i kildekoden eller publiseres på GitHub.

## Viktige filer

- `main.cpp` starter programmet.
- `stockapiclient.cpp` og `stockapiclient.h` henter aksjedata fra EODHD.
- `stockmodel.cpp` og `stockmodel.h` lagrer aksjedata.
- `Main.qml` inneholder brukergrensesnittet.
- `StockChart.qml` viser grafen.
- `CMakeLists.txt` brukes til å bygge prosjektet.

## Testing

Programmet er testet med ulike aksjer, tidsperioder, sortering, oppdatering, nettverksfeil, ugyldige data og raske aksjebytter.

Testene er dokumentert i innleveringsrapporten.

## Begrensninger

Programmet viser historiske aksjekurser, ikke sanntidskurser. Det krever internett og tilgang til EODHD.
