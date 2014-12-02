#include <iostream>
#include "config.h"
#include "MPRNG.h"
#include "printer.h"
#include "bank.h"
#include "parent.h"
#include "office.h"
#include "nameserver.h"
#include "vending.h"
#include "bottle.h"
#include "student.h"

using namespace std;

MPRNG randomizer;

void usage() {
    cerr << "Usage: soda [ config-file [ random-seed ] ]" << endl;
    exit( 1 );
}

void uMain::main() {
    int seed = getpid();
    string configFile = "soda.config";

    switch ( argc ) {
      case 3:
        seed = atoi( argv[ 2 ] ); 
        if ( seed <= 0 ) usage();
        break;
      case 2:
        configFile = string( argv[ 1 ] );
        break;
      case 1:
        break;
      default:
        usage();
        break;
    }

    randomizer.seed( seed );

    ConfigParms params; 
    processConfigFile( configFile.c_str(), params );
    Printer printer( params.numStudents, params.numVendingMachines, params.numCouriers );
    Bank bank( params.numStudents );
    Parent *parent = new Parent( printer, bank, params.numStudents, params.parentalDelay );
    WATCardOffice *watCardOffice = new WATCardOffice( printer, bank, params.numCouriers );
    NameServer *nameServer = new NameServer( printer, params.numVendingMachines, params.numStudents );
    BottlingPlant *bottlingPlant = new BottlingPlant( printer, *nameServer, params.numVendingMachines, params.maxShippedPerFlavour, params.maxStockPerFlavour, params.timeBetweenShipments );
    VendingMachine *vendingMachines[ params.numVendingMachines ];

    for ( unsigned int i = 0; i < params.numVendingMachines; i++ )
        vendingMachines[ i ] = new VendingMachine( printer, *nameServer, i, params.sodaCost, params.maxStockPerFlavour);

    Student *students[ params.numStudents ];
    for ( unsigned int i = 0; i < params.numStudents; i++ )
        students[ i ] = new Student( printer, *nameServer, *watCardOffice, i, params.maxPurchases );

    for ( unsigned int i = 0; i < params.numStudents; i++ ) 
        delete students[ i ];

    for ( unsigned int i = 0; i < params.numVendingMachines; i++ )
        delete vendingMachines[ i ];

    delete bottlingPlant;
    delete nameServer;
    delete watCardOffice;
    delete parent;
}
