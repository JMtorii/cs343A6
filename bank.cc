#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents( numStudents ) {
    // simple uCondition for deposit/withdraw
    conds = new uCondition[ numStudents ];

    // initialize accounts
    accounts = new unsigned int[ numStudents ];
    for ( unsigned int i = 0; i < numStudents; i++ ) accounts[ i ] = 0;
}

Bank::~Bank() {
    // delete all objects on the heap
    delete [] accounts;
    delete [] conds;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    // add money into account then signal
    accounts[ id ] += amount;
    conds[ id ].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    // wait until deposited then subtract
    while ( amount >= accounts[ id ] ) conds[ id ].wait();
    accounts[ id ] -= amount;
}

