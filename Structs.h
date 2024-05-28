#ifndef STRUCTS_H
#define STRUCTS_H

#include <Arduino.h>

struct JadwalData
{
    bool aktif;
    int jam;
    int menit;
};

struct Schedule
{
    int jam;
    int menit;
    String title;
};

// Struktur untuk menyimpan jam dan menit
struct Waktu
{
    int jam;
    int menit;
};

// Deklarasi struktur untuk elemen waktu
struct CustomTime
{
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    int seconds;
    int weekday;    // Menambah elemen untuk hari
    String dayName; // Menyimpan nama hari
};

#endif
