#include "DugumItem.h"
#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QBrush>
#include <QPen>
#include <QToolTip>
#include <QGraphicsTextItem>
#include <string>
#include <sstream>

DugumItem::DugumItem(Makale m) : makaleBilgisi(m) {
    setRect(0, 0, 70, 70); // Daire boyutu metin sığması için 70 yapıldı
    setBrush(Qt::white);
    setPen(QPen(Qt::black, 2));
    setFlag(QGraphicsItem::ItemIsSelectable);
    setAcceptHoverEvents(true);
    setZValue(10); // Düğümleri en üst katmana al
    etiketGuncelle(false);

    QString info = QString("ID: %1\nBaşlık: %2\nYıl: %3\nAtıf: %4")
                       .arg(m.id).arg(m.title).arg(m.year).arg(m.citedBy.size());
    setToolTip(info);
}

void DugumItem::farkliBoyat(QColor yeniRenk) {
    mevcutRenk = yeniRenk;
    setBrush(QBrush(yeniRenk));
}

void DugumItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    QGraphicsEllipseItem::setBrush(QBrush(Qt::red));
}

void DugumItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    Q_UNUSED(event);
    setBrush(QBrush(mevcutRenk.isValid() ? mevcutRenk : Qt::white));
}

void DugumItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (!scene()->views().isEmpty()) {
        QWidget* view = scene()->views().first();
        MainWindow* mw = qobject_cast<MainWindow*>(view->window());
        if (mw) {
            mw->dugumeTiklandi(makaleBilgisi.id);
        }
    }
    QGraphicsEllipseItem::mousePressEvent(event);
}

void DugumItem::etiketGuncelle(bool alfabetikMod, int siraNo) {
    for (QGraphicsItem* child : childItems()) { delete child; }

    std::string fullId = makaleBilgisi.id.toStdString();
    size_t wPos = fullId.find("/W");
    QString temizId = (wPos != std::string::npos) ?
                          QString::fromStdString(fullId.substr(wPos + 2)) : makaleBilgisi.id;

    QGraphicsTextItem *textItem = new QGraphicsTextItem(this);
    textItem->setTextWidth(70);

    if (alfabetikMod) {
        int atif = makaleBilgisi.citedBy.size();
        QString harf = (siraNo == 0) ? "A" : QString(QChar('B' + (siraNo - 1)));
        textItem->setHtml(QString("<center>%1<br>%2<br><b>%3</b></center>").arg(atif).arg(temizId).arg(harf));
        textItem->setPos(0, 5);
    } else {
        // YAZAR BAŞ HARFLERİ (Saf C++ Mantığı)
        std::string tumBasHarfler = "";
        for(const QString &yazar : makaleBilgisi.authors) {
            std::stringstream ss(yazar.toStdString());
            std::string kelime;
            // İsimdeki her kelimenin baş harfini al (Örn: Da Wen Sun -> DWS)
            while(ss >> kelime) {
                if(!kelime.empty()) tumBasHarfler += (char)toupper((unsigned char)kelime[0]);
            }
        }

        textItem->setHtml(QString("<center><span style='font-size:9px;'>%1</span><br><b>%2</b></center>")
                              .arg(temizId).arg(QString::fromStdString(tumBasHarfler)));
        textItem->setPos(0, 15);
    }
}
