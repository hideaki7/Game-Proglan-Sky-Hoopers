# Sky Hoopers 

Sky Hoopers adalah game platformer 2D dinamis dan modern yang terinspirasi dari game klasik *Doodle Jump*. Dibuat sepenuhnya menggunakan **C++** dan library **SFML**, game ini memperkenalkan mekanik unik seperti perubahan arah gravitasi, portal interaktif, dan berbagai *buff* karakter untuk memberikan pengalaman melompat tanpa batas yang lebih seru.

## Fitur Utama

- **Two-Way Responsive Scrolling:** Kamera dan gameplay dapat menyesuaikan secara dinamis, memungkinkan pergerakan secara vertikal maupun horizontal tergantung pada arah gravitasi saat ini.
- **Portal:** Portal yang akan memindahkan pemain sekaligus mengubah arah gravitasi dan pergerakan layar.
- **Power-up & Buff:**
  - 🚀 **Rocket:** Mendorong pemain meluncur ke atas/depan dengan kecepatan tinggi, membuat pemain kebal sementara waktu.
  - 🛡️ **Shield:** Melindungi pemain dari bahaya dan mencegah karakter jatuh.
  - ⭐ **2x Multiplier:** Menggandakan poin skor yang didapatkan dari pergerakan dan lompatan.
- **Sound Effects:** Dilengkapi dengan Musik Latar dan Efek Suara (SFX) yang responsif untuk setiap mendapat buff, portal, dan saat game over.

## Spesifikasi

Untuk melakukan *build* dan menjalankan game ini, kamu perlu menginstal beberapa perangkat lunak berikut (Linux/WSL, Windows, atau macOS):

- Compiler yang mendukung **C++17** (GCC, Clang, atau MSVC)
- **CMake** (versi minimal 3.10)
- **SFML** (Simple and Fast Multimedia Library - versi minimal 2.5)

## Struktur Proyek

Menggunakan struktur _Skeleton Code_ agar lebih rapi.
- `src/` - Berisi semua *source code* C++ dan *header files*.
  - `entities/` - Logika Pemain (Player), Platform, Portal, dan Buff.
  - `state/` - Manajemen *state* game (Menu, Panduan, Gameplay).
- `assets/` - Berbagai aset game termasuk gambar, font, dan file audio.
