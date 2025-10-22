#ifndef UNTITLED12_PIRATE_H
#define UNTITLED12_PIRATE_H
#include "ship.h"
class Ship;
class Pirate {
public:
    Pirate();
    Pirate(int pirateId, std::shared_ptr<Ship> ship, int treasure);
    //Pirate(int pirateId, int treasure);
    Pirate(const Pirate& pirate);
    Pirate& operator =(const Pirate& pirate)=delete;
    int getPirateId() const;
    int getTreasure() const;
    int getTreasureClean() const;
    void setTreasure(int change);
    void setPiratesPos(std::shared_ptr<Node<Pirate>> pos);
    std::shared_ptr<Node<Pirate>> getPiratesPos();
    void changeShip(std::weak_ptr<Ship> ship);
    std::shared_ptr<Ship> getShip() const;
    friend std::ostream& operator<<(std::ostream& os, const Pirate pirate);
private:
    int m_pirateId;
    std::weak_ptr<Ship> m_ship;
    int m_treasure;
    std::weak_ptr<Node<Pirate>> m_Piratespos;
};


#endif //UNTITLED12_PIRATE_H
