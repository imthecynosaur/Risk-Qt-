#ifndef TERRITORY_H
#define TERRITORY_H

#include <QObject>
#include <QList>


class Territory : public QObject
{
    Q_OBJECT

public:
    explicit Territory(QString continent, int index, QString name, QList<int> neighbours, QObject *parent = nullptr);

    QString getContinent() const;

    int getIndex() const;

    QString getName() const;

    QList<int> getNeighbours() const;

    int getOwnerNumber() const;
    void setOwnerNumber(int newOwnerNumber);


    int getTroops() const;
    void setTroops(int newTroops);

signals:
    void ownerChanged(Territory*, int newOwner);

private:
    QString continent;
    int index;
    QString name;
    QList<int> neighbours;
    int ownerNumber {-1};
    int troops{1};


};

#endif // TERRITORY_H
