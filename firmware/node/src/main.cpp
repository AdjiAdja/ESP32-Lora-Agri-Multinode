#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <DHT.h>

// ====== PIN CONFIG (sesuaikan dengan wiring) ======
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  2

#define DHTPIN     4
#define DHTTYPE    DHT22

#define SOIL_PIN   3   // Analog input untuk sensor kelembaban tanah

// ====== KONFIGURASI ======
#define NODE_ID    "NODE-01"
#define LORA_FREQ  433E6   // Sesuaikan: 433E6 / 868E6 / 915E6
#define TX_INTERVAL 60000  // Kirim data setiap 60 detik (ms)

DHT dht(DHTPIN, DHTTYPE);
unsigned long lastTx = 0;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Node Sensor Starting...");

    // Inisialisasi sensor
    dht.begin();
    pinMode(SOIL_PIN, INPUT);

    // Inisialisasi LoRa
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("LoRa init FAILED!");
        while (1) delay(1000);
    }

    // Konfigurasi parameter radio (harus SAMA dengan gateway)
    LoRa.setSpreadingFactor(7);        // SF7 = cepat, konsumsi daya rendah
    LoRa.setSignalBandwidth(125E3);    // 125 kHz
    LoRa.setCodingRate4(5);            // CR 4/5
    LoRa.setTxPower(17);               // 17 dBm (maks untuk SX1278)

    Serial.println("LoRa OK. Node ready.");
}

void loop() {
    if (millis() - lastTx < TX_INTERVAL) {
        delay(100);
        return;
    }
    lastTx = millis();

    // ====== BACA SENSOR ======
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();

    // Cek apakah pembacaan DHT valid
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("DHT read failed!");
        return;
    }

    // Baca sensor tanah (ADC 0-4095 → persentase kasar)
    int soilRaw = analogRead(SOIL_PIN);
    float soilPercent = map(soilRaw, 4095, 0, 0, 100); // Asumsi: kering=4095, basah=0
    if (soilPercent < 0) soilPercent = 0;
    if (soilPercent > 100) soilPercent = 100;

    // ====== FORMAT DATA ======
    // Format: NODE-01|T:28.5|H:65.2|S:45.0
    String payload = String(NODE_ID) +
                     "|T:" + String(temperature, 1) +
                     "|H:" + String(humidity, 1) +
                     "|S:" + String(soilPercent, 1);

    Serial.print("Sending: ");
    Serial.println(payload);

    // ====== KIRIM VIA LORA ======
    LoRa.beginPacket();
    LoRa.print(payload);
    LoRa.endPacket();

    Serial.println("Packet sent.");
}
