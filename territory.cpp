#include "territory.h"

Territory::Territory(QString continent, int index, QString name, QList<int> neighbours, QObject *parent)
    : QObject{parent},
      continent{continent},
      index{index},
      name{name},
      neighbours{neighbours}
{

}

QString Territory::getContinent() const
{
    return continent;
}

int Territory::getIndex() const
{
    return index;
}

QString Territory::getName() const
{
    return name;
}

QList<int> Territory::getNeighbours() const
{
    return neighbours;
}

int Territory::getOwnerNumber() const
{
    return ownerNumber;
}

void Territory::setOwnerNumber(int newOwnerNumber)
{
    ownerNumber = newOwnerNumber;
}
