#include "nameserver.h"
#include "vending.h"
#include "printer.h"
#include <iostream>

NameServer::NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents ) : 
    prt( prt ), numVendingMachines( numVendingMachines ), numStudents( numStudents ), numRegVendingMachines( 0 ) {
    // create an assigner for the machine and create all vending machines
    machineAssigner = new int[ numStudents ];
    vendingMachines = new VendingMachine*[ numVendingMachines ];
    for ( unsigned int i = 0; i < numStudents; i++ ) machineAssigner[ i ] = i % numVendingMachines;
}

NameServer::~NameServer() {
    // delete all objects on the heap
    delete[] machineAssigner;
    delete[] vendingMachines;
}

void NameServer::VMregister( VendingMachine *vendingmachine ) {
    // get the vending machine, print registered state, then icnreate the number of registered vending machines
    int machId = vendingmachine->getId();
    prt.print( Printer::NameServer, ( char ) REGISTER_VENDING_MACHINE, machId );
    vendingMachines[ numRegVendingMachines++ ] = vendingmachine;
}

VendingMachine* NameServer::getMachine( unsigned int id ) {
    // get the machine by id
    VendingMachine *machine = vendingMachines[ machineAssigner[ id ]];
    
    // print state of new machine
    prt.print( Printer::NameServer, ( char ) NEW_VENDING_MACHINE, id, machine->getId() );
    
    // set assigner accordingly then return
    machineAssigner[ id ] = ( machineAssigner[ id ] + 1 ) % numVendingMachines;
    return machine;
}

VendingMachine** NameServer::getMachineList() {
    return vendingMachines;
}

void NameServer::main() {
    // print out starting state
    prt.print( Printer::NameServer, ( char ) STARTING );
    
    for ( ;; ) {
        // accept destructor then exit
        _Accept ( ~NameServer ) break;

        // when number of registered machines match, accept getMachine or getMachineList
        or _When ( numRegVendingMachines == numVendingMachines ) _Accept ( getMachine, getMachineList );
        
        // if they don't match, register
        or _When ( numRegVendingMachines < numVendingMachines ) _Accept ( VMregister );
    }
   
    // print out finished state 
    prt.print( Printer::NameServer, ( char ) FINISHED );
}

