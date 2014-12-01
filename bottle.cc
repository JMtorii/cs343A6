#include "bottle.h"

BottlingPlant::BottlingPlant( Printer &prt, NameServer &nameServer, unsigned int numVendingMachines,
                 unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
                 unsigned int timeBetweenShipments ) : 
                 prt( prt ), nameServer( nameServer ), numVendingMachines( numVendingMachines ), 
                 maxShippedPerFlavour( maxShippedPerFlavour ), maxStockPerFlavour( maxStockPerFlavour ), 
                 timeBetweenShipments( timeBetweenShipments ) {
    isDone = false;
    for ( int i = 0; i < NUM_FLAVOURS; i++ ) stocks[ i ] = 0;
    truck = new Truck( prt, nameServer, *this, numVendingMachines, maxStockPerFlavour );
}

BottlingPlant::~BottlingPlant() {
    delete truck;
}

void BottlingPlant::getShipment( unsigned int cargo[] ) {
    if ( isDone ) _Throw( new Shutdown );

    prt.print( Printer::BottlingPlant ( char ) SHIPMENT_PICKED_UP );
    for ( int i = 0; i < NUM_FLAVOURS; i++ ) {
        cargo[ i ] = stocks[ i ];
        stocks[ i ] = 0;
    } 
}

void BottlingPlant::createStock() {
    int totalCreated = 0;

    for ( int i = 0; i < NUM_FLAVOURS; i++ ) {
        int stockAmount = randomizer( maxShippedPerFlavour );
        stocks[ i ] += stockAmount;
        totalCreated += stockAmount;
    }

    prt.print( Printer::BottlingPlant, ( char ) GENERATING_SODA, totalCreated );
}

void BottlingPlant::main() {
    prt.print( Printer::BottlingPlant, ( char ) STARTING );
    createSotck();

    for ( ;; ) {
        _Accept( ~BottlingPlant ) {
            isDone = true;
            _Accept( getShipment );
            break;
        } or _Accept( getShipment ) {
            yield( timeBetweenShipments );
            createStock();
        }
    }

    prt.print( Printer::BottlingPlant, ( char ) FINISHED );
}
