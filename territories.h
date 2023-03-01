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
#include <QMap>

#include "territory.h"


class Territories : public QObject
{
    Q_OBJECT
public:
    explicit Territories(QObject *parent = nullptr);

    const QList<Territory*> getTerritories() const;

    Territory* getTerritoryByInedx(int index) const;
    Territory* getTerritoryByName(QString name) const;

    const QMap<QString, QList<int> > getContinentInfo() const;
    const QMap<QString, QList<int> > setContinentInfo();

    void sendContinentInfo();

signals:
    void sendContinentInfoSignal(const QMap<QString, QList<int>>);

private:
    const QList<Territory*> territories;
    const QMap<QString, QList<int>> continentInfo;

    QList<Territory*> createTerritory();

};

#endif // TERRITORIES_H
