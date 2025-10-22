#ifndef UNTITLED12_SHIP_H
#define UNTITLED12_SHIP_H
#include "AVLTree.h"
#include "Queue.h"
#include "Pirate.h"
#include "Pair.h"
class Pirate;
class Ship {
public:
    Ship();
    Ship(int shipId, int cannons);
    Ship(const Ship& ship)=delete;
    Ship operator=(const Ship& ship)=delete;
    ~Ship();
    void addPirate(Pirate& pirate);
    void removePirate(Pirate& pirate);
    void treason(Pirate& pirate);
    int getExtra();
    void changeExtra(int change);
    int getCannons() const;
    int getPiratesNumber();
    Pirate* getOldestPirate();
    int getShipId();
    int getRichestPirate();
    void pirateTreasureUpdate(Pirate& pirate,int change);
    friend std::ostream& operator<<(std::ostream& os, const Ship ship);
private:
    int m_shipId;
    int m_cannons;
    int m_extra;
    int richestPirate;
    AVLTree<KV<Pair,std::shared_ptr<Pirate>>> m_pirates;
    Queue<Pirate> m_piratesQueue;
};


#endif //UNTITLED12_SHIP_H
