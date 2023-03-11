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
    setTerritoryNumbers();
}

void Player::loseTerritory(Territory * territory)
{
    territories.removeAt(territories.indexOf(territory));
    setTerritoryNumbers();
}

QList<Territory *> Player::getTerritories() const
{
    return territories;
}

void Player::deployTroops(int troopsCount){
    QTextStream stream(stdin);
    while (troopsCount > 0){
        qDebug() << "[PLAYER " << getNumber() << "]";
        qDebug() << "Remaining troops: " << troopsCount;
       qDebug()  << "where should troops be deployed, sir ?!";
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
    qDebug() << "Do you want to attack sir ?!";
    qDebug() << "1 - YES";
    qDebug() << "2 - NO";
    int choice;
    stream >> choice;
    if (choice == 2){
        return;
    }
    Territory* attacker {chooseTerritoryToAttackFrom()};
    if (!attacker){
        qDebug() << "can't initiate any other attacks, sir!";
        return;
    }
    Territory* defender {chooseEnemyToAttack(attacker)};
    if (!defender){
        qDebug() << "no more enemies left to attack, sir!";
        return;
    }
    attackPhase(attacker, defender);
    attack();
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

void Player::fetchRequestedTerritory(Territory * territory)
{
    this->requestedTerritory = territory;
}

QList<int> Player::getTerritoryNumbers() const
{
    return territoryNumbers;
}

void Player::setTerritoryNumbers()
{
    territoryNumbers.clear();
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
        if (eligibleTerritories.size() == 0){
            return nullptr;
        }

        qDebug() << "ELIGIBLE TERRITORIES :";
        for (auto& territory : eligibleTerritories){
            qDebug() << eligibleTerritories.indexOf(territory)+1 << "- " << territory->getName() << "[" << territory->getTroops() << "]";
        }
        int choice{};
        stream >> choice;
        return eligibleTerritories[choice-1];
}

Territory *Player::chooseEnemyToAttack(Territory * attackingTerritory)
{
    QList<Territory*> availableEnemies{};
    for (int territoryIndex : attackingTerritory->getNeighbours()){
        if (territoryNumbers.indexOf(territoryIndex) == -1){
            emit requestForTerritoryInfoSignal(territoryIndex);
            availableEnemies.append(requestedTerritory);
        }
    }
    if (availableEnemies.size() == 0){
        return nullptr;
    }
    qDebug() << "AVAILABLE ENEMIES :";
    foreach (const auto& enemy, availableEnemies) {
        qDebug() << availableEnemies.indexOf(enemy)+1 << enemy->getName() << "TROOPS:"  << enemy->getTroops();
    }
    int choice;
    stream >> choice;
    Territory* enemyTerritory = availableEnemies[choice-1];
    qDebug() << "attacking " << enemyTerritory->getName() << " from " << attackingTerritory->getName();
    return enemyTerritory;
}

void Player::showStatus()
{
    for (const auto& territory : territories) {
        qDebug() << territories.indexOf(territory) + 1 << "-" << territory->getName() << "TROOPS:" << territory->getTroops();
    }
}

QList<int> Player::rollDice(int diceCount)
{
    QList<int> result;
    QRandomGenerator generator;
    generator.seed(QDateTime::currentMSecsSinceEpoch() % 1000000);
    for (int i = 0; i < diceCount; i++) {
        result.append(generator.bounded(1, 7));
    }
    return result;
}

void Player::attackPhase(Territory * attacker, Territory * defender)
{
    QList<int> attackersDice{rollDice(std::min(3, attacker->getTroops()-1))};
    QList<int> defendersDice{rollDice(std::min(2, defender->getTroops()))};
    std::sort(attackersDice.begin(), attackersDice.end(), [](int a, int b){ return a > b; });
    std::sort(defendersDice.begin(), defendersDice.end(), [](int a, int b){ return a > b; });
    qDebug() << "attacker's Dices:" << attackersDice;
    qDebug() << "defender's Dices:" << defendersDice;
    for (int i = 0; i < std::min(attackersDice.size(), defendersDice.size()); i++) {
        if (defendersDice[i] >= attackersDice[i]){
            qDebug() << "attacker:" << attacker->getTroops() << "->" << attacker->getTroops()-1;
            attacker->setTroops(attacker->getTroops()-1);
        }else {
            qDebug() << "defender:" << defender->getTroops() << "->" << defender->getTroops()-1;
            defender->setTroops(defender->getTroops()-1);
        }
    }
    if(attacker->getTroops() == 1){
        return;
    }
    if(defender->getTroops() == 0){
        emit defender->ownerChanged(defender, attacker->getOwnerNumber(), defender->getOwnerNumber());
        qDebug() << defender->getName() << "now belongs to PLAYER NUMBER" << defender->getOwnerNumber();
        transferTroops(attacker, defender, false);
        return;
    }
    qDebug() << "continue attacking?";
    qDebug() << "1- YES";
    qDebug() << "2- NO";
    int choice;
    stream >> choice;
    if (choice == 1){
        attackPhase(attacker, defender);
    }
}

void Player::transferTroops(Territory * fromTerritory, Territory * toTerritory, bool isFriendly)
{
    qDebug() << "how many troops should be deployed to the new territory, sir?";
    qDebug() << fromTerritory->getTroops() << "-> ?(" << toTerritory->getTroops() << ")";
    int transferredTroops;
    stream >> transferredTroops;
    if (transferredTroops >= fromTerritory->getTroops() || transferredTroops <= 0){
        qDebug() << "You can't transfer this amount of troops, sir";
        transferTroops(fromTerritory, toTerritory, isFriendly);
        return;
    }
    fromTerritory->setTroops(fromTerritory->getTroops()-transferredTroops);
    if (isFriendly){
        toTerritory->setTroops(toTerritory->getTroops()+transferredTroops);
    }else{
        toTerritory->setTroops(transferredTroops);
    }
    showStatus();
}

void Player::getConnectedTerritories(Territory * territory, QList<int>& neighbours)
{
    foreach (int neighbour, territory->getNeighbours()) {
        if (territoryNumbers.contains(neighbour)){
            if (!neighbours.contains(neighbour)){
                neighbours.append(neighbour);
//                qDebug() << neighbours;
                emit requestForTerritoryInfoSignal(neighbour);
//                qDebug() << "workin on" << requestedTerritory->getName();
                getConnectedTerritories(requestedTerritory, neighbours);
            }
        }
    }
}

void Player::forfeit()
{
    setTerritoryNumbers();
    qDebug() << "do you want to forfeit sir?";
    qDebug() << "1-YES";
    qDebug() << "2-NO";
    int choice;
    stream >> choice;
    if (choice == 2){
        return;
    }
    qDebug() << "Available Territories :";
    QList<Territory*> availableTerritories;
    foreach (const auto& territory, territories) {
        if (territory->getTroops() > 1){
            for (int neighbour : territory->getNeighbours()) {
                if (territoryNumbers.contains(neighbour)){
                    availableTerritories.append(territory);
                    qDebug() << availableTerritories.size() << "-" << territory->getName() << "Troops:" << territory->getTroops();
                    break;
                }
            }
        }
    }
    stream >> choice;
    Territory* selectedTerritory = availableTerritories[choice-1];
    qDebug() << selectedTerritory->getName() << "selected";

    qDebug() << "Available Destinations :";
    QList<int> availableDestinations;
    getConnectedTerritories(selectedTerritory, availableDestinations);
    availableDestinations.removeAt(availableDestinations.indexOf(selectedTerritory->getIndex()));
    for (int destination : availableDestinations) {
        emit requestForTerritoryInfoSignal(destination);
        qDebug() << availableDestinations.indexOf(destination) + 1 << "-" << requestedTerritory->getName();
    }
    stream >> choice;
    emit requestForTerritoryInfoSignal(availableDestinations[choice-1]);
    Territory* selectedDestination{requestedTerritory};
    transferTroops(selectedTerritory, selectedDestination, true);
}


