# Panduan Setup ThingSpeak

## 1. Buat Akun
- Kunjungi https://thingspeak.com
- Daftar akun gratis

## 2. Buat Channel Baru
- Klik **Channels** → **My Channels** → **New Channel**
- Isi:
  - **Name**: `LoRa Agri Monitor`
  - **Field 1**: `Temperature (C)`
  - **Field 2**: `Humidity (%)`
  - **Field 3**: `Soil Moisture (%)`
  - **Field 4**: `RSSI (dBm)`
- Klik **Save Channel**

## 3. Dapatkan Write API Key
- Buka tab **API Keys** di channel kamu
- Copy **Write API Key** (string panjang)
- Ganti `YOUR_WRITE_API_KEY` di `gateway/src/main.cpp`

## 4. Dapatkan Channel ID
- Channel ID terlihat di URL atau di tab **Channel Settings**
- Ganti `THINGSPEAK_CHANNEL_ID` di `gateway/src/main.cpp`

## 5. Visualisasi
- Tab **Private View** menampilkan grafik real-time
- Data bisa di-export ke CSV via **Export recent data**
