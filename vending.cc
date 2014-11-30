#include "nameserver.h"
#include "watcard.h"
#include "vending.h"

void Vending::main() {
    for ( ;; ) {
        truckLock.V(); // ensure "buy" operation is not interrupted by restocking
        buyLock.P(); // wait until purchase before running
        truckLock.P();

        /*
         * Perform input validation, and set exception flag according to result
         */
        if ( watcardUsed.getBalance() < sodaCost ) {
            exceptionFlag = 'f'; // A value of 'f' implies that Funds should be thrown by the task
        }
        else if ( flavourStock[ flavourToBuy ] == 0 ) {
            exceptionFlag = 's'; // A value of 's' implies that Stock should be thrown by the task
        } else {
            exceptionFlag = 0; // No exception, handle purchase
            watcardUsed.withdraw( sodaCost ); // process payment
            flavourStock[ flavourToBuy ] -= 1; // decrement stock
        }

        purchaseCompleteLock.V();
    }
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,
                                unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
                                id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
    // Register with name server
    nameServer.VMRegister( this );

    // machine is initially empty
    for ( unsigned int i = 0; i < Flavours.FLAVOUR_COUNT; ++i ) {
        flavourCount[ i ] = 0;
    }

    exceptionFlag = 0; // no exception should be raised by default
};

void VendingMachine::buy( Flavours flavour, WATCard &card ) {

    // Do not proceed if there are other students purchasing
    studentMutexLock.P();

    flavourToBuy = flavour;
    watcardUsed = card;

    buyLock.V(); // perform purchase in main()
    purchaseCompleteLock.P(); // wait until purchase completes

    studentMutexLock.V(); // allow other buys to proceed

    // Handle any exceptions which may have been triggered
    if ( exceptionFlag == 'f' ) {
        _Throw Funds();
    } else if ( exceptionFlag == 's' ) {
        _Throw Stock();
    }
}

unsigned int *inventory() {
    truckLock.P(); // do not process "buys" while restocking

    return &flavourCount; // return direct pointer to array of inventory
}

/*
 * Signal the vending machine that restocking is complete
 */
void restocked() {
    truckLock.V(); // release the restocking lock
}


/*
 * Returns the cost of soda from this Vending Machine
 */
unsigned int VendingMachine::cost() {
    return sodaCost;
}

/*
 * Returns the ID of this Vending Machine
 */
unsigned int VendingMachine::getId() {
    return id;
}


