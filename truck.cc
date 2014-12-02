#include "vending.h"
#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottle.h"
#include "MPRNG.h"
#include "config.h"

/*
 * Helper function for determining if the truck is empty
 */
bool Truck::truckEmpty() {
    for ( int i = 0; i < NUM_FLAVOURS; ++i ) {
        if ( cargo[i] != 0 ) {
            return false;
        }
    }

    return true;
}

/*
 * Helper function for tallying truck cargo
 */
unsigned int Truck::tallyCargo() {
    int total = 0;
    for ( int i = 0; i < NUM_FLAVOURS; ++i ) {
        total += cargo[i];
    }

    return total;
}

void Truck::main() {
    // Obtain the location of each vending machine from the name server
    vendingMachines = nameServer.getMachineList();

    prt.print( Printer::Truck, ( char ) STARTING );

    unsigned int currentMachine = 0;

    for ( ;; ) {
        // Yield random number of times before each shipment
        yield( randomizer( 1, 10 ) );

        // Get shipment, throw out any old soda
        try {
            plant.getShipment( cargo );
        } catch ( BottlingPlant::Shutdown &e ) {
            break; // Shut down if the bottling plant is shutting down
        }
        prt.print( Printer::Truck, ( char ) PICKUP, tallyCargo() );

        // Distribute shipment across machines, rotate first machine for fairness
        for ( unsigned int i = 0; i < numVendingMachines && !truckEmpty(); ++i ) {

            // Unload as much shipment as possible
            unsigned int *machineContents = vendingMachines[currentMachine]->inventory();

            prt.print( Printer::Truck, ( char ) DELIVERYSTART, currentMachine, tallyCargo() );

            int unstocked = 0; // holds the number of bottles we were not able to restock
            for ( int j = 0; j < NUM_FLAVOURS; ++j ) {
                unsigned int sodaToTransfer;
                sodaToTransfer = maxStockPerFlavour - machineContents[ j ];
                if ( sodaToTransfer > cargo[j] ) {
                    sodaToTransfer = cargo[j];
                }

                // Remove necessary amount from cargo
                cargo[j] -= sodaToTransfer;

                // Add it to the machine
                machineContents[ j ] += sodaToTransfer;

                // Check how many we were not able to restock
                if ( machineContents[j] < maxStockPerFlavour ) {
                    unstocked += maxStockPerFlavour - machineContents[ j ];
                }
            }

            vendingMachines[currentMachine]->restocked(); // signal that we are done restocking

            // Print out "incomplete" message if applicable
            if ( unstocked > 0 ) {
                prt.print( Printer::Truck, ( char ) UNSUCCESS, currentMachine, unstocked );
            }

            prt.print( Printer::Truck, ( char ) DELIVERYSTOP, currentMachine, tallyCargo() );


            // Rotate starting machine to keep things fair, keeping track of last served between trips
            currentMachine += 1;
            currentMachine = currentMachine % numVendingMachines;
        }
    }

    prt.print( Printer::Truck, ( char ) FINISHED );
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
              unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
              prt(prt), nameServer(nameServer), plant(plant), numVendingMachines(numVendingMachines),
              maxStockPerFlavour(maxStockPerFlavour) {
    // Initialize cargo to empty
    for ( unsigned int i = 0; i < NUM_FLAVOURS; ++i ) {
        cargo[ i ] = 0;
    }
}

