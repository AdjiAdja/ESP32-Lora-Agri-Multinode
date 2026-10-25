#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <ThingSpeak.h>

// ====== PIN CONFIG (sesuaikan) ======
#define LORA_SS    5
#define LORA_RST   14
#define LORA_DIO0  2

// ====== KONFIGURASI WIFI & THINGSPEAK ======
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

unsigned long THINGSPEAK_CHANNEL_ID = 0000000;      // Ganti!
const char* THINGSPEAK_API_KEY     = "YOUR_WRITE_API_KEY"; // Ganti!

// ====== KONFIGURASI LORA ======
#define LORA_FREQ  433E6

WiFiClient client;

void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("Gateway Starting...");

    // ====== KONEKSI WIFI ======
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected. IP: " + WiFi.localIP().toString());

    // ====== INIT THINGSPEAK ======
    ThingSpeak.begin(client);

    // ====== INIT LORA ======
    LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
    if (!LoRa.begin(LORA_FREQ)) {
        Serial.println("LoRa init FAILED!");
        while (1) delay(1000);
    }

    // Parameter radio harus SAMA dengan node
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);

    Serial.println("Gateway ready. Waiting for packets...");
}

void loop() {
    // ====== CEK PAKET LORA MASUK ======
    int packetSize = LoRa.parsePacket();
    if (packetSize == 0) {
        delay(10);
        return;
    }

    // Baca payload
    String payload = "";
    while (LoRa.available()) {
        payload += (char)LoRa.read();
    }

    int rssi = LoRa.packetRssi();
    Serial.println("---");
    Serial.print("Received: ");
    Serial.println(payload);
    Serial.print("RSSI: ");
    Serial.println(rssi);

    // ====== PARSE DATA ======
    // Format: NODE-01|T:28.5|H:65.2|S:45.0
    float temp = 0, hum = 0, soil = 0;

    int tIdx = payload.indexOf("T:");
    if (tIdx != -1) temp = payload.substring(tIdx + 2, payload.indexOf("|", tIdx)).toFloat();

    int hIdx = payload.indexOf("H:");
    if (hIdx != -1) hum = payload.substring(hIdx + 2, payload.indexOf("|", hIdx)).toFloat();

    int sIdx = payload.indexOf("S:");
    if (sIdx != -1) soil = payload.substring(sIdx + 2).toFloat();

    Serial.printf("Parsed → Temp: %.1f, Hum: %.1f, Soil: %.1f\n", temp, hum, soil);

    // ====== KIRIM KE THINGSPEAK ======
    ThingSpeak.setField(1, temp);   // Field 1: Suhu
    ThingSpeak.setField(2, hum);    // Field 2: Kelembaban udara
    ThingSpeak.setField(3, soil);   // Field 3: Kelembaban tanah
    ThingSpeak.setField(4, rssi);   // Field 4: RSSI (opsional)

    int response = ThingSpeak.writeFields(THINGSPEAK_CHANNEL_ID, THINGSPEAK_API_KEY);
    if (response == 200) {
        Serial.println("ThingSpeak update OK");
    } else {
        Serial.print("ThingSpeak error: ");
        Serial.println(response);
    }
}
