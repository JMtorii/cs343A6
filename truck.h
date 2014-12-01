#include "vending.h" // include vending to get number of flavours

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
    void main();
    bool truckEmpty();
    Printer &prt;
    BottlingPlant plant;
    unsigned int numVendingMachines. maxStockPerFlavour;
    VendingMachine** vendingMachines;
    unsigned int cargo[ VendingMachine::FLAVOUR_COUNT ];
  public:
    Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
           unsigned int numVendingMachines, unsigned int maxStockPerFlavour );
};

