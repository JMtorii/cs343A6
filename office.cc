#include "office.h"

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    prt( prt ), bank( bank), numCouriers( numCouriers ) {
    // initialize all objects and prepare courier
    job = NULL;
    isFinished = false;
    couriers = new Courier*[ numCouriers ];
    for ( unsigned int i = 0; i < numCouriers; i++ ) couriers[ i ] = new Courier( i, this, prt );
}

WATCardOffice::~WATCardOffice() {
    // delete all objects on the heap
    delete job;
    for ( unsigned int i = 0; i < numCouriers; i++ ) delete couriers [ i ];
    delete [] couriers;
}

void WATCardOffice::main() {
    // print out starting state
    prt.print( Printer::WATCardOffice, ( char ) STARTING ) ;

    for ( ; ; ) {
        // accept destructor and then requestWork for each courier
        _Accept( ~WATCardOffice ) {
            isFinished = true;
            for ( unsigned int i = 0; i < numCouriers; i++ ) _Accept( requestWork ); 
            break;
        
        // accept depending on job object
        } or _When( job != NULL ) _Accept( requestWork );
          or _When( job == NULL ) _Accept( create, transfer );   
    }

    // print out finished state
    prt.print( Printer::WATCardOffice, ( char ) FINISHED );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    // create new job from args, then return result
    job = new Job( Args( sid, amount, NULL, bank ));
    prt.print( Printer::WATCardOffice, ( char ) CREATE_CALL_COMPLETE, sid, amount );
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    // create new job for transferring to card, then return result
    job = new Job( Args( sid, amount, card, bank ));
    prt.print( Printer::WATCardOffice, ( char ) TRANSFER_CALL_COMPLETE, sid, amount );
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    // if office is finished, no need to request work
    if ( isFinished ) return NULL;
    
    // create temporary job to return
    Job* tmp = job;
    job = NULL;
    return tmp;
}

void WATCardOffice::Courier::main() {
    // print out courier starting state
    prt.print( Printer::Courier, id, ( char ) STARTING );
    
    for ( ;; ) {
        // create tmp job for requesting work
        Job *tmpJob = office->requestWork();
        
        // exit if no work is necessary
        if ( tmpJob == NULL ) break;
        Args args = tmpJob->args;

        // generate possibility to lose watcard then raise exception
        if ( randomizer( CHANCE_TO_LOSE_CARD - 1 ) == 0 ) {
            tmpJob->result.exception( new Lost );
            if ( args.watcard != NULL ) delete args.watcard;  // IS THIS NECESSARY?
        } else {
            // print out state for transfering funds
            prt.print( Printer::Courier, id, ( char ) START_FUND_TRANSFER, args.id, args.amount );
            
            // withdraw from bank then create new card
            args.bank.withdraw( args.id, args.amount );
            if ( args.watcard == NULL ) args.watcard = new WATCard();

            // deposit money into card
            args.watcard->deposit( args.amount );

            // print out finished transferring state then deliver to args watcard
            prt.print( Printer::Courier, id, ( char ) COMPLETE_FUND_TRANSFER, args.id, args.amount );
            tmpJob->result.delivery( args.watcard );
        }

        delete tmpJob;
    }
    
    // print finished state for courier
    prt.print( Printer::Courier, id, ( char ) FINISHED );
}
