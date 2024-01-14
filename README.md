# Dokumentacja projektu: Elektroniczny Zegar z Interfejsem Graficznym

## Autorzy
- Aleksander Siekliński
- Szymon Kolanko

## Data
Styczeń 2024

## Opis Projektu
Celem naszego projektu jest stworzenie elektronicznego zegarka z interfejsem graficznym, który będzie programowany przez interfejs SPI.

## Komponenty Projektu
1. **Mikrokontroler LPC1768:**
   - Centralny element sterujący projektem.
   - Odpowiada za obsługę interfejsu SPI i komunikację z wyświetlaczem.
     
2. **Wyświetlacz Monochromatyczny OLED 1,3":**
   - Służy do wyświetlania interfejsu graficznego zegarka.
   - Programowany za pomocą interfejsu SPI.
     
3. **Bateria:**
   - Służy do utrzymania funkcji RTC w mikrokontrolerze, gdy płytka odpięta jest od innych rodzajów zasilania.

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
