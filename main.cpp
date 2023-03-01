#include <QCoreApplication>
#include <QTextStream>
#include "manager.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Manager manager;
    manager.createPlayers(2);
    manager.distributeTerritories();


    foreach (Player* player, manager.getPlayers()) {
//        foreach (Territory* territory, player->getTerritories()) {
//            qDebug() << territory->getName() << territory->getTroops();
//        }
        player->deployTroops(15);
        player->attack();
    }

    return a.exec();
}
