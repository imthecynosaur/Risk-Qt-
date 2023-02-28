#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QList>

#include "player.h"
#include "territories.h"

class Manager : public QObject
{
    Q_OBJECT
public:

    explicit Manager(QObject *parent = nullptr);
    void distributeTerritories();
    void createPlayers(int playerCount);
    QList<Player *> getPlayers() const;

signals:


public slots:
    void addTerritoryToPlayer(Territory* territory, int playerNumber);
//    void takeTerritoryFromPlayer(int playerNumber, Territory* territoryptr = nullptr);


private:
    QList<Player*> players;
    Territories territories;
};

#endif // MANAGER_H
