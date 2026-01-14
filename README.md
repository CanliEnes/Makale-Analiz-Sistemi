# Makale-Analiz-Sistemi
Akademik atıf ağlarını analiz eden yüksek performanslı C++/Qt uygulaması. İteratif K-Core ayrıştırma, Brandes tabanlı Betweenness Centrality (Ara Merkeziyet), H-Index hesaplamaları ve çakışma önleyici spiral yerleşim algoritmasıyla dinamik graf görselleştirme içerir.

Bu proje, bilimsel makaleler arasındaki karmaşık atıf ilişkilerini modellemek, akademik etki metriklerini hesaplamak ve bu verileri interaktif bir şekilde görselleştirmek amacıyla geliştirilmiş bir masaüstü uygulamasıdır. 

Uygulama, büyük veri setlerini (OpenAlex) işleyerek makalelerin ağ içindeki stratejik konumlarını ve dahil oldukları akademik toplulukları tespit eder.

## 🚀 Öne Çıkan Özellikler

*   **Saf C++ Veri İşleme:** Herhangi bir harici JSON kütüphanesi kullanmadan, bellek verimliliği için optimize edilmiş, anahtar kelime tabanlı hızlı veri ayrıştırıcı (Manual Parser).
*   **Gelişmiş Analiz Metrikleri:**
    *   **Betweenness Centrality (Ara Merkeziyet):** Brandes algoritması kullanılarak grafın "köprü" makalelerinin tespiti ve stratejik düğümlerin boyutlandırılması.
    *   **K-Core Decomposition:** Ağın en yoğun çekirdek yapısını belirleyen, komşuluk derecelerini anlık güncelleyen iteratif budama (pruning) algoritması.
    *   **H-Index & H-Median:** Makale bazlı akademik başarı ölçümü ve çekirdek grup performans analizi.
*   **Dinamik Görselleştirme:**
    *   **Spiral Yerleşim (Collision Detection):** Yeni düğümler eklenirken üst üste binmeyi engelleyen akıllı çakışma kontrolü ve spiral yayılım mantığı.
    *   **Modüler Görünüm:** Analiz modunda yönlü okların otomatik olarak yönsüz (undirected) kenarlara dönüşümü ve soluklaştırma efektleri.
    *   **Etkileşimli Arayüz:** Düğüm üzerine gelindiğinde anlık bilgi kartı (Tooltip) gösterimi ve tıklama ile grafın derinlemesine genişletilmesi.

## 🛠 Kullanılan Teknolojiler

*   **Dil:** C++ (Saf STL kullanımı: `std::map`, `std::set`, `std::queue`, `std::stack`, `std::stringstream`)
*   **Arayüz ve Grafik:** Qt Framework (QGraphicsView, QGraphicsScene, GUI)
*   **Veri Seti:** OpenAlex Akademik Veri Tabanı

## 📂 Proje Yapısı

*   `MainWindow`: Uygulama akışı, buton yönetimi ve ana analiz algoritmaları.
*   `DugumItem`: Grafiksel düğüm (node) tasarımı, etiket yönetimi ve görsel efektler.
*   `okluCizgiCiz`: Trigonometrik hesaplamalar ile vektörel ok ve çizgi motoru.
*   `veriyiYukleSafCPP`: Blok bazlı yüksek hızlı veri ayrıştırma ünitesi.

## 📸 Ekran Görüntüleri

| Genel Görünüm | H-Core Analizi | K-Core Analizi |
| :---: | :---: | :---: |
| ![Genel](img/general_view.png) | ![H-Core](img/hcore_view.png) | ![K-Core](img/kcore_view.png) |

## ⚙️ Kurulum ve Çalıştırma

1.  **Qt Creator** IDE'sini açın.
2.  `MakaleGrafAnaliz.pro` dosyasını projeye dahil edin.
3.  `MainWindow.cpp` dosyası içerisindeki `data.json` dosya yolunu kendi bilgisayarınıza göre güncelleyin:
    ```cpp
    veriyiYukleSafCPP("C:/Belgelerim/MakaleProjesi/data.json");
    ```
4.  Projeyi **Build (Derle)** edin ve çalıştırın.

## 📊 Algoritmik Detaylar

### K-Core Algoritması
Sadece basit derece sayısına bakmak yerine, iteratif bir döngü kullanarak derecesi $k$ değerinden küçük olan düğümleri siler. Bir düğüm silindiğinde ona bağlı komşuların derecesi de düşürüldüğü için, bu işlem stabil bir "çekirdek" grup kalana kadar tekrarlanır.

### Betweenness Centrality
Ağ üzerindeki her düğüm çifti arasındaki tüm en kısa yolları (BFS) hesaplar. Brandes algoritması sayesinde $O(VE)$ karmaşıklığında çalışarak, grafın merkeziyetini (köprü düğümlerini) doğru bir şekilde tespit eder.

## 👥 Geliştiriciler
*   Enes Canlı
