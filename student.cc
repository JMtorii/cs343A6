#include "student.h"
#include "MPRNG.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) : 
    prt( prt ), nameServer( nameServer ), cardOffice( cardOffice ), id ( id ) {
    numPurchases = randomizer( 1, maxPurchases );
    favFlavourId = randomizer ( NUM_FLAVOURS - 1 );
}

void Student::main() {
    prt.print( Printer::Student, id, ( char ) STARTING, favFlavourId, numPurchases );
    FWATCard watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );

    for ( unsigned int i = 0; i < numPurchases; i++ ) {
        yield( randomizer( 1, 10 ) ); 
        
        VendingMachine *vendingMachine = nameServer.getMachine( id );
        prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, machine->getId() );
        bool isBought = false;
        
        while ( !isBought ) {
            try {

                // TODO: Need to catch the states where the vending machine returns a Funds exception
                VendingMachine::Status status = machine->buy( ( VendingMachine::Flavours ) favFlavourId, *( watcard() ) );
                switch( status ) {
                    // MAY NEED TO CHANGE THESE STATES
                  case VendingMachine::BUY:
                    isBought = true;
                    prt.print( Printer::Student, id, ( char ) BUYING, machine->getId() );
                    break;
                  case VendingMachine::IN_STOCK:
                    vendingMachine = nameServer.getMachine( id );
                    prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, machine->getId() );
                    break;
                  case VendingMachine::FUNDING:
                    watcard = cardOffice.transfer( id, INITIAL_CARD_AMOUNT + machine->cost(), watcard() );
                    break;
                }
                
            } catch ( WATCardOffice::Lost &e ) {
                watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );
                prt.print( Printer::Student, id, ( char ) WATCARD_LOST, machine->getId() );
            } 
        }
    }
    delete watcard();
    prt.print( Printer::Student, id, ( char ) FINISHED );
}
