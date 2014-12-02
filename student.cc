#include "student.h"

Student::Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id, unsigned int maxPurchases ) : 
    prt( prt ), nameServer( nameServer ), cardOffice( cardOffice ), id ( id ) {
    // generate random number of purchases for student
    numPurchases = randomizer( 1, maxPurchases );
    
    // fetch favourite flavour
    favFlavourId = randomizer ( NUM_FLAVOURS - 1 );
}

void Student::main() {
    // print start state
    prt.print( Printer::Student, id, ( char ) STARTING, favFlavourId, numPurchases );
    
    // create future watcard
    WATCard::FWATCard watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );

    // until all purchases are made, attempt purchases
    for ( unsigned int i = 0; i < numPurchases; i++ ) {
        // yeild before attempting to buy soda
        yield( randomizer( 1, 10 ) ); 
       
        // get selected vending machine then print results 
        VendingMachine *vendingMachine = nameServer.getMachine( id );
        prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, vendingMachine->getId() );
        bool isBought = false;
        
        // until item is bought, try to purchase
        while ( !isBought ) {

            // triple try statements to catch all possible exceptions
            try {
                try {
                    try {
                        // attempt to buy
                        vendingMachine->buy( ( VendingMachine::Flavours ) favFlavourId, *( watcard() ) );
                        
                        // upon successful purchase, move onto next purchase and print out state
                        isBought = true;
                        prt.print( Printer::Student, id, ( char ) BUYING, vendingMachine->getId() );
                    }
                    catch ( VendingMachine::Funds ) {
                        // transfer from card office
                        watcard = cardOffice.transfer( id, INITIAL_CARD_AMOUNT + vendingMachine->cost(), watcard() );
                    }
                } catch ( VendingMachine::Stock ) {
                    // indicate that machine is being selected
                    vendingMachine = nameServer.getMachine( id );
                    prt.print( Printer::Student, id, ( char ) SELECTING_MACHINE, vendingMachine->getId() );
                }
            } catch ( WATCardOffice::Lost &e ) {
                // indicate that watcard has been lost and a new one is required
                watcard = cardOffice.create( id, INITIAL_CARD_AMOUNT );
                prt.print( Printer::Student, id, ( char ) WATCARD_LOST, vendingMachine->getId() );
            } 
        }
    }

    // cleanup watcard object then print finished state
    delete watcard();
    prt.print( Printer::Student, id, ( char ) FINISHED );
}
