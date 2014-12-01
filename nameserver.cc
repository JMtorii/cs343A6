#include "nameserver.h"
#include "vending.h"
#include "printer.h"

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) : 
    prt( prt ), numVendingMachines( numVendingMachines ), numStudents( numStudents ) {
    machineAssigner = new int[ numStudents ];
    vendingMachines = new VendingMachine*[ numVendingMachines ];
    for ( unsigned int i = 0; i < numStudents; i++ ) machineAssigner[ i ] = i % numVendingMachines;
}

NameServer::~NameServer() {
    delete[] machineAssigner;
    delete[] vendingMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    int machId = vendingmachine->getId();
    prt.print( Printer::NameServer, ( char ) REGISTER_VENDING_MACHINE, machId );
    vendingMachines[ numRegVendingMachines++ ] = vendingmachine;
}

VendingMachine* NameServer::getMachine( unsigned int id ) {
    VendingMachine *machine = vendingMachines[ machineAssigner[ id ]];
    prt.print( Printer::NameServer, ( char ) NEW_VENDING_MACHINE, id, machine->getId() );
    machineAssigner[ id ] = ( machineAssigner[ id ] + 1 ) % numVendingMachines;
    return machine;
}

VendingMachine** NameServer::getMachineList() {
    return vendingMachines;
}

void NameServer::main() {
    prt.print( Printer::NameServer, ( char ) STARTING );
    
    for ( ;; ) {
        _Accept ( ~NameServer ) break;
        or _When ( numRegVendingMachines == numVendingMachines ) _Accept ( getMachine, getMachineList );
        or _When ( numRegVendingMachines < numVendingMachines ) _Accept ( VMregister );
    }
    
    prt.print( Printer::NameServer, ( char ) FINISHED );
}

