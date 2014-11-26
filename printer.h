#ifndef __PRINTER_H__
#define __PRINTER_H__

const unsigned int SINGLE_KIND_TOTAL = 5;

struct Data {
    unsigned int id;
    char state;
    int value1, value2;
    bool isFilled;
};

_Monitor Printer {
  public:
    enum Kind { Parent, WATCardOffice, NameServer, Truck, BottlingPlant, Student, Vending, Courier };
    Printer( unsigned int numStudents, unsigned int numVendingMachines, unsigned int numCouriers );
    ~Printer();
    void print( Kind kind, char state );
    void print( Kind kind, char state, int value1 );
    void print( Kind kind, char state, int value1, int value2 );
    void print( Kind kind, unsigned int lid, char state );
    void print( Kind kind, unsigned int lid, char state, int value1 );
    void print( Kind kind, unsigned int lid, char state, int value1, int value2 );
  private:
    unsigned int numStudents, numVendingMachines, numCouriers, totalSize;
    Data *printData;
    void clear();
    _Mutex void print( unsigned int id, char state, int value1, int value2 );
};

#endif
