#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "printer.h"
#include "nameserver.h"
#include "office.h"
#include "watcard.h"
#include "config.h"
#include "vending.h"
#include "MPRNG.h"

_Task Student {
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
  private:
    enum State { STARTING = 'S', SELECTING_MACHINE = 'V', BUYING = 'B', WATCARD_LOST = 'L', FINISHED = 'F' };
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    unsigned int id, numPurchases, favFlavourId;
    void main();
};

#endif
