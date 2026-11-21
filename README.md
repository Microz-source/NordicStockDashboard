# Nordic Stock Dashboard

## Prosjektbeskrivelse

Nordic Stock Dashboard er en applikasjon utviklet som en del av Arbeidskrav 2 i programmering og prosjektering.

Programmet viser historiske aksjekurser for tre norske børsnoterte selskaper:

- Mowi ASA
- SpareBank 1 SMN
- Yara International ASA

Brukeren kan velge en aksje og se historiske sluttkurser i en tabell og en linjegraf.

## Teknologier

- C++
- Qt 6 og Qt Quick
- QML
- CMake
- EODHD REST API
- JSON

## Funksjoner

- Henting av historiske aksjekurser fra EODHD
- Behandling av JSON-data i C++
- Visning av aksjekurser i en tabell
- Linjegraf med dato og sluttkurs
- Valg mellom tre aksjer

## Status

Prosjektet er under utvikling. Flere funksjoner, testing og dokumentasjon gjenstår.

## API-nøkkel

Programmet bruker EODHD API. API-nøkkelen lagres som miljøvariabelen `EODHD_API_TOKEN` i Qt Creator og er ikke inkludert i kildekoden.

## Innlevering

Arbeidskrav 2 – 30. november 2026.
