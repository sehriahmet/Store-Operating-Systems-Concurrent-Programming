#include <iostream>
enum ItemType { AAA, BBB, CCC };

#include "monitor.h"

class MarketStoreMonitor : public Monitor {
    int maxOrd;
    int cap[3];
    int avail[3];
    int reserved_supply[3];
    
    Condition can_buy;
    Condition not_sufficient_supply[3];

public:
    MarketStoreMonitor() : can_buy(this), not_sufficient_supply{Condition(this), Condition(this), Condition(this)} {

    }

    void initMarket(int cA, int cB, int cC, int mO) {
        __synchronized__;
        
        cap[AAA] = cA;
        cap[BBB] = cB;
        cap[CCC] = cC;
        
        maxOrd = mO;
        
        avail[AAA] = cA;
        avail[BBB] = cB;
        avail[CCC] = cC;

        reserved_supply[0] = 0; 
        reserved_supply[1] = 0;
        reserved_supply[2] = 0;
    }

    void buyM(int aA, int aB, int aC) {
        __synchronized__;

        if (aA > maxOrd || aB > maxOrd || aC > maxOrd) {
            // pthread_exit(NULL);
            return;
        }

        while (aA > avail[AAA] || aB > avail[BBB] || aC > avail[CCC]) {
            can_buy.wait();
        }

        if (aA <= avail[AAA]) avail[AAA] -= aA;
        if (aB <= avail[BBB]) avail[BBB] -= aB;
        if (aC <= avail[CCC]) avail[CCC] -= aC;

        not_sufficient_supply[AAA].notifyAll();
        not_sufficient_supply[BBB].notifyAll();
        not_sufficient_supply[CCC].notifyAll();

    }

    void maysupplyM(int itype, int n) {
        __synchronized__;
        
        if (n<0) return; 

        while (reserved_supply[itype] + avail[itype] + n > cap[itype]) {
            not_sufficient_supply[itype].wait(); 
        }

        reserved_supply[itype] += n;

    }
    
    void supplyM(int itype, int n) {
        __synchronized__;
        
        reserved_supply[itype] -= n;
        avail[itype] += n;
        can_buy.notifyAll();
        not_sufficient_supply[itype].notifyAll(); // this maybe nonsense but additional check 

    }
    
    void monitorStoreM(int c[3], int a[3]) {
        __synchronized__;
        
        for (int i = 0; i < 3; i++) {
            c[i] = cap[i];
            a[i] = avail[i];
        }
    }

};

MarketStoreMonitor marketStore;

void initStore(int cA, int cB, int cC, int mO) {
    marketStore.initMarket(cA,cB,cC,mO);

}

void buy(int aA, int aB, int aC) {
    marketStore.buyM(aA,aB,aC);
}

void maysupply(int itype, int n) {
    marketStore.maysupplyM(itype,n);
}

void supply(int itype, int n) {
    marketStore.supplyM(itype,n);
}

void monitorStore(int c[3], int a[3]) {
    marketStore.monitorStoreM(c,a);
}
