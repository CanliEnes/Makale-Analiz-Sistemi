#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <map>
#include <vector>
#include "DugumItem.h"
#include "Makale.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // DugumItem'dan çağrılabilmesi için bu fonksiyonu public veya public slot yapıyoruz
    void dugumeTiklandi(QString hedefId);

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QString hedefId;
    int harfSayaci = 0;

    // Tüm veri kümesini tutan yapı
    map<QString, Makale> makaleGrafi;

    // O an ekranda (sahnede) çizili olan düğümleri takip eden yapı
    // Bu yapı sayesinde genişletme yaparken eski düğümleri koruyabiliyoruz.
    map<QString, DugumItem*> cizilenDugumler;

    // Temel Fonksiyonlar
    void veriyiYukleSafCPP(std::string dosyaYolu); // Saf C++ bildirimi
      // Veriyi belleğe yükler
    void istatistikleriGoster();            // Arayüzdeki genel bilgileri günceller
    void grafiCiz();                        // Başlangıçtaki kısıtlı grafı çizer

    // H-Index ve Genişletme Fonksiyonları
    void hesaplaVeCizHCore(QString hedefId); // H-index hesaplar ve ilk h-core'u çizer

    // Mevcut grafı yeni h-core düğümleriyle büyütür
    void genisletVeCiz(QString merkezId, vector<QString> komsular, QColor renk);

    // mainwindow.h içindeki ilgili satırı bul ve bununla değiştir:
    void okluCizgiCiz(QPointF s, QPointF e, QColor r, int z, bool okUcuOlsun); // 5. parametreyi (bool) buraya ekledik
    void analizGorunumuGuncelle();

    void genelGrafiCiz(int limit = 50);

    // mainwindow.h içindeki private kısmına ekle:
    struct CentralityResult {
        QString id;
        double score;
    };

    void betweennessAnaliziYap();
    void kCoreAnaliziYap(int k); // Genelde k=2 veya 3 kullanılır

    // Public slots kısmına analiz butonu için:
    void on_btnAnalizYap_clicked();
};
#endif // MAINWINDOW_H
