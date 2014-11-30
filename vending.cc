#include "nameserver.h"
#include "watcard.h"
#include "vending.h"

void Vending::main() {


}

VendingMachine::VendingMachine( Printer &prt, NameServer &nameServer, unsigned int id,
                                unsigned int sodaCost, unsigned int maxStockPerFlavour ) :
                                id(id), sodaCost(sodaCost), maxStockPerFlavour(maxStockPerFlavour) {
    // Register with name server
    nameServer.VMRegister( this );

    for ( unsigned int i = 0; i < Flavours.FLAVOUR_COUNT; ++i ) {
        flavourCount[ i ] = maxStockPerFlavour;
    }
};

void VendingMachine::buy( Flavours flavour, WATCard &card ) {

    // MUST USE FLAG VAIRABLE TO RAISE EXCEPTION ON CORRECT STACK
    
    /*
     * check if the passed watcard has sufficient funds to complete the transaction
     * raise "Funds" exception if not
     */
    if ( card.getBalance() < sodaCost ) {
        _Throw Funds();
    }

    /*
     * check if the desired soda is in stock,
     * raise "Stock" exception if not
     */
    if ( < sodaCost ) {
        _Throw Stock();
    }
}

unsigned int *inventory() {

}

/*
 * Signal the vending machine that restocking is complete
 */
void restocked() {

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


