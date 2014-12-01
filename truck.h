#include "config.h"

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;
_Task VendingMachine;

_Task Truck {
    void main();
    bool truckEmpty();
    unsigned int tallyCargo();
    Printer &prt;
    BottlingPlant &plant;
    unsigned int numVendingMachines, maxStockPerFlavour;
    VendingMachine** vendingMachines;
    unsigned int cargo[ NUM_FLAVOURS ];
    enum State { STARTING = 'S', PICKUP = 'P', DELIVERYSTART = 'd', UNSUCCESS = 'U', DELIVERYSTOP = 'D', FINISHED = 'F' };
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

