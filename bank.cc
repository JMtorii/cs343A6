#include "bank.h"

Bank::Bank( unsigned int numStudents ) : numStudents( numStudents ) {
    conds = new uCondition[ numStudents ];
    accounts = new int[ numStudents ];
    for ( unsigned int i = 0; i < numStudents; i++ ) accounts[ i ] = 0;
}

Bank::~Bank() {
    delete [] accounts;
    delete [] conds;
}

void Bank::deposit( unsigned int id, unsigned int amount ) {
    accounts[ id ] += amount;
    conds[ id ].signal();
}

void Bank::withdraw( unsigned int id, unsigned int amount ) {
    while ( amount >= accounts[ id ] ) conds[ id ].wait();
    accounts[ id ] -= amount;
}

