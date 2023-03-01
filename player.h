#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
#include <QMap>
#include <QRandomGenerator>
#include "qdatetime.h"
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
    QList<int> getTerritoryNumbers() const;
    void setTerritoryNumbers();

signals:
    void requestForEnemyInfoSignal(int enemyIndex);

public slots:
    void fetchContinetInfo(const QMap<QString, QList<int>>);
    void fetchEnemyTerritory(Territory*);

private:
    QTextStream stream;
    int number;

    Territory* enemyTerritory;
    QList<Territory*> territories;
    QList<int> territoryNumbers;

    int checkForContinent();
    QMap<QString, QList<int> > continetInfo;

    Territory* chooseTerritoryToAttackFrom();
    Territory* chooseEnemyToAttack(Territory*);

    void showStatus();

    QList<int> rollDice(int);

    void attackPhase(Territory*, Territory*);

};

#endif // PLAYER_H
