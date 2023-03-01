#include "territories.h"

Territories::Territories(QObject *parent)
    : QObject{parent},
      territories{createTerritory()},
      continentInfo{setContinentInfo()}
{
    qDebug() << "Territories created";
}

const QList<Territory *> Territories::getTerritories() const
{
    return territories;
}

QList<Territory *> Territories::createTerritory()
{
    QList<Territory*> tempTerritories;
    QString filePath = QDir::toNativeSeparators("D:/C++/Qt projects/Risk/Risk/territories.json");

    QFile file(filePath);


    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QString contents = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(contents.toUtf8());

    QJsonObject root = doc.object();
    QJsonArray continents = root.value("continents").toArray();

    for (int i = 0; i < continents.size(); i++) {
        QJsonObject continent = continents[i].toObject();
        QString continentName = continent.value("name").toString();
        QJsonArray territories = continent.value("territories").toArray();

        for (int j = 0; j < territories.size(); j++) {
            QJsonObject territory = territories[j].toObject();
            int index = territory.value("index").toInt();
            QString name = territory.value("name").toString();
            QJsonArray neighborsArray = territory.value("neighbors").toArray();
            QList<int> neighbors;

            for (int k = 0; k < neighborsArray.size(); k++) {
                int neighborIndex = neighborsArray[k].toInt();
                neighbors.append(neighborIndex);
            }

            tempTerritories.append(new Territory(continentName, index, name, neighbors));
        }
    }
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(tempTerritories.begin(), tempTerritories.end(), g);
    return tempTerritories;
}


Territory *Territories::getTerritoryByInedx(int index) const
{
    foreach (Territory* territory, territories) {
        if (territory->getIndex() == index){
            return territory;
        }
    }
    qDebug() << "Not found!";
    return nullptr;
}

Territory *Territories::getTerritoryByName(QString name) const
{
    qDebug() << name;
    foreach (Territory* territory, territories) {
        if (territory->getName().toLower().trimmed() == name.toLower().trimmed()){
            return territory;
        }
    }
    qDebug() << "Not found!";
    return nullptr;
}

const QMap<QString, QList<int> > Territories::getContinentInfo() const
{
    return continentInfo;
}

const QMap<QString, QList<int> > Territories::setContinentInfo()
{
    QMap<QString, QList<int> > tempContinet;
    foreach(Territory* territory, territories){
        QList<int> tempValue {tempContinet.value(territory->getContinent())};
        tempValue.append(territory->getIndex());
        tempContinet.insert(territory->getContinent(),  QList<int> {tempValue});
    }
    return tempContinet;
}

void Territories::sendContinentInfo()
{
    emit sendContinentInfoSignal(continentInfo);
}

void Territories::recieveTerritoryInfoRequest(int territoryIndex)
{
    emit sendTerritoryInfoSignal(getTerritoryByInedx(territoryIndex));
}
