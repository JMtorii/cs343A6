#ifndef __WATCARD_H__
#define __WATCARD_H__

#include "watcard.h"
#include "bank.h"
#include "printer.h"
#include "office.h"
#include "MPRNG.h"


_Task WATCardOffice {
  public:
    _Event Lost {};                        // lost WATCard
    WATCardOffice( Printer &prt, Bank &bank, unsigned int numCouriers );
    ~WATCARDOffice();
    WATCard::FWATCard create( unsigned int sid, unsigned int amount );
    WATCard::FWATCard transfer( unsigned int sid, unsigned int amount, WATCard *card );
    Job *requestWork();
  private:
    enum State { STARTING = 'S', WORK_CALL_COMPLETE = 'W', CREATE_CALL_COMPLETE = 'C', 
        TRANSFER_CALL_COMPLETE = 'T', FINISHED = 'F' };
    
    struct Args {
        unsigned int id, amount;
        WATCard *watcard;
        Bank &bank;
        
        Args( unsigned int id, unsigned int amount, WATCard *watcard, Bank &bank );
    };
  
    struct Job {                           // marshalled arguments and return future
        Args args;                         // call arguments (YOU DEFINE "Args")
        WATCard::FWATCard result;          // return future
        
        Job( Args args );
    };
    
    _Task Courier {
      public:
        enum State { STARTING = 'S', START_FUND_TRANSFER = 't', COMPLETE_FUND_TRANSFER = 'T', FINISHED = 'F' };
        Courier( unsigned int id, WATCardOffice *watcardOffice, Printer &prt );
      private:
        unsigned int id;
        WATCardOffice *office;
        Printer &prt;
        void main();
    };                 // communicates with bank
    
    Printer &prt;
    Bank &bank;
    Courier **couriers;
    Job *job
    bool isFinished;
    void main();
};

#endif
