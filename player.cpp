#include "player.h"

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

//void Player::addTerritory(Territory * territory)
//{
//    territories.append(territory);
//}

//QList<Territory *> Player::getTerritories() const
//{
//    return territories;
//}

//int Player::checkForContinent()
//{

//}

//void Player::loseTerritory(Territory * territory)
//{
//    territories.removeAt(territories.indexOf(territory->getIndex()));
//    qDebug() << "territory removed";
//}


