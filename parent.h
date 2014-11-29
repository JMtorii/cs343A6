#ifndef __PARENT_H__
#define __PARENT_H__

#include "printer.h"
#include "bank.h"
#include "MPRNG.h"

_Task Parent {
  public:
    Parent( Printer &prt, Bank &bank, unsigned int numStudents, unsigned int parentalDelay );
  private:
    enum State { STARTING = 'S', DEPOSIT_GIFT = 'D', FINISHED = 'F' };
    Printer &prt;
    Bank &bank;
    unsigned int numStudents, parentalDelay;
    void main();
};

#endif
