# Wiring Gateway

| Komponen | Pin Komponen | ESP32 DevKit |
|---|---|---|
| LoRa SX1278 | NSS/SS | GPIO 5 |
| LoRa SX1278 | RST | GPIO 14 |
| LoRa SX1278 | DIO0 | GPIO 2 |
| LoRa SX1278 | SCK | GPIO 18 (default VSPI) |
| LoRa SX1278 | MOSI | GPIO 23 (default VSPI) |
| LoRa SX1278 | MISO | GPIO 19 (default VSPI) |
| Semua | VCC | 3.3V |
| Semua | GND | GND |

> Gateway menggunakan ESP32 DevKit (bukan C3) karena lebih banyak pin dan lebih stabil untuk koneksi WiFi + LoRa simultan.
