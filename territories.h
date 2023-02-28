#ifndef TERRITORIES_H
#define TERRITORIES_H

#include <QObject>
#include <QList>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDir>
#include <algorithm>
#include <random>

#include "territory.h"


class Territories : public QObject
{
    Q_OBJECT
public:
    explicit Territories(QObject *parent = nullptr);

    const QList<Territory*> getTerritories() const;

    Territory* getTerritoryByInedx(int index) const;
    Territory* getTerritoryByName(QString name) const;

signals:


private:
    const QList<Territory*> territories;

    QList<Territory*> createTerritory();

};

#endif // TERRITORIES_H
