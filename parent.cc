#include "parent.h"
#include "config.h"

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
    prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {
    prt.print( Printer::Parent, ( char ) STARTING );
}

void Parent::main() {
    for ( ;; ) {
        _Accept( ~Parent ) {
            break;
        } _Else {
            yield( parentalDelay );
            int student = randomizer( numStudents - 1 );
            int amount = randomizer( MIN_PARENT_MONEY, MAX_PARENT_MONEY );
            prt.print( Printer::Parent, ( char ) DEPOSIT_GIFT, student, amount );
            bank.deposit( student, amount );
        }
    }

    prt.print( Printer::Parent, ( char ) FINISHED );
}
