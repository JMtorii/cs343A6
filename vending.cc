#include "nameserver.h"
#include "watcard.h"
#include "vending.h"
#include "printer.h"

void VendingMachine::main() {
    prt.print( Printer::Vending, ( char ) STARTING, sodaCost );
    
    // Register with name server
    nameServer.VMregister( this );

    for ( ;; ) {
        _Accept( ~VendingMachine ) {
            break;
        } or _When( !isRestocking ) _Accept( buy, inventory ) {
        } or _When ( isRestocking ) _Accept( restocked ) {
        } 
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
    if ( flavourStock[ flavour ] == 0 ) { uRendezvousAcceptor(); _Throw Stock(); }
    else if ( sodaCost > card.getBalance() ) { uRendezvousAcceptor();  _Throw Funds(); }
    
    prt.print( Printer::Vending, id, ( char ) BUY, flavour, --flavourStock[ flavour ]) ;
    card.withdraw( sodaCost );
}

unsigned int *VendingMachine::inventory() {
    isRestocking = true;
    prt.print( Printer::Vending, id, ( char ) RELOADSTART );
    return flavourStock; // return direct pointer to array of inventory
}

/*
 * Signal the vending machine that restocking is complete
 */
void VendingMachine::restocked() {
    isRestocking = false;
    prt.print( Printer::Vending, id, ( char ) RELOADSTOP );
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


