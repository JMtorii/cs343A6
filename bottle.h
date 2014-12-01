#include "config.h"

#ifndef __BOTTLING_PLANT_H__
#define __BOTTLING_PLANT_H__

_Task Truck;

_Task BottlingPlant {
  public:
    _Event Shutdown {};                    // shutdown plant
    BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments );
    ~BottlingPlant();
    void getShipment( unsigned int cargo[] );
  private:
    enum State { STARTING = 'S', GENERATING_SODA = 'G', SHIPMENT_PICKED_UP = 'P', FINISHED = 'F' };
    Printer &prt;
    NameServer &nameServer;
    unsigned int numVendingMachines, maxShippedPerFlavour, maxStockPerFlavour, timeBetweenShipments;
    unsigned int stocks[ NUM_FLAVOURS ];
    int numRegVendingMachines;
    bool isDone;    // is this necessary
    Truck *truck;
    void createStock();
    void main();
};

#endif
