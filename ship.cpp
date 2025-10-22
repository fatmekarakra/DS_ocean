#include "ship.h"
#include "Pirate.h"
#include <iostream>
Ship::Ship(int shipId, int cannons): m_shipId(shipId), m_cannons(cannons), m_extra(0) {}

Ship::~Ship(){
}
void Ship::addPirate(Pirate& pirate){
    /*if(pirate.getPirateId()==38){
        std::cout<<"this is also pirate 38 his clean coins is "<<pirate.getTreasureClean()<<std::endl;
        std::cout<<"and this is the extra "<<m_extra<<std::endl;
    }*/
    pirate.setTreasure(-m_extra);
    /*if(pirate.getPirateId()==38){
        std::cout<<"this is also pirate 38 his clean coins is after adding the extra "<<pirate.getTreasureClean()<<std::endl;
    }*/
    Pair temp(pirate.getTreasureClean(),pirate.getPirateId());
    //m_piratesQueue.enqueue(pirate);
    auto pirateNode = m_piratesQueue.enqueue(pirate);
    pirate.setPiratesPos(pirateNode);
    /*if(pirate.getPirateId()==65){ 
       std::cout << "after enqueue!"<< std::endl;
    }
    if(pirate.getPirateId()==65){
        if(pirate.getPiratesPos()==nullptr){
            std::cout << "nullptr problem when inserting!!!"<< std::endl;
        }
    }*/
    KV<Pair,std::shared_ptr<Pirate>> pirateKV(temp,std::make_shared<Pirate>(pirate));
    /*if(pirate.getPirateId()==65){
        std::cout<<"before inserting into the tree"<<std::endl;
    }*/
    m_pirates.insert(pirateKV);
    /*if(pirate.getPirateId()==65){
        std::cout<<"after inserting into the tree and before finding the max value"<<std::endl;
    }*/
    KV<Pair,std::shared_ptr<Pirate>> maxKey=m_pirates.getMaxValue();
    Pair maxPair=maxKey.getKey();
    richestPirate=maxPair.second;
}
void Ship::removePirate(Pirate& pirate){
    /*if(pirate.getPirateId()==12){
        std::cout << pirate.getPirateId()<< std::endl;
        std::cout << pirate.getTreasureClean()<< std::endl;
        std::cout << "in remove of the ship  "<< std::endl;
    }*/
    /*std::cout << pirate.getPirateId()<< std::endl;
    std::cout << pirate.getTreasureClean()<< std::endl;
    std::cout << "in remove of the ship  "<< std::endl;*/
    Pair temp(pirate.getTreasureClean(),pirate.getPirateId());
    pirate.setTreasure(m_extra);
    KV<Pair,std::shared_ptr<Pirate>> pirateKV(temp);
    m_piratesQueue.removeValueInPosition(pirate.getPiratesPos());
    m_pirates.erase(pirateKV);
    /*if(pirate.getPirateId()==12){
        std::cout << "after erasing from the tree"<< std::endl;
        if(pirate.getPiratesPos()==nullptr){
            std::cout << "nullptr problem"<< std::endl;
        }
        std::cout << "ship data :"<< std::endl;
        std::cout << this->getShipId()<< std::endl;
        if(m_piratesQueue.isEmpty()){
            std::cout << "nullptr cause queue is empty!"<< std::endl;
        }
    }*/
    /*std::cout << "after erasing from the tree"<< std::endl;
    if(pirate.getPiratesPos()==nullptr){
        std::cout << "nullptr problem"<< std::endl;
    }*/
    /*if(pirate.getPirateId()==12){
        std::cout << "after erasing from the queue"<< std::endl;
    }*/
    //std::cout << "after erasing from the queue"<< std::endl;
    if(!(m_pirates.getSize()<=0)){
        KV<Pair,std::shared_ptr<Pirate>> maxKey=m_pirates.getMaxValue();
        Pair maxPair=maxKey.getKey();
        richestPirate=maxPair.second;
    }
}
void Ship::treason(Pirate& pirate){
    /*if(m_shipId==38){
        std::cout << "this is the traitors Id "<<pirate.getPirateId()<< std::endl;
        std::cout <<  "this is the coins we are looking for "<<pirate.getTreasureClean()<< std::endl;
        std::cout<<"the pirates tree in the ship:"<<std::endl;
        std::cout<<m_pirates<<std::endl;
        std::cout<<"the pirates Queue in the ship:"<<std::endl;
        std::cout<<m_piratesQueue<<std::endl;
    }*/
    Pair temp(pirate.getTreasureClean(),pirate.getPirateId());
    pirate.setTreasure(m_extra);
    KV<Pair,std::shared_ptr<Pirate>> pirateKV(temp);
    m_pirates.erase(pirateKV);
    m_piratesQueue.dequeue();
    if(!(m_pirates.getSize()<=0)){
        KV<Pair,std::shared_ptr<Pirate>> maxKey=m_pirates.getMaxValue();
        Pair maxPair=maxKey.getKey();
        richestPirate=maxPair.second;
    }
}
void Ship::pirateTreasureUpdate(Pirate& pirate,int change){
    /*if(pirate.getPirateId()==59){
        std::cout<<"clean treasure is "<<pirate.getTreasureClean()<<std::endl;
        std::cout<<"clean treasure - change is "<<pirate.getTreasureClean()-change<<std::endl;
        std::cout<<"Id is "<<pirate.getPirateId()<<std::endl;
        std::cout<<"piartes tree:- "<<std::endl;
        std::cout<<m_pirates<<std::endl;
    }*/
    Pair temp(pirate.getTreasureClean()-change,pirate.getPirateId());
    KV<Pair,std::shared_ptr<Pirate>> pirateKVOld(temp);
    m_pirates.erase(pirateKVOld);
    temp.first+=change;
    KV<Pair,std::shared_ptr<Pirate>> pirateKV(temp,std::make_shared<Pirate>(pirate));
    m_pirates.insert(pirateKV);
    //m_piratesQueue.changeValueInPos(pirate.getPiratesPos(),pirate);
    KV<Pair,std::shared_ptr<Pirate>> maxKey=m_pirates.getMaxValue();
    Pair maxPair=maxKey.getKey();
    richestPirate=maxPair.second;
}
Pirate* Ship::getOldestPirate(){
    return &m_piratesQueue.peak();
}
int Ship::getExtra(){
    return m_extra;
}
void Ship::changeExtra(int change){
    m_extra=m_extra+change;
}
int Ship::getCannons() const{
    return m_cannons;
}
int Ship::getPiratesNumber(){
    return m_pirates.getSize();
}
int Ship::getShipId(){
    return m_shipId;
}
int Ship::getRichestPirate(){
    return richestPirate;
}
std::ostream& operator<<(std::ostream& os, const Ship ship) {
    os<<"SId"<<ship.m_shipId<<" CNum: "<<ship.m_cannons<<" PiratesNumber "<<ship.m_pirates.getSize()<<" richest "<<ship.richestPirate;
    return os;
}