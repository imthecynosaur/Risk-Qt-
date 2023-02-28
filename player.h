#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
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

signals:
    void addTerritorySignal(int playerNumber, Territory* territoryPtr);
    void loseTerritorySignal(int playerNumber, Territory* territoryPtr);

private:
    int number;

    QList<Territory*> territories;
    int checkForContinent();


};

#endif // PLAYER_H