#ifndef MAKALE_H
#define MAKALE_H

#include <QString>
#include <QStringList>

struct Makale {

    QString id;                 // Makalenin tam ID'si
    QString title;              // Başlık
    int year;                   // Basım yılı
    QStringList authors;        // Yazar listesi
    QStringList references;     // Atıf yaptığı makaleler (Siyah Oklar)
    QStringList citedBy;        // Bu makaleye atıf yapanlar (H-index için)
    long long numericId;        // Yeşil kenar sıralaması için ID

    double centralityScore = 0.0;
    int kCoreValue = 0;
};

#endif // MAKALE_H
