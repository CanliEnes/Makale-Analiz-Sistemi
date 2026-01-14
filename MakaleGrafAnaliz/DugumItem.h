#ifndef DUGUMITEM_H
#define DUGUMITEM_H

#include <QGraphicsEllipseItem>
#include "Makale.h"

class MainWindow;

class DugumItem : public QGraphicsEllipseItem {
public:
    Makale makaleBilgisi;
    DugumItem(Makale m);
    void farkliBoyat(QColor yeniRenk);
    void etiketGuncelle(bool alfabetikMod, int siraNo = 0);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
private:
    QColor mevcutRenk = Qt::white;

};


#endif
