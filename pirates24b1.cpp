#include "pirates24b1.h"
#include <iostream>

static int min(int a, int b){
    if(a<b){
        return a;
    }
    return b;
}

Ocean::Ocean() : m_ships(), m_pirates() {}


Ocean::~Ocean()=default;

StatusType Ocean::add_ship(int shipId, int cannons)
{
    if (shipId <= 0 || cannons < 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        std::shared_ptr<Ship> temp=std::make_shared<Ship>(shipId, cannons);
        KV<int, std::shared_ptr<Ship>> newShip(shipId,temp);
        if (m_ships.find(newShip)) {
            return StatusType::FAILURE;
        }
        m_ships.insert(newShip);
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::remove_ship(int shipId)
{
    if (shipId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int,  std::shared_ptr<Ship>> shipKV(shipId);
        if (!m_ships.find(shipKV)) {
            return StatusType::FAILURE;
        }
        if (m_ships.get(shipKV).getValue()->getPiratesNumber()>0) {
            return StatusType::FAILURE;
        }
        m_ships.erase(shipKV);
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::add_pirate(int pirateId, int shipId, int treasure)
{
    if (shipId <= 0 || pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        /*if(pirateId==65){
            std::cout<<"before finding the ship"<<std::endl;
        }*/
        KV<int,  std::shared_ptr<Ship>> shipKV(shipId);
        if (!m_ships.find(shipKV)) {
            return StatusType::FAILURE;
        }
        /*if(pirateId==65){
            std::cout<<"before finding the pirate"<<std::endl;
        }*/
        KV<int, Pirate> pirateKey(pirateId);
        if (m_pirates.find(pirateKey)) {
            return StatusType::FAILURE;
        }
        //if(pirateId==65){std::cout<<"before getting the ship"<<std::endl;}
        std::shared_ptr<Ship> ship = m_ships.get(shipKV).getValue();
        Pirate newPirate(pirateId, ship, treasure);
        /*if(pirateId==65){
            if(ship==nullptr){
                std::cout<<"ship is null"<<std::endl;
            }
            else{
                std::cout<<"ship is not null"<<std::endl;
                std::cout<<ship->getShipId()<<std::endl;
                std::cout<<ship->getPiratesNumber()<<std::endl;
            }
        }
        if(pirateId==65){
            std::cout<<m_pirates<<std::endl;
        }*/
        ship->addPirate(newPirate);
        /*if(pirateId==38){
            std::cout<<"this is pirate 38 his coins when added to the tree is "<<newPirate.getTreasureClean()<<std::endl;
        }*/
        KV<int, Pirate> pirateKV(pirateId, newPirate);
        m_pirates.insert(pirateKV);
        return StatusType::SUCCESS;
    } 
    catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::remove_pirate(int pirateId)
{
    if (pirateId <= 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int, Pirate> pirateKV(pirateId);
        if (!m_pirates.find(pirateKV)) {
            return StatusType::FAILURE;
        }
        Pirate temp=m_pirates.get(pirateKV).getValue();
        std::shared_ptr<Ship> ship=temp.getShip();
        /*std::cout << temp.getPirateId()<< std::endl;
        std::cout << temp.getTreasureClean()<< std::endl;*/
        ship->removePirate(temp);
        m_pirates.erase(pirateKV);
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::treason(int sourceShipId, int destShipId)
{
    if (sourceShipId <= 0 || destShipId<=0 || sourceShipId==destShipId) {
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int,  std::shared_ptr<Ship>> shipKVSource(sourceShipId);
        KV<int,  std::shared_ptr<Ship>> shipKVDest(destShipId);
        if (!m_ships.find(shipKVSource) || !m_ships.find(shipKVDest)) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Ship> shipSource = m_ships.get(shipKVSource).getValue();
        if(shipSource->getPiratesNumber()<=0){
            return StatusType::FAILURE;
        }
        std::shared_ptr<Ship> shipDest = m_ships.get(shipKVDest).getValue();
        Pirate pirateTemp= *shipSource->getOldestPirate();
        KV<int, Pirate> pirateKV(pirateTemp.getPirateId());
        Pirate pirateOriginal=m_pirates.get(pirateKV).getValue();
        pirateOriginal.changeShip(shipDest);
        shipSource->treason(pirateOriginal);
        shipDest->addPirate(pirateOriginal);
        m_pirates.erase(pirateKV);
        KV<int, Pirate> newpirateKV(pirateOriginal.getPirateId(),pirateOriginal);
        m_pirates.insert(newpirateKV);
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::update_pirate_treasure(int pirateId, int change)
{
    if(pirateId<=0){
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int, Pirate> pirateKV(pirateId);
        if (!m_pirates.find(pirateKV)) {
            return StatusType::FAILURE;
        }
        Pirate& pirate = m_pirates.get(pirateKV).getValue();
        std::shared_ptr<Ship> ship=pirate.getShip();
        /*if(pirateId==59){
            std::cout<<"treasure before setting it is "<<pirate.getTreasureClean()<<" and change is "<<change<<std::endl;
        }*/
        pirate.setTreasure(change);
        /*if(pirateId==59){
            std::cout<<"treasure after setting it is "<<pirate.getTreasureClean()<<std::endl;
        }*/
        ship->pirateTreasureUpdate(pirate,change);
        m_pirates.erase(pirateKV);
        KV<int, Pirate> newpirateKV(pirateId,pirate);
        m_pirates.insert(newpirateKV);
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Ocean::get_treasure(int pirateId)
{
    if(pirateId<=0){
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int, Pirate> pirateKV(pirateId);
        if (!m_pirates.find(pirateKV)) {
            return StatusType::FAILURE;
        }
        Pirate& pirate = m_pirates.get(pirateKV).getValue();
        return output_t<int> (pirate.getTreasure());
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}


output_t<int> Ocean::get_cannons(int shipId)
{
    if(shipId<=0){
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int,  std::shared_ptr<Ship>> shipKV(shipId);
        if (!m_ships.find(shipKV)) {
            return StatusType::FAILURE;
        }
        Ship& ship = *m_ships.get(shipKV).getValue();
        return output_t<int> (ship.getCannons());
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

output_t<int> Ocean::get_richest_pirate(int shipId)
{
    if(shipId<=0){
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int,  std::shared_ptr<Ship>> shipKV(shipId);
        if (!m_ships.find(shipKV)) {
            return StatusType::FAILURE;
        }
        Ship& ship = *m_ships.get(shipKV).getValue();
        if (ship.getPiratesNumber()<=0) {
            return StatusType::FAILURE;
        }
        return output_t<int> (ship.getRichestPirate());
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}

StatusType Ocean::ships_battle(int shipId1,int shipId2)
{
    if (shipId1 <= 0 || shipId2<=0 || shipId1==shipId2) {
        return StatusType::INVALID_INPUT;
    }
    try {
        KV<int,  std::shared_ptr<Ship>> shipKV1(shipId1);
        KV<int,  std::shared_ptr<Ship>> shipKV2(shipId2);

        if (!m_ships.find(shipKV1) || !m_ships.find(shipKV2) ) {
            return StatusType::FAILURE;
        }
        std::shared_ptr<Ship> ship1 = m_ships.get(shipKV1).getValue();
        std::shared_ptr<Ship> ship2 = m_ships.get(shipKV2).getValue();

        if(min(ship1->getPiratesNumber(),ship1->getCannons())>min(ship2->getPiratesNumber(),ship2->getCannons())){
            ship1->changeExtra(ship2->getPiratesNumber());
            ship2->changeExtra(-ship1->getPiratesNumber());
        } else if(min(ship1->getPiratesNumber(),ship1->getCannons())<min(ship2->getPiratesNumber(),ship2->getCannons())){
            ship2->changeExtra(ship1->getPiratesNumber());
            ship1->changeExtra(-ship2->getPiratesNumber());
        }
        return StatusType::SUCCESS;
    } catch (std::bad_alloc const& e) {
        return StatusType::ALLOCATION_ERROR;
    }
}



