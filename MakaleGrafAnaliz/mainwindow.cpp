#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DugumItem.h"
#include <fstream>
#include <queue>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <cmath>
#include <QGraphicsLineItem>
#include <QGraphicsPolygonItem>
#include <QDebug>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    if(ui->graphicsView) { ui->graphicsView->setScene(scene); }

    // Veriyi yükle
    veriyiYukleSafCPP("C:/Users/canli/Documents/MakaleAnalizSistemi/data.json");
    istatistikleriGoster();

    connect(ui->btnHesapla, &QPushButton::clicked, this, [this]() {

        hedefId = ui->txtMakaleId->text().trimmed();
        if(!hedefId.isEmpty()) {
            scene->clear();
            cizilenDugumler.clear();
            harfSayaci = 0;
            dugumeTiklandi(hedefId);
        }
    });

    connect(ui->btnGenelGoster, &QPushButton::clicked, this, [this]() {
        genelGrafiCiz(50);
    });

    // ANALİZ YAP BUTONU - TEMİZLENMİŞ HALİ
    connect(ui->btnAnalizYap, &QPushButton::clicked, this, [this]() {
        int k = 2; // Veya ui->txtKValue->text().toInt();
        betweennessAnaliziYap();
        kCoreAnaliziYap(k);
        analizGorunumuGuncelle(); // Tüm çizim işlemini bu fonksiyon yapacak
    });
}

void MainWindow::dugumeTiklandi(QString id) {
    if (!makaleGrafi.count(id)) return;
    hedefId = id;
    Makale &hedef = makaleGrafi[id];

    // 1. Atıf yapan makalelerin atıf sayılarını topla
    vector<int> atifSayilari;
    for (const QString &v : hedef.citedBy) {
        atifSayilari.push_back((int)makaleGrafi[v].citedBy.size());
    }

    // 2. Büyükten küçüğe sırala (Saf C++)
    sort(atifSayilari.begin(), atifSayilari.end(), greater<int>());

    // 3. H-Index Hesapla
    int hIndex = 0;
    for (int i = 0; i < (int)atifSayilari.size(); ++i) {
        if (atifSayilari[i] >= i + 1) {
            hIndex = i + 1;
        } else {
            break;
        }
    }

    // 4. H-Median Hesapla (H-Core içindeki atıf sayılarının ortancası)
    double hMedian = 0;
    if (hIndex > 0) {
        // H-Core kümesi: atifSayilari içindeki ilk 'hIndex' kadar eleman
        if (hIndex % 2 == 1) {
            // Tek sayı ise tam ortadaki
            hMedian = atifSayilari[hIndex / 2];
        } else {
            // Çift sayı ise ortadaki ikisinin ortalaması
            hMedian = (atifSayilari[(hIndex / 2) - 1] + atifSayilari[hIndex / 2]) / 2.0;
        }
    }

    // 5. Arayüz etiketlerini güncelle
    ui->lblHIndex->setText("H-Index: " + QString::number(hIndex));
    ui->lblHMedian->setText("H-Median: " + QString::number(hMedian, 'f', 1));

    // 6. Grafı genişletme mantığı (mevcut kodunuz)
    vector<QString> hCoreIds;
    // Atıf yapan makaleleri atıf sayısına göre sıralayıp h-core id'lerini belirle
    struct Aday { QString id; int atif; };
    vector<Aday> adaylar;
    for (const QString &v : hedef.citedBy) adaylar.push_back({v, (int)makaleGrafi[v].citedBy.size()});
    sort(adaylar.begin(), adaylar.end(), [](const Aday &a, const Aday &b){ return a.atif > b.atif; });

    for (int i = 0; i < hIndex; ++i) {
        hCoreIds.push_back(adaylar[i].id);
    }

    QColor r = cizilenDugumler.empty() ? QColor(255, 255, 180) : QColor(180, 255, 180);
    genisletVeCiz(id, hCoreIds, r);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::veriyiYukleSafCPP(string dosyaYolu) {
    ifstream dosya(dosyaYolu);
    if (!dosya.is_open()) {
        qDebug() << "DOSYA ACILAMADI!";
        return;
    }

    stringstream buffer;
    buffer << dosya.rdbuf();
    string icerik = buffer.str();
    dosya.close();

    makaleGrafi.clear();
    int bulunanIdSayisi = 0;

    size_t pos = 0;
    while (true) {
        // Her seferinde bir sonraki "id": metnini ara
        size_t idKeyPos = icerik.find("\"id\":", pos);
        if (idKeyPos == string::npos) break;

        // Bloğu oluştur (Bir sonraki ID'ye kadar olan her şey)
        size_t nextIdKeyPos = icerik.find("\"id\":", idKeyPos + 5);
        string blok = (nextIdKeyPos != string::npos) ?
                          icerik.substr(idKeyPos, nextIdKeyPos - idKeyPos) :
                          icerik.substr(idKeyPos);

        Makale m;

        // 1. ID AYRIŞTIR (Hatasız)
        size_t s = blok.find("\"", 5) + 1;
        size_t e = blok.find("\"", s);
        if (s != string::npos && e != string::npos) {
            string rawIdStr = blok.substr(s, e - s);
            m.id = QString::fromStdString(rawIdStr);

            // Numeric ID'yi sadece rakamları alarak ayıkla (stoll güvenliği)
            size_t wPos = rawIdStr.find("/W");
            if (wPos != string::npos) {
                string sadeceRakam = "";
                for(char c : rawIdStr.substr(wPos + 2)) {
                    if(isdigit(c)) sadeceRakam += c; else break;
                }
                if(!sadeceRakam.empty()) m.numericId = stoll(sadeceRakam);
            }
        }

        // 2. YIL AYRIŞTIR
        size_t yearPos = blok.find("\"year\":");
        if (yearPos != string::npos) {
            size_t startY = blok.find_first_of("0123456789", yearPos + 6);
            if (startY != string::npos) {
                size_t endY = blok.find_first_not_of("0123456789", startY);
                m.year = stoi(blok.substr(startY, endY - startY));
            }
        }

        // 3. BAŞLIK AYRIŞTIR
        size_t titlePos = blok.find("\"title\":");
        if (titlePos != string::npos) {
            size_t ts = blok.find("\"", titlePos + 8) + 1;
            size_t te = blok.find("\"", ts);
            m.title = QString::fromStdString(blok.substr(ts, te - ts));
        }

        // 4. YAZARLARI AYRIŞTIR
        size_t authPos = blok.find("\"authors\":");
        if (authPos != string::npos) {
            size_t startB = blok.find("[", authPos);
            size_t endB = blok.find("]", startB);
            if (startB != string::npos && endB != string::npos) {
                string yazarStr = blok.substr(startB + 1, endB - startB - 1);
                size_t subP = 0;
                while((subP = yazarStr.find("\"", subP)) != string::npos) {
                    size_t sA = subP + 1;
                    size_t eA = yazarStr.find("\"", sA);
                    if (eA == string::npos) break;
                    m.authors << QString::fromStdString(yazarStr.substr(sA, eA - sA));
                    subP = eA + 1;
                }
            }
        }

        // 5. REFERANSLARI AYRIŞTIR
        size_t refPos = blok.find("\"referenced_works\":");
        if (refPos != string::npos) {
            size_t startR = blok.find("[", refPos);
            size_t endR = blok.find("]", startR);
            if (startR != string::npos && endR != string::npos) {
                string refs = blok.substr(startR + 1, endR - startR - 1);
                size_t subR = 0;
                while((subR = refs.find("\"", subR)) != string::npos) {
                    size_t sR = subR + 1;
                    size_t eR = refs.find("\"", sR);
                    if (eR == string::npos) break;
                    m.references << QString::fromStdString(refs.substr(sR, eR - sR));
                    subR = eR + 1;
                }
            }
        }

        // Map'e ekle
        if (!m.id.isEmpty()) {
            makaleGrafi[m.id] = m;
            bulunanIdSayisi++;
        }

        pos = idKeyPos + 10; // Bir sonraki "id": metnine atla
        if (nextIdKeyPos == string::npos) break; // Sonuncuydu
    }

    // CitedBy bağlama
    for (auto it = makaleGrafi.begin(); it != makaleGrafi.end(); ++it) {
        for (const QString& refId : it->second.references) {
            if (makaleGrafi.count(refId)) {
                makaleGrafi[refId].citedBy << it->first;
            }
        }
    }

    qDebug() << "Bulunan Makale Sayisi (Döngü):" << bulunanIdSayisi;
    qDebug() << "Haritadaki Makale Sayisi (Map size):" << makaleGrafi.size();
}

void MainWindow::istatistikleriGoster() {
    // Toplam makale sayısı haritanın (map) büyüklüğüdür
    int toplamMakale = (int)makaleGrafi.size();
    long toplamReferans = 0;
    int maxAtif = -1;
    QString enCokAtifAlanId = "Yok";

    for(auto const& [id, m] : makaleGrafi) {
        // Graf genelindeki toplam kenar (referans) sayısı
        toplamReferans += m.references.size();

        // Atıf istatistiği
        int atifSayisi = (int)m.citedBy.size();
        if(atifSayisi > maxAtif) {
            maxAtif = atifSayisi;
            enCokAtifAlanId = id;
        }
    }

    ui->lblToplamMakale->setText("Toplam Makale: " + QString::number(makaleGrafi.size()));
    ui->lblToplamRef->setText("Toplam Referans: " + QString::number(toplamReferans));
    ui->lblEnCokAtif->setText("En Cok Atif (" + QString::number(maxAtif) + "): " + enCokAtifAlanId.split("/").last());
}

void MainWindow::genelGrafiCiz(int limit) {
    scene->clear();
    cizilenDugumler.clear();

    vector<Makale*> sirali;
    for(auto &p : makaleGrafi) sirali.push_back(&p.second);
    sort(sirali.begin(), sirali.end(), [](Makale* a, Makale* b){ return a->numericId < b->numericId; });

    int adet = min((int)sirali.size(), limit);
    for (int i = 0; i < adet; ++i) {
        DugumItem *item = new DugumItem(*sirali[i]);
        item->setPos((i % 5) * 200, (i / 5) * 200);
        item->etiketGuncelle(false);
        scene->addItem(item);
        cizilenDugumler[sirali[i]->id] = item;
    }

    // 1. SİYAH OKLAR (Referanslar: Veren -> Verilen)
    for (auto const& [id, item] : cizilenDugumler) {
        for (const QString &refId : item->makaleBilgisi.references) {
            if (cizilenDugumler.count(refId)) {
                // Yön: Kaynak Düğümden Referans Verilen Düğüme
                okluCizgiCiz(item->sceneBoundingRect().center(),
                             cizilenDugumler[refId]->sceneBoundingRect().center(),
                             Qt::black, 5, true);
            }
        }
    }

    // 2. YEŞİL OKLAR (Artan ID: Küçük ID -> Büyük ID)
    for (int i = 0; i < adet - 1; ++i) {
        QString id1 = sirali[i]->id;
        QString id2 = sirali[i+1]->id;
        if (cizilenDugumler.count(id1) && cizilenDugumler.count(id2)) {
            // Yön: i'den i+1'e (ID sırasına göre)
            okluCizgiCiz(cizilenDugumler[id1]->sceneBoundingRect().center(),
                         cizilenDugumler[id2]->sceneBoundingRect().center(),
                         Qt::green, 2, true);
        }
    }
}

void MainWindow::genisletVeCiz(QString merkezId, vector<QString> komsular, QColor renk) {
    // 1. Merkez düğüm yoksa önce onu oluştur
    if (!cizilenDugumler.count(merkezId)) {
        DugumItem *m = new DugumItem(makaleGrafi[merkezId]);
        m->setPos(0, 0);
        m->farkliBoyat(QColor(255, 100, 100)); // Merkez düğüm rengi
        m->etiketGuncelle(true, 0);
        scene->addItem(m);
        cizilenDugumler[merkezId] = m;
        harfSayaci = 1;
    }

    QPointF merkezPos = cizilenDugumler[merkezId]->pos();
    const double MIN_MESAFE = 180.0; // Düğümler arası minimum güvenli mesafe

    for (size_t i = 0; i < komsular.size(); ++i) {
        QString kId = komsular[i];

        // Eğer düğüm zaten çizilmişse yerini değiştirme, sadece devam et
        if (cizilenDugumler.count(kId)) continue;

        double r = 250.0; // Başlangıç yarıçapı
        double baslangicAcisi = (2 * M_PI * i) / komsular.size();
        bool uygunYerBulundu = false;
        QPointF adayPos;

        // 2. Çakışma Kontrolü Döngüsü (Saf C++ Matematik)
        while (!uygunYerBulundu) {
            adayPos.setX(merkezPos.x() + r * cos(baslangicAcisi));
            adayPos.setY(merkezPos.y() + r * sin(baslangicAcisi));

            uygunYerBulundu = true;
            // Mevcut tüm çizilmiş düğümlerle mesafeyi kontrol et
            for (auto const& [id, item] : cizilenDugumler) {
                QPointF mevcutPos = item->pos();
                double dx = adayPos.x() - mevcutPos.x();
                double dy = adayPos.y() - mevcutPos.y();
                double mesafe = sqrt(dx*dx + dy*dy);

                if (mesafe < MIN_MESAFE) {
                    uygunYerBulundu = false;
                    r += 50.0; // Mesafe yetersizse dışa doğru it
                    baslangicAcisi += 0.2; // Hafifçe döndür (spiral etkisi)
                    break;
                }
            }
        }

        // 3. Uygun yer bulundu, düğümü ekle
        DugumItem *k = new DugumItem(makaleGrafi[kId]);
        k->setPos(adayPos);
        k->farkliBoyat(renk);
        k->etiketGuncelle(true, harfSayaci++);
        scene->addItem(k);
        cizilenDugumler[kId] = k;
    }

    // 4. Bağlantıları (Okları) Güncelle
    // Mevcut okları temizle
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* it : items) {
        if (it->type() == QGraphicsLineItem::Type || it->type() == QGraphicsPolygonItem::Type) {
            scene->removeItem(it);
            delete it;
        }
    }

    // Tüm çizili düğümler arasındaki referans ilişkilerini tekrar kur
    for (auto const& [id1, item1] : cizilenDugumler) {
        for (const QString &refId : item1->makaleBilgisi.references) {
            if (cizilenDugumler.count(refId)) {
                okluCizgiCiz(item1->sceneBoundingRect().center(),
                             cizilenDugumler[refId]->sceneBoundingRect().center(),
                             Qt::black, 2, true);
            }
        }
    }
}

void MainWindow::betweennessAnaliziYap() {
    // Skorları sıfırla
    map<QString, double> scores;
    for (auto const& [id, m] : makaleGrafi) scores[id] = 0;

    // Sadece şu an çizili olan düğümler arasında analiz yap (veya tüm graf üzerinde)
    vector<QString> DugumListesi;
    for (auto const& [id, item] : cizilenDugumler) DugumListesi.push_back(id);

    for (const QString &baslangic : DugumListesi) {
        // Her düğüm için BFS başlat
        map<QString, vector<QString>> ebeveynler;
        map<QString, int> mesafe;
        map<QString, int> yolSayisi;
        for (const QString &id : DugumListesi) mesafe[id] = -1;

        queue<QString> q;
        q.push(baslangic);
        mesafe[baslangic] = 0;
        yolSayisi[baslangic] = 1;

        vector<QString> siraliDugumler;

        while (!q.empty()) {
            QString v = q.front();
            q.pop();
            siraliDugumler.push_back(v);

            // Komşuları bul (Yönsüz kabul ederek)
            set<QString> komsular;
            for(const auto& ref : makaleGrafi[v].references) if(cizilenDugumler.count(ref)) komsular.insert(ref);
            for(const auto& cited : makaleGrafi[v].citedBy) if(cizilenDugumler.count(cited)) komsular.insert(cited);

            for (const QString &w : komsular) {
                if (mesafe[w] < 0) {
                    mesafe[w] = mesafe[v] + 1;
                    q.push(w);
                }
                if (mesafe[w] == mesafe[v] + 1) {
                    yolSayisi[w] += yolSayisi[v];
                    ebeveynler[w].push_back(v);
                }
            }
        }

        map<QString, double> bagimlilik;
        for (const QString &id : DugumListesi) bagimlilik[id] = 0;

        // Sirali listeyi tersten işle (Brandes algoritması mantığı)
        for (int i = siraliDugumler.size() - 1; i >= 0; --i) {
            QString w = siraliDugumler[i];
            for (const QString &v : ebeveynler[w]) {
                bagimlilik[v] += ((double)yolSayisi[v] / yolSayisi[w]) * (1.0 + bagimlilik[w]);
            }
            if (w != baslangic) scores[w] += bagimlilik[w];
        }
    }

    // Maksimum skoru bul ve düğümleri boyutlandır
    double maxScore = 0;
    for (auto const& [id, s] : scores) if (s > maxScore) maxScore = s;

    for (auto const& [id, item] : cizilenDugumler) {
        double oran = (maxScore > 0) ? (scores[id] / maxScore) : 0;
        item->setScale(1.0 + oran); // Merkezi olan düğümler daha büyük görünür
    }
}

// 2. ITERATİF K-CORE ALGORİTMASI (Saf C++)
void MainWindow::kCoreAnaliziYap(int k) {
    map<QString, set<QString>> undirectedAdj;
    map<QString, int> currentDegrees;
    set<QString> aktifDugumler;

    // Sadece ekrandaki düğümlerle yönsüz komşuluk kur
    for (auto const& [id, item] : cizilenDugumler) {
        aktifDugumler.insert(id);
        for (const QString &ref : item->makaleBilgisi.references) {
            if (cizilenDugumler.count(ref)) {
                if (ref == id) continue;
                undirectedAdj[id].insert(ref);
                undirectedAdj[ref].insert(id);
            }
        }
    }

    for (const QString &id : aktifDugumler) {
        currentDegrees[id] = undirectedAdj[id].size();
    }

    // İteratif Silme İşlemi (K-Core'un kalbi burasıdır)
    bool degisimVar = true;
    while (degisimVar) {
        degisimVar = false;
        vector<QString> silinecekler;

        for (const QString &id : aktifDugumler) {
            if (currentDegrees[id] < k) {
                silinecekler.push_back(id);
            }
        }

        for (const QString &id : silinecekler) {
            for (const QString &komsu : undirectedAdj[id]) {
                currentDegrees[komsu]--;
            }
            aktifDugumler.erase(id);
            degisimVar = true;
        }
    }

    // Görselleştirme: K-Core içindekileri Şekil 5'teki mor renge boya
    for (auto const& [id, item] : cizilenDugumler) {
        if (aktifDugumler.count(id)) {
            item->farkliBoyat(QColor(216, 191, 216)); // Mor/Lila renk
            item->setOpacity(1.0);
        } else {
            item->farkliBoyat(Qt::white);
            item->setOpacity(0.3); // Core dışı sönük
        }
    }
}

void MainWindow::analizGorunumuGuncelle() {
    // Eski çizgileri temizle
    QList<QGraphicsItem*> items = scene->items();
    for (QGraphicsItem* it : items) {
        if (it->type() == QGraphicsLineItem::Type || it->type() == QGraphicsPolygonItem::Type) {
            scene->removeItem(it);
            delete it;
        }
    }

    // Çizgileri Opaklığa Göre Çiz
    for (auto const& [id1, item1] : cizilenDugumler) {
        for (const QString &refId : item1->makaleBilgisi.references) {
            if (cizilenDugumler.count(refId)) {
                DugumItem* item2 = cizilenDugumler[refId];

                // Eğer her iki düğüm de k-core içindeyse (Opaklık 1.0 ise)
                bool hemCore1 = (item1->opacity() > 0.8);
                bool hemCore2 = (item2->opacity() > 0.8);

                if (hemCore1 && hemCore2) {
                    // Çekirdek içi: Parlak Mavi
                    okluCizgiCiz(item1->sceneBoundingRect().center(),
                                 item2->sceneBoundingRect().center(),
                                 QColor(0, 0, 255), 1, false);
                } else {
                    // Çekirdek dışı: Çok Soluk Gri/Mavi
                    // Sondaki 30 değeri şeffaflığı (Alpha) temsil eder
                    okluCizgiCiz(item1->sceneBoundingRect().center(),
                                 item2->sceneBoundingRect().center(),
                                 QColor(150, 150, 150, 30), 1, false);
                }
            }
        }
    }
}

void MainWindow::okluCizgiCiz(QPointF s, QPointF e, QColor renk, int z, bool okUcuOlsun) {
    QLineF hat(s, e);
    if (hat.length() < 35) return;

    qreal oran = 35.0 / hat.length();
    QPointF yeniBitis = e - QPointF(hat.dx() * oran, hat.dy() * oran);

    // Çizgi kalınlığını renge (şeffaflığa) göre ayarla
    int kalinlik = (renk.alpha() < 100) ? 1 : 2;

    QGraphicsLineItem *line = scene->addLine(QLineF(s, yeniBitis), QPen(renk, kalinlik));
    line->setZValue(z); // Çizgi z değeri (Genelde 1 veya 2)

    if (okUcuOlsun) {
        double aci = atan2(hat.dy(), hat.dx());
        qreal okBoyu = 12;
        qreal okAcisi = M_PI / 6;

        QPointF p1 = yeniBitis - QPointF(cos(aci - okAcisi) * okBoyu, sin(aci - okAcisi) * okBoyu);
        QPointF p2 = yeniBitis - QPointF(cos(aci + okAcisi) * okBoyu, sin(aci + okAcisi) * okBoyu);

        QPolygonF kafa;
        kafa << yeniBitis << p1 << p2;
        QGraphicsPolygonItem *arrow = scene->addPolygon(kafa, QPen(renk), QBrush(renk));
        arrow->setZValue(z);
    }
}
