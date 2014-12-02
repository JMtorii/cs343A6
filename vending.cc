#include "nameserver.h"
#include "watcard.h"
#include "vending.h"
#include "printer.h"
#include <iostream>

void VendingMachine::main() {
    prt.print( Printer::Vending, ( char ) STARTING, sodaCost );
    
    // Register with name server
    nameServer.VMregister( this );

    for ( ;; ) {
        _Accept( ~VendingMachine ) {
            break;
        } or _When( !isRestocking ) _Accept( buy, inventory ) {
            prt.print( Printer::Vending, id, ( char ) RELOADSTART );
        } or _When ( isRestocking ) _Accept( restocked ) {
            prt.print( Printer::Vending, id, ( char ) RELOADSTOP );
        } 

        // Print purchase message
        prt.print( Printer::Vending, ( char ) BUY, ( int ) flavourToBuy, flavourStock[ flavourToBuy ] );

    }

    prt.print( Printer::Vending, ( char ) FINISHED, sodaCost );
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,
                                unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
                                prt(prt), nameServer(nameServer), isRestocking(false), id(id), sodaCost(sodaCost), 
                                maxStockPerFlavour(maxStockPerFlavour), watcardUsed(NULL),
                                buyLock(0), truckLock(0), studentMutexLock(1), purchaseCompleteLock(0) {
    // machine is initially empty
    for ( unsigned int i = 0; i < NUM_FLAVOURS; ++i ) {
        flavourStock[ i ] = 0;
    }

    exceptionFlag = 0; // no exception should be raised by default
};

void VendingMachine::buy( Flavours flavour, WATCard &card ) {
    if ( flavourStock[ flavour ] == 0 ) _Throw Stock();
    else if ( sodaCost > card.getBalance() ) _Throw Funds();
    
    prt.print( Printer::Vending, id, ( char ) BUY, flavour, --flavourStock[ flavour ]) ;
    card.withdraw( sodaCost );
}

unsigned int *VendingMachine::inventory() {/*
    prt.print( Printer::Vending, ( char ) RELOADSTART, sodaCost );

    truckLock.P();        // do not process "buys" while restocking
*/
    isRestocking = true;
    return flavourStock; // return direct pointer to array of inventory
}

/*
 * Signal the vending machine that restocking is complete
 */
void VendingMachine::restocked() {/*
    truckLock.V(); // release the restocking lock
    prt.print( Printer::Vending, ( char ) RELOADSTOP, sodaCost ); */
    isRestocking = false;
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


