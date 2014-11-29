#include "watoffice.h"

WATCardOffice::WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers ) : 
    prt( prt ), bank( bank), numCouriers( numCouriers ) {
    job = NULL;
    isFinished = false;
    couriers = new Couriers*[ numCouriers ];
    for ( unsigned int i = 0; i < numCouriers; i++ ) couriers[ i ] = new Couriers( i, this, prt );
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
        } or _When( job != NULL ) _Accept( requestWord );
          or _When( job == NULL ) _Accept( create, trasfer );   
    }

    prt.print( Printer::WATCardOffice, ( char ) FINISHED );
}

WATCard::FWATCard WATCardOffice::create( unsigned int sid, unsigned int amount ) {
    job = new Job( Args( bank, sid, NULL, amount ));
    prt.print( Printer::WATCardOffice, ( char ) CREATE_CALL_COMPLETE, sid, amount );
    return job->result;
}

WATCard::FWATCard WATCardOffice::transfer( unsigned int sid, unsigned int amount, WATCard *card ) {
    job = new Job( Args( bank, sid, card, amount ));
    prt.print( Printer::WATCardOffice, ( char ) TRANSFER_CALL_COMPLETE, sid, amount );
    return job->result;
}

Job *WATCardOffice::requestWork() {
    if ( isFinished ) return NULL;
    
    Job* tmp = job;
    job = NULL;
    return tmp;
}

WATCardOffice::Args( unsigned int id, unsigned int amount, WATCard *watcard, Bank &bank ) :
    Args::id( id ), Args::bank( bank ), Args::watcard( watcard ), Args::amount( amount ) {}


WATCardOffice::Job( Args args ) : Job::args( args ) {}



WATCardOffice::Courier( unsigned int id, WATCardOffice *watcardOffice, Printer &prt ) : 
    Courier::id( id ), Courier::watcardOffice( watcardOffice ), Courier::prt( prt ) {}

void WATCardOffice::Courier::main() {
    prt.main( Printer::Courier, id, ( char ) STARTING );
    
    for ( ; ; ) {
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
            args.card->deposit( args.amount );
            prt.print( Printer::Courier, id, ( char ), COMPLETE_FUND_TRANSFER, args.id, args.amount );
            job->result.delivery( args.watcard );
        }

        delete tmpJob;
    }
    
    prt.print( Printer::Courier, id, ( char ) FINISHED );
}
