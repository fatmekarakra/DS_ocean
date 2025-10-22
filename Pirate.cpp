#include "Pirate.h"
//#include "ship.h"
Pirate::Pirate(int pirateId, std::shared_ptr<Ship> ship, int treasure):m_pirateId(pirateId), m_ship(ship), m_treasure(treasure){};
//Pirate::Pirate(int pirateId, int treasure):m_pirateId(pirateId), m_treasure(treasure),m_Piratespos(nullptr){};
//Pirate::Pirate(int pirateId, int treasure):m_pirateId(pirateId), m_treasure(treasure){};
/*Pirate::Pirate(const Pirate& pirate){
    m_pirateId=pirate.m_pirateId;
    m_Piratespos=pirate.m_Piratespos;
    m_ship=pirate.m_ship;
    m_treasure=pirate.m_treasure;
}*/
Pirate::Pirate(const Pirate& pirate)
    : m_pirateId(pirate.m_pirateId), 
      m_ship(pirate.m_ship), 
      m_treasure(pirate.m_treasure), 
      m_Piratespos(pirate.m_Piratespos) {
}
int Pirate::getPirateId() const{
    return m_pirateId;
}
int Pirate::getTreasure()const{
    if (auto ship = m_ship.lock()) {
        return m_treasure + ship->getExtra();
    }
    return m_treasure;
    //return m_treasure+m_ship->getExtra();
    //return m_treasure;
}

int Pirate::getTreasureClean()const{
    //return m_treasure+m_ship->getExtra();
    return m_treasure;
}
void Pirate::setTreasure(int change){
    m_treasure=m_treasure+change;
}
void Pirate::setPiratesPos(std::shared_ptr<Node<Pirate>> pos){
    m_Piratespos=pos;
}
std::shared_ptr<Node<Pirate>> Pirate::getPiratesPos(){
    return m_Piratespos.lock();
}
void Pirate::changeShip(std::weak_ptr<Ship> ship) {
    m_ship = ship;
}

std::shared_ptr<Ship> Pirate::getShip() const {
    return m_ship.lock(); // Lock the weak_ptr to get the shared_ptr if it is still valid
}
std::ostream& operator<<(std::ostream& os, const Pirate pirate) {
    os<<"PId"<<pirate.getPirateId()<<" SId: "<<pirate.getShip()->getShipId()<<" treasure "<<pirate.getTreasureClean();
    return os;
}
/*Pirate& Pirate::operator=(const Pirate& pirate) {
    if (this == &pirate) {
        return *this; // Handle self-assignment
    }

    // Copy the simple data members
    m_pirateId = pirate.m_pirateId;
    m_treasure = pirate.m_treasure;

    // Copy the shared_ptr and weak_ptr
    m_ship = pirate.m_ship;
    m_Piratespos = pirate.m_Piratespos;

    return *this;
}*/