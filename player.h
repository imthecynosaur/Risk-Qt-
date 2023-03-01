#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QTextStream>
#include "territory.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(int number, QObject *parent = nullptr);

    int getNumber() const;

    void addTerritory(Territory*);
    void loseTerritory(Territory*);
    QList<Territory *> getTerritories() const;

    int setDraftCount();
    void deployTroops(int troopsCount);

    void attack();
signals:
    void requestForEnemyInfoSignal(int enemyIndex);

public slots:
    void fetchContinetInfo(const QMap<QString, QList<int>>);
    void fetchEnemyTerritory(Territory*);

private:
    int number;

    Territory* enemyTerritory;
    QList<Territory*> territories;

    int checkForContinent();
    QMap<QString, QList<int> > continetInfo;

    void showStatus();

};

#endif // PLAYER_H
