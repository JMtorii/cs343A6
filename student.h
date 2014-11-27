#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "printer.h"
#include "nameserver.h"
#include "office.h"
#include "watcard.h"

_Task Student {
  public:
    Student( Printer &prt, NameServer &nameServer, WATCardOffice &cardOffice, unsigned int id,
             unsigned int maxPurchases );
  private:
    Printer &prt;
    NameServer &nameServer;
    WATCardOffice &cardOffice;
    unsigned int id, maxPurchases;
    void main();
};

#endif
