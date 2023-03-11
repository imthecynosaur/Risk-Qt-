#include "manager.h"

Manager::Manager(QObject *parent)
    : QObject{parent},
      stream{stdin}
{
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
        QObject::connect(tempPlayer, &Player::requestForTerritoryInfoSignal, &territories, &Territories::recieveTerritoryInfoRequest);
        QObject::connect(&territories, &Territories::sendTerritoryInfoSignal, tempPlayer, &Player::fetchRequestedTerritory);
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
            player->showStatus();
            return;
        }
    }
}

bool Manager::checkForWin(int winCondition)
{
    for (const auto& player : players) {
        if (player->getTerritories().size() >= winCondition){
            qDebug() << "PLAYER" << player->getNumber() << "WON THE GAME!";
            return true;
        }
    }
    return false;
}



void Manager::gameLoop()
{
    qDebug() << "How many players will be playing?!";
    int playerCount;
    stream >> playerCount;
    createPlayers(playerCount);

    qDebug() << "How many territories does the winner have to have under control?";
    int winCondition;
    stream >> winCondition;

    distributeTerritories();

    qDebug() << "Initial distribution";
    for (int i = 0; i < 2; ++i) {
        for (const auto& player : players) {
            player->deployTroops(3);
        }
    }
    qDebug() << "GAME STARTED";
    bool isRunning {true};
    while (isRunning){
        for (const auto& player : players) {
            player->deployTroops(player->setDraftCount());
            player->attack();
            if(checkForWin(winCondition)){
                isRunning = false;
                break;
            }
            player->forfeit();
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
