#include "manager.h"

Manager::Manager(QObject *parent)
    : QObject{parent}
{
    qDebug() << "manager created!" ;
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

void Manager::addTerritoryToPlayer(Territory* territory, int playerNumber)
{
    territory->setOwnerNumber(playerNumber);
    foreach (Player* player, players) {
        if (player->getNumber() == playerNumber){
            player->addTerritory(territory);
            return;
        }
    }
}

QList<Player *> Manager::getPlayers() const
{
    return players;
}

//void Manager::takeTerritoryFromPlayer(int playerNumber, Territory *territoryptr)
//{
//    foreach (Player* player, players) {
//        if (player->getNumber() == playerNumber){
//            player->loseTerritory(territoryptr);
//            return;
//        }
//    }
//}


