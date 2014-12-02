#include "watcard.h"

WATCard::WATCard() : curBalance( 0 ) {}

void WATCard::deposit( unsigned int amount ) {
    // add moeny
    curBalance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    // remove money
    curBalance -= amount;
}

unsigned int WATCard::getBalance() {
    // return the current balance on the watcard
    return curBalance;
}

// do we need operator= ?
