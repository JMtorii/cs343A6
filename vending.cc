#include "nameserver.h"
#include "watcard.h"
#include "vending.h"
#include "printer.h"

void VendingMachine::main() {
    prt.print( Printer::Vending, ( char ) STARTING, sodaCost );
    
    // Register with name server
    nameServer.VMregister( this );

    for ( ;; ) {
        while ( 1 ) { }
        _Accept( ~VendingMachine ) {
            break;
        } or _When( !isRestocking ) _Accept( buy, inventory ) {
            prt.print( Printer::Vending, id, ( char ) RELOADSTART );
        } or _When ( isRestocking ) _Accept( restocked ) {
            prt.print( Printer::Vending, id, ( char ) RELOADSTOP );
        } 

        /*
         * Perform input validation, and set exception flag according to result
         */
    /*    if ( watcardUsed->getBalance() < sodaCost ) {
            exceptionFlag = 'f';               // A value of 'f' implies that Funds should be thrown by the task
        }
        else if ( flavourStock[ flavourToBuy ] == 0 ) {
            exceptionFlag = 's';               // A value of 's' implies that Stock should be thrown by the task
        } else {
            exceptionFlag = 0;                 // No exception, handle purchase
            watcardUsed->withdraw( sodaCost );  // process payment
            flavourStock[ flavourToBuy ] -= 1; // decrement stock
        }
*/
        // Print purchase message
        prt.print( Printer::Vending, ( char ) BUY, ( int ) flavourToBuy, flavourStock[ flavourToBuy ] );

    }

    // TODO: Ensure that main gets terminated gracefully, so this gets printed?
    prt.print( Printer::Vending, ( char ) FINISHED, sodaCost );
}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,
                                unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
                                prt(prt), nameServer(nameServer), id(id), sodaCost(sodaCost), 
                                maxStockPerFlavour(maxStockPerFlavour), watcardUsed(NULL),
                                buyLock(0), truckLock(0), studentMutexLock(1), purchaseCompleteLock(0) {
    // machine is initially empty
    for ( unsigned int i = 0; i < NUM_FLAVOURS; ++i ) {
        flavourStock[ i ] = 0;
    }

    exceptionFlag = 0; // no exception should be raised by default
};

void VendingMachine::buy( Flavours flavour, WATCard &card ) {

    // Do not proceed if there are other students purchasing
    /*studentMutexLock.P();

    flavourToBuy = flavour;
    watcardUsed = &card;

    buyLock.V();                // perform purchase in main()
    purchaseCompleteLock.P();   // wait until purchase completes
    studentMutexLock.V();       // allow other buys to proceed

    // Handle any exceptions which may have been triggered
    if ( exceptionFlag == 'f' ) {
        _Throw Funds();
    } else if ( exceptionFlag == 's' ) {
        _Throw Stock();
    }*/
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


