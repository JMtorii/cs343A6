#include "parent.h"
#include "config.h"

Parent::Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay ) :
    prt( prt ), bank( bank ), numStudents( numStudents ), parentalDelay( parentalDelay ) {
    // print starting state
    prt.print( Printer::Parent, ( char ) STARTING );
}

void Parent::main() {
    for ( ;; ) {
        // break if destructor is called
        _Accept( ~Parent ) {
            break;
        } _Else {
            // wait, pick student, pick amount
            yield( parentalDelay );
            int student = randomizer( numStudents - 1 );
            int amount = randomizer( MIN_PARENT_MONEY, MAX_PARENT_MONEY );
            
            // print out depositing state
            prt.print( Printer::Parent, ( char ) DEPOSIT_GIFT, student, amount );
            
            // deposit money to student
            bank.deposit( student, amount );
        }
    }

    // print out finished state for parent
    prt.print( Printer::Parent, ( char ) FINISHED );
}
