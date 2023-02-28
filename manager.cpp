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
//        territory->setOwnerNumber(i);
        addTerritoryToPlayer(territory, i);
//        players[i-1]->addTerritory(territory);
        if (++i > players.size()){
            i %= players.size();
        }
    }
}

void Manager::createPlayers(int playerCount)
{
    for (int i=1; i<=playerCount; i++){
        Player* tempPlayer = new Player(i);
        players.append(tempPlayer);
    }
}

void Manager::addTerritoryToPlayer(Territory* territory, int playerNumber)
{
//    foreach (Player* player, players) {
//        if (player->getNumber() == playerNumber){
//            player->addTerritory(territoryptr);
//            return;
//        }
//    }
    territory->setOwnerNumber(playerNumber);

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


