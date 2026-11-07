
#include <iostream>
#include <iomanip>
#include <string>
#include "../include/fchash.h"
using namespace std;

int main(){
    cout<<fixed<<setprecision(5);
    string x = "abcde@GHIJ:klmn&OPQR_stuv-WXYz";
    double r1 = fchash(x,5);
    double r2 = fchash(x,5);
    cout<<"\tr1: "<<r1<<"\n\tr2: "<<r2<<" = "<<(r1==r2)<<endl;

    return 0;
}
