#ifndef __WATCARD_H__
#define __WATCARD_H__

class WATCard {
  public:
    WATCard();
    typedef Future_ISM<WATCard *> FWATCard; // future watcard pointer
    void deposit( unsigned int amount );
    void withdraw( unsigned int amount );
    unsigned int getBalance();
  private:
    WATCard( const WATCard & );            // prevent copying
    WATCard &operator=( const WATCard & );
    unsigned int curBalance;
};

#endif
