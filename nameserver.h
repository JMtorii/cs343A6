#ifndef __NAMESERVER_H__
#define __NAMESERVER_H__

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
  public:
    NameServer( Printer &prt, unsigned int numVendingMachines, unsigned int numStudents );
    ~NameServer();
    void VMregister( VendingMachine *vendingmachine );
    VendingMachine *getMachine( unsigned int id );
    VendingMachine **getMachineList();
  private:
    Printer &prt;
    unsigned int numVendingMachines, numStudents
    void main();
};

#endif
