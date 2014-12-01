#include "truck.h"
#include "printer.h"
#include "nameserver.h"
#include "bottle.h"

/*
 * Helper function for determining if the truck is empty
 */
bool Truck::truckEmpty( ) {
    for ( int i = 0; i < VendingMachine::FLAVOUR_COUNT; ++i ) {
        if ( cargo[i] != 0 ) {
            return false;
        }
    }

    return true;
}

void Truck::main() {
    unsigned int currentMachine = 0;

    for ( ;; ) {
        // Yield random number of times before each shipment
        yield( randomizer( 1, 10 ) );

        // Get shipment, throw out any old soda
        try {
            plant.getShipment( &cargo );
        } catch ( BottlingPlant::Shutdown &e ) {
            break; // Shut down if the bottling plant is shutting down
        }

        // Distribute shipment across machines, rotate first machine for fairness
        for ( int i = 0; i < numVendingMachines && !truckEmpty(); ++i ) {

            // Unload as much shipment as possible
            unsigned int *machineContents = vendingMachines[currentMachine].inventory();
            for ( int j = 0; j < VendingMachine::FLAVOUR_COUNT; ++j ) {
                unsigned int sodaToTransfer;
                sodaToTransfer = maxStockPerFlavour - (*machineContents)[ j ];
                if ( sodaToTransfer > cargo[j] ) {
                    sodaToTransfer = cargo[j];
                }

                // Remove necessary amount from cargo
                cargo[j] -= sodaToTransfer;

                // Add it to the machine
                (*machineContents)[ j ] += sodaToTransfer;
            }
            VendingMachines[currentMachine].restocked(); // signal that we are done restocking


            // Rotate starting machine to keep things fair, keeping track of last served between trips
            currentMachine += 1;
            currentMachine = currentMachine % numVendingMachines;
        }
    }
}

Truck::Truck( Printer &prt, NameServer &nameServer, BottlingPlant &plant,
              unsigned int numVendingMachines, unsigned int maxStockPerFlavour ) :
              prt(prt), plant(plant), numVendingMachines(numVendingMachines),
              maxStockPerFlavour(maxStockPerFlavour) {

    // Obtain the location of each vending machine from the name server
    vendingMachines = nameServer.getMachineList();

    // Initialize cargo to empty
    for ( unsigned int i = 0; i < VendingMachine::FLAVOUR_COUNT; ++i ) {
        cargo[ i ] = 0;
    }
}

