#include"BigInt.h"
int main()
{
    BigInt bt, bt1, bt2;
    
    bt1.LoadData(100);
    cout<<"bt1 = ";
    bt1.PrintData();

    sleep(1);
    cout<<"bt2 = ";
    bt2.LoadData(100);
    bt2.PrintData();

    //bt = bt1 + bt2;
    //BigInt::Add(bt,bt1,bt2);
    //BigInt::Sub(bt,bt1,bt2);
    BigInt::Mul(bt,bt1,bt2);
    cout<<"bt = ";
    bt.PrintData();

    return 0;
}
