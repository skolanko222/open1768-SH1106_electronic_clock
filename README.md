# Dokumentacja projektu: Elektroniczny Zegar z Interfejsem Graficznym

## Autorzy
- Aleksander Siekliński
- Szymon Kolanko

## Data
Styczeń 2024

## Opis Projektu
Celem naszego projektu jest stworzenie elektronicznego zegarka z interfejsem graficznym na wyświetlaczu [1.3inch OLED SH1106](https://www.waveshare.com/wiki/1.3inch_OLED_(A)), który będzie programowany przez interfejs SPI. Zasilanie zegara RTC jest możliwe poprzez zewnętrzną baterię.

## Komponenty Projektu
1. **Mikrokontroler LPC1768:**
   - Centralny element sterujący projektem.
   - Odpowiada za obsługę interfejsu SPI i komunikację z wyświetlaczem.
     
2. **Wyświetlacz Monochromatyczny OLED 1,3":**
   - Służy do wyświetlania interfejsu graficznego zegarka.
   - Programowany za pomocą interfejsu SPI.
     
3. **Bateria:**
   - Służy do utrzymania funkcji RTC w mikrokontrolerze, gdy płytka odpięta jest od innych rodzajów zasilania.
  
## Budowa urządzenia
1. **Podłączenie wyświetlacza OLED:**
   - Wyswietlacz należy podłączyć do portu SPI1, który znajduje się w lewym dolnym rogu płytki pod portem SSP.
   - Należy upewnić się, że piny wyświetlacza zostały podłączone do headera SPI wg. schematu: (VCC -> 3.3V, GND - > GND, DIN -> MOSI, CLK -> SCK, CS -> NSS)
   - Ostatni pin wyświetlacza - RES - który wystaje poza header, należy osobnym kabelkiem podłączyć do pinu P2.0.
     
2. **Konfiguracja UART (opcjonalne):**
   - Interfejs UART należy podłączyć do portu UART0 - wyłącznie w celach diagnostycznych.
   - Konfiguracja terminala szeregowego: baud rate: 115200, długość słowa bitowego: 8 bit, brak parzystości, 1 bit stopu.
     
## Sposób Działania
1. **Uruchomienie Urządzenia:**
   - Po włączeniu urządzenia użytkownik będzie mógł rozpocząć konfigurację zegarka. Aby to zrobić, powinien nacisnąć joystick na płytce.

2. **Konfiguracja Zegarka:**
   - Podczas konfiguracji zegarka, mrugająca cyfra będzie wskazywać ten element który jest aktualnie w trybie edycji.

3. **Obsługa Przycisków:**
   - Przesuwanie się między polami konfiguracyjnymi będzie obsługiwane poprzez przesunięcie joysticka w lewo lub w prawo.
   - Zmiana wartości aktualnej cyfry odbywa się poprzez przesunięcie joysticka w górę lub w dół.

4. **Zakończenie Konfiguracji:**
   - Po zakończeniu konfiguracji użytkownik powinien ponownie nacisąć joystick, co zapisze wprowadzone ustawienia.

## Elementy opcjonalne
### UART
Przy podłączonym interfejsie UART na porcie 0, mikrokontroler komunikuje aktualną godzinę, minutę oraz sekundę. Wysyłanie zatrzymuje się na czas włączenia trybu konfiguracyjnego. Po jego zakończeniu wyświetlana jest godzina, na którą zegarek został zmieniony, po czym ponownie wysyłana jest aktualna godzina.

## Wykorzystane Peryferia Mikrokontrolera
### Interfejs SPI:

Konfiguracja do komunikacji z wyświetlaczem OLED.
### Joystick:

Obsługa do nawigacji po interfejsie.
### RTC:

Wykorzystywane do śledzenia czasu.

### UART:

(Opcjonalnie) Dane diagnostyczne o działaniu mikrokontrolera.

## Szkic algorytmu
### Empty
Empty
