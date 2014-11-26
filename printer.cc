#include "printer.h"
#include <iostream>
using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) : 
    numStudents( numStudents ), numVendingMachines( numVendingMachines ), numCouriers( numCouriers ) {
    printData = new Data[ SINGLE_KIND_TOTAL + numStudents + numVendingMachines + numCouriers ];
    
    cout << "Parent\tWATOff\tNames\tTruck\tPlant\t";
    for( unsigned int i = 0; i < numStudents; i++ ) cout << "Stud" << i << '\t';
    for( unsigned int i = 0; i < numVendingMachines; i++ ) cout << "Mach" << i << '\t';
    for( unsigned int i = 0; i < numCouriers; i++ ) cout << "Cour" << i << '\t';
    
    cout << endl;
    
    for( unsigned int i = 0; i < totalSize; i++ ) printData[ i ].filled = false;
}

Printer::~Printer() {
    cout << "***********************" << endl;
}

void Printer::print( Kind kind, char state ) {
    print( kind, 0, state, -1, -1 );
}

void Printer::print( Kind kind, char state, int value1 ) {
    print( kind, 0, state, value1, -1 );
}

void Printer::print( Kind kind, char state, int value1, int value2) {
    print( kind, 0, state, value1, value2 );
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    print( kind, lid, state, -1, -1 );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    print( kind, lid, state, value1, -1 );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    int lid_offset = 0;    

    switch( kind ) {
      case Vending:
        lid_offset = SINGLE_KIND_TOTAL + numStudents;
        break;
      case Student:
        lid_offset = SINGLE_KIND_TOTAL;
        break;
      case Courier:
        lid_offset = SINGLE_KIND_TOTAL + numStudents + numVendingMachines;
        break;
      default:
        lid_offset = ( unsigned int ) kind;
        break;
    }

    print( lid + lid_offset, state, value1, value2 );
}

void Printer::clear() {
    for( unsigned int i = 0; i < totalSize; i++ ) {
        if( printData[ i ].isFilled ) {
            cout << printData[ i ].state;
            
            if( printData[ i ].value1 != -1 ) {
                cout << printData[i].value1;
                
                if( printData[ i ].value2 != -1 ) cout << "," << printData[ i ].value2;
            }
            
            printData[ i ].isFilled = false;
        }
        
        cout << '\t';
    }

    cout << endl;
}

_Mutex void Printer::print( unsigned int id, char state, int value1, int value2 ) {
    if( state == 'F' ) {
        for( unsigned int i = 0; i < totalSize; i++ ) {
            if( printData[ id ].isFilled ) {
                clear();
                break;
            }
        }

        for( unsigned int i = 0; i < totalSize; i++ ) {
            if( id != i ) {
                cout << "...";
            } else {
                cout << state;
            }
            cout << '\t';
        }

        cout << endl;
    } else if( printState[ id ].isFilled ) {
        clear();
    }

    if( state!= 'F' ) {
        printData[ id ].state = state;
        printData[ id ].value1 = value1;
        printData[ id ].value2 = value2;
        printData[ id ].isFilled = true;
    }
}
