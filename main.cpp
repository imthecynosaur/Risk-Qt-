#include <QCoreApplication>
#include <QTextStream>
#include "manager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Manager manager;
    manager.createPlayers(5);
    manager.distributeTerritories();

//    foreach (Player* player, manager.getPlayers()) {
//        foreach (Territory* territory, player->getTerritories()) {
//            qDebug() << territory->getName() << territory->getOwnerNumber();
//        }
//    }

    return a.exec();
}
