#include "config.h"
#include "printer.h"
#include "nameserver.h"
#include "MPRNG.h"
#include "bottle.h"
#include "truck.h"

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) : 
                 prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), 
                 maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), 
                 timeBetweenShipments( timeBetweenShipments ) {
    // initialize done state flag, stock for machine, and new truck
    isDone = false;
    for ( int i = 0; i < NUM_FLAVOURS; i++ ) stocks[ i ] = 0;
    truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
}

BottlingPlant::~BottlingPlant() {
    // delete truck instance
    delete truck;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    // send out exception to inform system that 
    if ( isDone ) {
        uRendezvousAcceptor();    
        _Throw( Shutdown() );
    }

    // print out current picked up state
    prt.print( Printer::BottlingPlant, ( char ) SHIPMENT_PICKED_UP );
    
    // set up cargo and put stocks back to initialized state
    for ( int i = 0; i < NUM_FLAVOURS; i++ ) {
        cargo[ i ] = stocks[ i ];
        stocks[ i ] = 0;
    } 
}

void BottlingPlant::createStock() {
    int totalCreated = 0;

    // create new stock for each flavour
    for ( int i = 0; i < NUM_FLAVOURS; i++ ) {
        int stockAmount = randomizer( maxShippedPerFlavour );
        stocks[ i ] += stockAmount;
        totalCreated += stockAmount;
    }

    // print generating state
    prt.print( Printer::BottlingPlant, ( char ) GENERATING_SODA, totalCreated );
}

void BottlingPlant::main() {
    // print out starting state
    prt.print( Printer::BottlingPlant, ( char ) STARTING );
    
    // create soda stock
    createStock();

    for ( ;; ) {
        // Accept destructor or getShipment
        _Accept( ~BottlingPlant ) {
            isDone = true;
            _Accept( getShipment );
            break;
        } or _Accept( getShipment ) {
            // yield between the time between shipments then create stock
            yield( timeBetweenShipments );
            createStock();
        }
    }

    // print finished state
    prt.print( Printer::BottlingPlant, ( char ) FINISHED );
}
