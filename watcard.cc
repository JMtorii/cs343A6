#include "watcard.h"

WATCard::WATCard() {
    curBalance = 0;
}

void WATCard::deposit( unsigned int amount ) {
    curBalance += amount;
}

void WATCard::withdraw( unsigned int amount ) {
    curBalance -= amount;
}

unsigned int WATCard::getBalance() {
    return curBalance;
}

// do we need operator= ?
