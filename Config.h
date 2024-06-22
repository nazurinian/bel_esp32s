
#ifndef CONFIG_H
#define CONFIG_H

#define BUTTON_1_PIN 14        // Tombol 1 (Pin 14) (WiFi Manager)
#define BUTTON_2_PIN 27        // Tombol 2 (Pin 27) (Stop Audio)
#define BUTTON_3_PIN 26        // -
#define LED_PIN 2              // Lampu LED (Pin 2, default untuk ESP32)
#define LED_1_GREEN_PIN 12     // LED Hijau 1 (Pin 12)
#define LED_2_RED_PIN 13       // LED Merah 2 (Pin 13)
#define LED_3_COLOR_PIN 25     // LED Almuhim 3 (Pin 25) Untuk status putar
#define DFPLAYER_VOLUME_PIN 32 // Pin untuk mengendalikan volume DFPlayer (Pin 32)
#define DFPLAYER_BUSY_PIN 23   // Pin untuk mendeteksi keadaan sibuk DFPlayer (Pin 23)

#define API_KEY "AIzaSyBj8BvP9fbpHe82STphSUZqF1ZY1TirRXY"
#define DATABASE_URL "https://bel-sekolah-2-default-rtdb.asia-southeast1.firebasedatabase.app"
#define DATABASE_SECRET_KEY "G3cm9Lall4qO6PuxtaDdMPb5SNtpuvytzJGI90Va"
#define FIREBASE_USE_PSRAM

#define AP_SSID "BSIB-AP"
#define AP_PASSWORD "config123"

#define SENIN_SAMPAI_KAMIS "/jadwal/senin-kamis/"
#define JUMAT "/jadwal/jumat/"
#define JAM_MASUK "/jam/"
#define MENIT_MASUK "/menit/"
#define STATUS_MASUK "/aktif/"

#define PUTAR_MANUAL "/putar-manual"
#define STATUS_PUTAR "/putar"
#define PILIHAN_PUTAR "/choice"

#define GMT_OFFSET_SEC (8 * 3600)
#define JUMLAH_JADWAL 16

#endif
