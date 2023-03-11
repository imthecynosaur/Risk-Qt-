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
    void forfeit();

    QList<int> getTerritoryNumbers() const;
    void setTerritoryNumbers();

    void showStatus();

signals:
    void requestForTerritoryInfoSignal(int enemyIndex);

public slots:
    void fetchContinetInfo(const QMap<QString, QList<int>>);
    void fetchRequestedTerritory(Territory*);

private:
    QTextStream stream;
    int number;

    Territory* requestedTerritory;
    QList<Territory*> territories;
    QList<int> territoryNumbers;

    int checkForContinent();
    QMap<QString, QList<int> > continetInfo;

    Territory* chooseTerritoryToAttackFrom();
    Territory* chooseEnemyToAttack(Territory*);



    QList<int> rollDice(int);

    void attackPhase(Territory*, Territory*);
    void transferTroops(Territory*, Territory*, bool);

    void getConnectedTerritories(Territory*, QList<int>&);

};

#endif // PLAYER_H
