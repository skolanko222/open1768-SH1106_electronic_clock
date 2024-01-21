# Dokumentacja projektu: Elektroniczny Zegar z Interfejsem Graficznym

## Autorzy
- Aleksander Siekliński
- Szymon Kolanko

## Data
Styczeń 2024

## Opis Projektu
Celem naszego projektu jest stworzenie elektronicznego zegarka z interfejsem graficznym na wyświetlaczu [1.3inch OLED SH1106](https://www.waveshare.com/wiki/1.3inch_OLED_(A)), który będzie programowany przez interfejs SPI. Zasilanie zegara RTC jest możliwe poprzez zewnętrzną baterię.

## Komponenty Projektu
1. **Płytka Open1768:**
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

![1](https://github.com/skolanko222/systemy_wbudowane/assets/108925571/758ac8e7-8821-4b80-b5d7-f5c3ec3aa62c)
 
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

![2](https://github.com/skolanko222/systemy_wbudowane/assets/108925571/3c0d0563-3eb8-428f-93f8-aba53a455723)

## Elementy opcjonalne
### UART
Przy podłączonym interfejsie UART na porcie 0, mikrokontroler komunikuje aktualną godzinę, minutę oraz sekundę. Wysyłanie zatrzymuje się na czas włączenia trybu konfiguracyjnego. Po jego zakończeniu wyświetlana jest godzina, na którą zegarek został zmieniony, po czym ponownie wysyłana jest aktualna godzina.

## Wykorzystane Peryferia Mikrokontrolera
### Interfejs SPI:
   - Konfiguracja do komunikacji z wyświetlaczem OLED.
### Joystick:
   - Obsługa do nawigacji po interfejsie.
### RTC:
   - Wykorzystywane do śledzenia czasu.
### UART:
   - Diagnostyka
(Opcjonalnie) Dane diagnostyczne o działaniu mikrokontrolera.

## Szkic najważniejszych funkcji
1. **OLED_driver.h**
   - OLED_init() - wysłanie sekwecji inicjalizującej wyświetlacz: reset urządzenia (pin RES), wysłanie rejestrów inicjalizujących parametry wyświetlacza takich jak dzielniki wewnętrzego zegara, ilość cykli zegara potrzebnych do pre-chargu i dischardzu pixeli czy mapowanie pamięci do wyświetlacza.
   - OLED_writeReg(uint_t data) - wysyła bajt danych przez SPI i powiadamia wyświetlacz, że ma traktować nadchodzące dane jako komendę (pin DC -> 0)
   - OLED_writeData(uint_t data) - wysyła bajt danych przez SPI i powiadamia wyświetlacz, że ma traktować nadchodzące dane jako obraz (pin DC -> 1)
   - OLED_display(uint_t * image) - wysyła tablicę reprezentującą obraz do wyświetlacza. Tablica jest jednowymiarowa, lecz każde 8 uint'ów (zaczynając od początku tablicy) reprezentuje jeden wiersz pixelów na wyświetlaczu ( 8x8bit = 64bit), zaś każda wartoś 1/0 bitu oznacza zapalenie lub zgaszenie korespondującego pixela.
W celu zapełnienia całego wyświetlacza, tablica powinna więc przetrzymywać 8*128=1024 uint'ów. 
2. **image.h**
   - drawPixel(unsigned char image[1030], int x, int y, char value) - ustawia danych pixel o współrzędnych (x, y) na wartość 1/0, w tablicy reprezentującej obraz wysyłany do wyświetlacza.
   - drawArray(unsigned char image[1030], int x, int y, int width, int height, unsigned char array[width][height]) - ustawia obszar (prostokąt o wymiarach: width x height, i początku w (x,y)) reprezentowany w tablicy image, który zostanie wyświetlony na ekranie.
3. **rtc_timer.h**
   - RTC_Configuration() - konfiguruje Real Time Clock oraz włącza przerwanie zegara.
   - setDateTime() - zapisuje globalne wartości zegara zapisane przez użytkownika w czasie konfiguracji zegara.
   - readDataTime() - odczytuje obecną datę i godzinę zawartą w RTC i zapisuje je w wartościach globalnych.
