#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QList>
#include <QMap>
#include "territory.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(int number, QObject *parent = nullptr);

    int getNumber() const;

    void addTerritory(Territory*);
    void loseTerritory(Territory*);
    QList<Territory *> getTerritories() const;

signals:

public slots:
    void fetchContinetInfo(const QMap<QString, QList<int>>);

private:
    int number;

    QList<Territory*> territories;

    int checkForContinent();
     QMap<QString, QList<int> > continetInfo;


};

#endif // PLAYER_H
