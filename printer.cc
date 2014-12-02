#include "printer.h"
#include <iostream>
using namespace std;

Printer::Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers) : 
    numStudents( numStudents ), numVendingMachines( numVendingMachines ), numCouriers( numCouriers ) {
    
    // instantiate total number of size and held data to print
    totalSize = SINGLE_KIND_TOTAL + numStudents + numVendingMachines + numCouriers; 
    printData = new Data[ totalSize ];
    
    // print out header
    cout << "Parent\tWATOff\tNames\tTruck\tPlant\t";
    for( unsigned int i = 0; i < numStudents; i++ ) cout << "Stud" << i << '\t';
    for( unsigned int i = 0; i < numVendingMachines; i++ ) cout << "Mach" << i << '\t';
    for( unsigned int i = 0; i < numCouriers; i++ ) cout << "Cour" << i << '\t';
    
    cout << endl;
    
    for( unsigned int i = 0; i < totalSize; i++ ) printData[ i ].isFilled = false;
}

Printer::~Printer() {
    // clear then print last line
    clear();
    delete [] printData;
    cout << "***********************" << endl;
}

void Printer::print( Kind kind, char state ) {
    // forward calling
    print( kind, state, -1, -1 );
}

void Printer::print( Kind kind, char state, int value1 ) {
    // forward calling
    print( kind, state, value1, -1 );
}

void Printer::print( Kind kind, char state, int value1, int value2) {
    // forward calling
    print( kind, 0, state, value1, value2 );
}

void Printer::print( Kind kind, unsigned int lid, char state ) {
    // forward calling
    print( kind, lid, state, -1, -1 );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1 ) {
    // forward calling
    print( kind, lid, state, value1, -1 );
}

void Printer::print( Kind kind, unsigned int lid, char state, int value1, int value2 ) {
    int lid_offset = 0;    

    // depending on object, change the lid offset
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

    // print out result
    print( lid + lid_offset, state, value1, value2 );
}

void Printer::clear() {
    // until all objects are printed, continue printing object's state
    for( unsigned int i = 0; i < totalSize; i++ ) {
        
        // if filled, printo ut the state and value
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
    // if finished, clear all
    if( state == 'F' ) {
        for( unsigned int i = 0; i < totalSize; i++ ) {
            if( printData[ id ].isFilled ) {
                clear();
                break;
            }
        }
        
        // print either ... or state
        for( unsigned int i = 0; i < totalSize; i++ ) {
            if( id != i ) {
                cout << "...";
            } else {
                cout << state;
            }
            cout << '\t';
        }

        cout << endl;
    
    // if data is filled, clear data
    } else if( printData[ id ].isFilled ) {
        clear();
    }

    // if state is not finished, copy to printData
    if( state != 'F' ) {
        printData[ id ].state = state;
        printData[ id ].value1 = value1;
        printData[ id ].value2 = value2;
        printData[ id ].isFilled = true;
    }
}
