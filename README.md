# Dokumentasi Proyek Monitor Suhu dan Kelembaban dengan DHT11 dan OLED

## 📋 Deskripsi Proyek
This project is a temperature and humidity monitoring system using a DHT11 sensor that displays data on an SSD1306 OLED display. The system displays humidity, temperature, and heat index in real time.

---

## 🔌 Komponen yang Dibutuhkan

| Komponen | Jumlah | Keterangan |
|----------|--------|------------|
| ESP8266/NodeMCU | 1 | Mikrokontroler utama |
| Sensor DHT11 | 1 | Sensor suhu dan kelembaban |
| OLED Display SSD1306 (128x64) | 1 | Layar tampilan I2C |
| Kabel Jumper | Secukupnya | Untuk koneksi |
| Breadboard | 1 | Opsional untuk prototyping |

---

## 🔧 Diagram Pemasangan Pin

### **Sensor DHT11 ke ESP8266/NodeMCU**
```
DHT11          NodeMCU/ESP8266
─────────────────────────────
VCC    ───────→  3.3V atau 5V
GND    ───────→  GND
DATA   ───────→  D5 (GPIO 14)
```

### **OLED Display SSD1306 ke ESP8266/NodeMCU**
```
OLED SSD1306   NodeMCU/ESP8266
─────────────────────────────
VCC    ───────→  3.3V
GND    ───────→  GND
SDA    ───────→  D2 (GPIO 4)
SCL    ───────→  D1 (GPIO 5)
```

### **Diagram Visual**
```
        ┌─────────────────┐
        │   NodeMCU/      │
        │   ESP8266       │
        ├─────────────────┤
3.3V ───┤ 3V3         D5  ├─── DHT11 (DATA)
GND  ───┤ GND         D2  ├─── OLED (SDA)
        │             D1  ├─── OLED (SCL)
        └─────────────────┘
```

---

## 📚 Library yang Dibutuhkan

Install library berikut melalui Arduino IDE (Sketch → Include Library → Manage Libraries):

1. **DHT sensor library** by Adafruit
2. **Adafruit GFX Library**
3. **Adafruit SSD1306**
4. **Adafruit Unified Sensor**

---

## 💻 Penjelasan Kode

### **1. Include Library dan Definisi Pin**
```cpp
#include "DHT.h"
#define DHTPIN 14      // PIN D5 pada NodeMCU
#define DHTTYPE DHT11  // Tipe sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
```
**Penjelasan:**
- `DHT.h`: Library untuk sensor DHT
- `DHTPIN 14`: Pin GPIO 14 (D5) untuk data DHT11
- `DHTTYPE DHT11`: Menentukan tipe sensor yang digunakan

### **2. Konfigurasi OLED**
```cpp
#define SCREEN_WIDTH 128  // Lebar layar OLED (pixel)
#define SCREEN_HEIGHT 64  // Tinggi layar OLED (pixel)
#define OLED_RESET -1     // Pin reset (-1 jika sharing dengan reset Arduino)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
```
**Penjelasan:**
- Mendefinisikan ukuran layar OLED 128x64 pixel
- Menggunakan komunikasi I2C (Wire)

### **3. Setup - Inisialisasi**
```cpp
void setup() {
  Serial.begin(9600);           // Memulai komunikasi serial
  dht.begin();                  // Inisialisasi sensor DHT
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // Mulai OLED di alamat I2C 0x3C
  display.clearDisplay();        // Bersihkan layar
  display.display();             // Tampilkan layar kosong
}
```

### **4. Loop - Pembacaan Data**
```cpp
float h = dht.readHumidity();        // Baca kelembaban (%)
float t = dht.readTemperature();     // Baca suhu (Celsius)
float f = dht.readTemperature(true); // Baca suhu (Fahrenheit)
```
**Penjelasan:**
- Membaca data dari sensor DHT11
- Proses pembacaan membutuhkan ~250ms

### **5. Validasi Data**
```cpp
if (isnan(h) || isnan(t) || isnan(f)) {
  Serial.println(F("Failed to read from DHT sensor!"));
  return;
}
```
**Penjelasan:**
- Mengecek apakah pembacaan gagal
- `isnan()`: Mengecek nilai NaN (Not a Number)

### **6. Heat Index (Indeks Panas)**
```cpp
float hif = dht.computeHeatIndex(f, h);      // Heat index (Fahrenheit)
float hic = dht.computeHeatIndex(t, h, false); // Heat index (Celsius)
```
**Penjelasan:**
- **Heat Index**: Suhu yang dirasakan tubuh dengan memperhitungkan kelembaban
- Berguna untuk mengetahui tingkat kenyamanan

### **7. Tampilan OLED**
```cpp
display.clearDisplay();          // Hapus tampilan sebelumnya
display.setTextColor(WHITE);     // Warna teks putih
display.setTextSize(2);          // Ukuran teks 2x

// Tampilkan Kelembaban
display.setCursor(0, 5);         // Posisi X=0, Y=5
display.println("H: ");
display.setCursor(25, 5);
display.println(h);              // Nilai kelembaban
display.setCursor(90, 5);
display.println("%");

display.display();               // Update tampilan OLED
delay(1000);                     // Tunggu 1 detik
```

---

## 📊 Format Tampilan OLED

```
┌──────────────────────┐
│ H:  65.00      %     │  ← Kelembaban (Humidity)
│                      │
│ T:  28.50      C     │  ← Suhu (Temperature)
│                      │
│ HI  29.30      C     │  ← Indeks Panas (Heat Index)
└──────────────────────┘
```

---

## 🚀 Cara Menggunakan

1. **Pasang semua komponen** sesuai diagram pin di atas
2. **Install library** yang diperlukan di Arduino IDE
3. **Upload kode** ke ESP8266/NodeMCU
4. **Buka Serial Monitor** (baud rate: 9600) untuk melihat data
5. **Lihat tampilan OLED** untuk monitoring real-time

---

## 🔍 Troubleshooting

| Masalah | Solusi |
|---------|--------|
| OLED tidak menyala | Cek koneksi SDA/SCL, pastikan alamat I2C 0x3C benar |
| DHT sensor gagal | Cek koneksi pin data, tunggu 2 detik setelah power on |
| Data NaN terus-menerus | Periksa koneksi DHT11, coba ganti sensor |
| Serial Monitor kosong | Pastikan baud rate 9600, tekan tombol RST |

---

## 📝 Catatan Penting

- **DHT11** memiliki akurasi: ±2°C untuk suhu, ±5% untuk kelembaban
- **Pembacaan sensor** sebaiknya dilakukan minimal setiap 2 detik
- **Heat Index** hanya akurat pada suhu >27°C dan kelembaban >40%
- **Alamat I2C OLED** umumnya 0x3C atau 0x3D (cek dengan I2C scanner jika perlu)

---
