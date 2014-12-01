#include "student.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) : 
    prt( prt ), nameServer( nameServer ), cardOffice( cardOffice ), id ( id ) {
    numPurchases = randomizer( 1, maxPurchases );
    favFlavourId = randomizer ( NUM_FLAVOURS - 1 );
}

void Student::main() {
    prt.print( Printer::Student, id, ( char ) STARTING, favFlavourId, numPurchases );
    WATCard::FWATCard watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );

    for ( unsigned int i = 0; i < numPurchases; i++ ) {
        yield( randomizer( 1, 10 ) ); 
        
        VendingMachine *vendingMachine = nameServer.getMachine( id );
        prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, vendingMachine->getId() );
        bool isBought = false;
        
        while ( !isBought ) {
            try {
                try {
                    try {
                        vendingMachine->buy( ( VendingMachine::Flavours ) favFlavourId, *( watcard() ) );
                        isBought = true;
                        prt.print( Printer::Student, id, ( char ) BUYING, vendingMachine->getId() );
                    }
                    catch ( VendingMachine::Funds ) {
                        watcard = cardOffice.transfer( id, INITIAL_CARD_AMOUNT + vendingMachine->cost(), watcard() );
                    }
                } catch ( VendingMachine::Stock ) {
                    vendingMachine = nameServer.getMachine( id );
                    prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, vendingMachine->getId() );
                }
            } catch ( WATCardOffice::Lost &e ) {
                watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );
                prt.print( Printer::Student, id, ( char ) WATCARD_LOST, vendingMachine->getId() );
            } 
        }
    }
    delete watcard();
    prt.print( Printer::Student, id, ( char ) FINISHED );
}
