# Wiring Node Sensor

| Komponen | Pin Komponen | ESP32-C3 |
|---|---|---|
| LoRa SX1278 | NSS/SS | GPIO 5 |
| LoRa SX1278 | RST | GPIO 14 |
| LoRa SX1278 | DIO0 | GPIO 2 |
| LoRa SX1278 | SCK | GPIO 6 (default SPI) |
| LoRa SX1278 | MOSI | GPIO 7 (default SPI) |
| LoRa SX1278 | MISO | GPIO 10 (default SPI) |
| DHT22 | Data | GPIO 4 |
| Soil Sensor | Analog Out | GPIO 3 (ADC1_CH3) |
| Semua | VCC | 3.3V |
| Semua | GND | GND |

> **Penting:** Modul LoRa SX1278 beroperasi di 3.3V. Jangan hubungkan ke 5V.
> Antena wajib terpasang sebelum menyalakan radio.
