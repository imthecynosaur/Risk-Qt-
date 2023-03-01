#include "player.h"
#include "qdebug.h"

Player::Player(int number, QObject *parent)
    : QObject{parent},
      number{number}
{
//    this->connect(this, addTerritorySignal&, Territory&, Territory::);
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

void Player::fetchContinetInfo(const QMap<QString, QList<int> > continentInfo)
{
    this->continetInfo = continentInfo;
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

//void Player::loseTerritory(Territory * territory)
//{
//    territories.removeAt(territories.indexOf(territory->getIndex()));
//    qDebug() << "territory removed";
//}


