#include "config.h"

#include <uSemaphore.h>

#ifndef __VENDING_H__
#define __VENDING_H__

_Task NameServer;
_Monitor Printer;
class WATCard;

_Task VendingMachine {
  public:
    enum Flavours { CHERRY, CREAM, ROOT, LIME };
    _Event Funds {};                       // insufficient funds
    _Event Stock {};                       // out of stock for particular flavour
    VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id, unsigned int sodaCost,
                    unsigned int maxStockPerFlavour );
    void buy( Flavours flavour, WATCard &card );
    unsigned int *inventory();
    void restocked();
    _Nomutex unsigned int cost();
    _Nomutex unsigned int getId();
  private:
    enum State { STARTING = 'S', RELOADSTART = 'r', RELOADSTOP = 'R', BUY = 'B', FINISHED = 'F' };
    Printer& prt;
    NameServer& nameServer;
    bool isRestocking;  // MAY NOT BE NECESSARY
    char exceptionFlag; // 0 = no exception, 'f' = Funds, 's' = Stock
    unsigned int id, sodaCost, maxStockPerFlavour;
    unsigned int flavourStock[ NUM_FLAVOURS ];
    Flavours flavourToBuy;
    WATCard* watcardUsed;
    uSemaphore buyLock, truckLock, studentMutexLock, purchaseCompleteLock;
    void main();
};

#endif
