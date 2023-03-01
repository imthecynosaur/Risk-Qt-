#include "player.h"
#include "qdebug.h"

Player::Player(int number, QObject *parent)
    : QObject{parent},
      number{number},
      stream{stdin}
{

}

int Player::getNumber() const
{
    return number;
}

void Player::addTerritory(Territory * territory)
{
    territories.append(territory);
}

QList<Territory *> Player::getTerritories() const
{
    return territories;
}

void Player::deployTroops(int troopsCount){
    QTextStream stream(stdin);
    while (troopsCount > 0){
        qDebug() << "Remaining troops: " << troopsCount;
       qDebug() << "[PLAYER " << getNumber() << "]: " << "where should troops be deployed, sir ?!";
        showStatus();
        int choice{};
        stream >> choice;
        if (choice == 0){
            return;
        }
        if (choice > territories.size() || choice < 0){
            qDebug() << "WRONG TERRITORY !";
            deployTroops(troopsCount);
            return;
        }
        qDebug() << "How many troops should be deployed my lord ?!" ;
        int troopsToBeDeployed {};
        stream >> troopsToBeDeployed;
        if (troopsToBeDeployed > troopsCount){
            qDebug() << "YOU DON'T HAVE THIS MUCH TROOPS MY LORD !";
            deployTroops(troopsCount);
            return;
        }
        territories[choice-1]->setTroops(territories[choice-1]->getTroops() + troopsToBeDeployed);
        troopsCount -= troopsToBeDeployed;
    }
    showStatus();
}


void Player::attack()
{
        chooseEnemyToAttack(chooseTerritoryToAttackFrom());
}



int Player::setDraftCount(){
    int draftCount{std::max(3, static_cast<int>(territories.size() / 3))};
    draftCount += checkForContinent();
    return draftCount;
}

void Player::fetchContinetInfo(const QMap<QString, QList<int> > continentInfo)
{
    this->continetInfo = continentInfo;
}

void Player::fetchEnemyTerritory(Territory * enemy)
{
    this->enemyTerritory = enemy;
}

QList<int> Player::getTerritoryNumbers() const
{
    return territoryNumbers;
}

void Player::setTerritoryNumbers()
{
    foreach (const auto& territory, territories) {
        territoryNumbers.append(territory->getIndex());
    }
}

int Player::checkForContinent()
{
    int continentBouns{};
    QList<int> territoryNumbers;
    foreach (const auto& territory, territories) {
        territoryNumbers.append(territory->getIndex());
    }
    foreach (const auto& continent, continetInfo.keys()){
        bool isComplete {true};
        for (int territoryNumber : continetInfo.value(continent)){
            if (std::find(territoryNumbers.begin(), territoryNumbers.end(), territoryNumber) == territoryNumbers.end()){
                isComplete = false;
                break;
            }

        }
        if (isComplete) {
            int bonus{};
            if (continent == "North America"){
                bonus = 5;
            }
            if (continent == "South America"){
                bonus = 2;
            }
            if (continent == "Europe"){
                bonus = 5;
            }
            if (continent == "Africa"){
                bonus = 3;
            }
            if (continent == "Asia"){
                bonus = 7;
            }
            if (continent == "Australia"){
                bonus = 2;
            }
            continentBouns += bonus;
            qDebug() << "getting bonus troops from " << continent;
        }
    }
    return continentBouns;
}

Territory *Player::chooseTerritoryToAttackFrom()
{
    QList<Territory*> eligibleTerritories{};
    setTerritoryNumbers();
        for (const auto& territory : territories){
            if (territory->getTroops() > 1){
                for (auto neighbourTerritoryIndex : territory->getNeighbours()){
                    if (territoryNumbers.indexOf(neighbourTerritoryIndex) == -1){
                        if (eligibleTerritories.indexOf(territory) == -1){
                            eligibleTerritories.append(territory);
                        }
                        break;

                    }
                }
            }
        }

        int choice{};

        qDebug() << "ELIGIBLE TERRITORIES :";
        for (auto& territory : eligibleTerritories){
            qDebug() << eligibleTerritories.indexOf(territory)+1 << "- " << territory->getName() << "[" << territory->getTroops() << "]";
        }
        stream >> choice;
        return eligibleTerritories[choice-1];
}

Territory *Player::chooseEnemyToAttack(Territory * attackingTerritory)
{
    QList<Territory*> availableEnemies{};
    for (int territoryIndex : attackingTerritory->getNeighbours()){
        if (territoryNumbers.indexOf(territoryIndex) == -1){
            emit requestForEnemyInfoSignal(territoryIndex);
            availableEnemies.append(enemyTerritory);
        }
    }
    qDebug() << "AVAILABLE ENEMIES :";
    foreach (const auto& enemy, availableEnemies) {
        qDebug() << availableEnemies.indexOf(enemy)+1 << enemy->getName() << enemy->getTroops();
    }
    int choice;
    stream >> choice;
    Territory* enemyTerritory = availableEnemies[choice-1];
    qDebug() << " NEW VERSION : attacking " << enemyTerritory->getName() << " from " << attackingTerritory->getName();
    return enemyTerritory;
}

void Player::showStatus()
{
    for (const auto& territory : territories) {
        qDebug() << territories.indexOf(territory) + 1 << "-" << territory->getName() << territory->getTroops();
    }
}

//void Player::loseTerritory(Territory * territory)
//{
//    territories.removeAt(territories.indexOf(territory->getIndex()));
//    qDebug() << "territory removed";
//}


