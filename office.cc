#include "office.h"
WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    prt( prt ), bank( bank), numCouriers( numCouriers ) {
    job = NULL;
    isFinished = false;
    couriers = new Courier*[ numCouriers ];
    for ( unsigned int i = 0; i < numCouriers; i++ ) couriers[ i ] = new Courier( i, this, prt );
}

WATCardOffice::~WATCardOffice() {
    delete job;
    for ( unsigned int i = 0; i < numCouriers; i++ ) delete couriers [ i ];
    delete [] couriers;
}

void WATCardOffice::main() {
    prt.print( Printer::WATCardOffice, ( char ) STARTING ) ;

    for ( ; ; ) {
        _Accept( ~WATCardOffice ) {
            isFinished = true;
            for ( unsigned int i = 0; i < numCouriers; i++ ) _Accept( requestWork ); 
            break;
        } or _When( job != NULL ) _Accept( requestWork );
          or _When( job == NULL ) _Accept( create, transfer );   
    }

    prt.print( Printer::WATCardOffice, ( char ) FINISHED );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    job = new Job( Args( sid, amount, NULL, bank ));
    prt.print( Printer::WATCardOffice, ( char ) CREATE_CALL_COMPLETE, sid, amount );
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    job = new Job( Args( sid, amount, card, bank ));
    prt.print( Printer::WATCardOffice, ( char ) TRANSFER_CALL_COMPLETE, sid, amount );
    return job->result;
}

WATCardOffice::Job *WATCardOffice::requestWork() {
    if ( isFinished ) return NULL;
    
    Job* tmp = job;
    job = NULL;
    return tmp;
}

void WATCardOffice::Courier::main() {
    prt.print( Printer::Courier, id, ( char ) STARTING );
    
    for ( ;; ) {
        Job *tmpJob = office->requestWork();
        
        if ( tmpJob == NULL ) break;
        Args args = tmpJob->args;

        if ( randomizer( CHANCE_TO_LOSE_CARD - 1 ) == 0 ) {
            tmpJob->result.exception( new Lost );
            if ( args.watcard != NULL ) delete args.watcard;  // IS THIS NECESSARY?
        } else {
            prt.print( Printer::Courier, id, ( char ) START_FUND_TRANSFER, args.id, args.amount );
            args.bank.withdraw( args.id, args.amount );
            if ( args.watcard == NULL ) args.watcard = new WATCard();
            args.watcard->deposit( args.amount );
            prt.print( Printer::Courier, id, ( char ) COMPLETE_FUND_TRANSFER, args.id, args.amount );
            tmpJob->result.delivery( args.watcard );
        }

        delete tmpJob;
    }
    
    prt.print( Printer::Courier, id, ( char ) FINISHED );
}
