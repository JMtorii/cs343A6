#ifndef __CONFIG_H__
#define __CONFIG_H__

#define NUM_FLAVOURS 4
#define INITIAL_CARD_AMOUNT 5
#define CHANCE_TO_LOSE_CARD 6 
#define MIN_PARENT_MONEY 1
#define MAX_PARENT_MONEY 3
#define SINGLE_KIND_TOTAL 5


struct ConfigParms {
    unsigned int sodaCost;                  // MSRP per bottle
    unsigned int numStudents;               // number of students to create
    unsigned int maxPurchases;              // maximum number of bottles a student purchases
    unsigned int numVendingMachines;        // number of vending machines
    unsigned int maxStockPerFlavour;        // maximum number of bottles of each flavour stocked
    unsigned int maxShippedPerFlavour;      // number of bottles of each flavour in a shipment
    unsigned int timeBetweenShipments;      // length of time between shipment pickup
    unsigned int parentalDelay;             // length of time between cash deposits
    unsigned int numCouriers;               // number of couriers in the pool
};

void processConfigFile( const char *configFile, ConfigParms &cparms );

#endif
