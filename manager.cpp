#include "manager.h"

Manager::Manager(QObject *parent)
    : QObject{parent}
{
    qDebug() << "manager created!" ;
    connectSignals();
}

void Manager::distributeTerritories()
{
    int i = 1;
    foreach (Territory* territory, territories.getTerritories()) {
        addTerritoryToPlayer(territory, i);
        if (++i > players.size()){
            i %= players.size();
        }
    }
}

void Manager::createPlayers(int playerCount)
{
    for (int i=1; i<=playerCount; i++){
        Player* tempPlayer = new Player(i);
        QObject::connect(&territories, &Territories::sendContinentInfoSignal, tempPlayer, &Player::fetchContinetInfo);
        QObject::connect(tempPlayer, &Player::requestForEnemyInfoSignal, &territories, &Territories::recieveTerritoryInfoRequest);
        QObject::connect(&territories, &Territories::sendTerritoryInfoSignal, tempPlayer, &Player::fetchEnemyTerritory);
        players.append(tempPlayer);
    }
    territories.sendContinentInfo();
}

void Manager::connectSignals()
{
    for (auto& territory : territories.getTerritories()) {
        QObject::connect(territory, &Territory::ownerChanged, this, &Manager::changeTerritoryOwnership);
    }
}

QList<Player *> Manager::getPlayers() const
{
    return players;
}

void Manager::changeTerritoryOwnership(Territory * territory, int newOwner, int previousOwner)
{
    takeTerritoryFromPlayer(territory, previousOwner);
    addTerritoryToPlayer(territory, newOwner);
}

void Manager::takeTerritoryFromPlayer(Territory *territory, int playerNumber)
{
    foreach (Player* player, players) {
        if (player->getNumber() == playerNumber){
            player->loseTerritory(territory);
            territory->setOwnerNumber(-1);
            return;
        }
    }
}

void Manager::addTerritoryToPlayer(Territory* territory, int playerNumber)
{
    territory->setOwnerNumber(playerNumber);
    foreach (Player* player, players) {
        if (player->getNumber() == playerNumber){
            player->addTerritory(territory);
            territory->setOwnerNumber(playerNumber);
            return;
        }
    }
}


