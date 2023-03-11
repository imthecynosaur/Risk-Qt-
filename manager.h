#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include <QList>
#include <QTextStream>

#include "player.h"
#include "territories.h"

class Manager : public QObject
{
    Q_OBJECT
public:
    explicit Manager(QObject *parent = nullptr);
    void gameLoop();

signals:
public slots:
    void changeTerritoryOwnership(Territory*, int, int);

private:
    QTextStream stream;
    QList<Player*> players;
    Territories territories;
    void connectSignals();

    void addTerritoryToPlayer(Territory* territory, int playerNumber);
    void takeTerritoryFromPlayer(Territory* territoryptr, int playerNumber);

    bool checkForWin(int);

    void distributeTerritories();

    void createPlayers(int playerCount);
    QList<Player *> getPlayers() const;

};

#endif // MANAGER_H
