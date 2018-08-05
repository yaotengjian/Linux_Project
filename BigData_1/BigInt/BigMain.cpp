#include"BigInt.h"

int main()
{
    //BigInt bt, bt1(44), bt2(4), bt3(44);
    BigInt bt, bt1, bt2, bt3;
    bt1.LoadData(3);
    cout<<"bt1 = "<<bt1<<endl;
    bt2.LoadData(1);
    cout<<"bt2 = "<<bt2<<endl;
    bt3.LoadData(2);
    cout<<"bt3 = "<<bt3<<endl;

    BigInt::PowMod(bt, bt1, bt2, bt3);
    //BigInt::Power(bt,bt1,bt2);
    cout<<"bt = "<<bt<<endl;
    return 0;
}

/*
int main()
{
    BigInt bt;
    BigInt b1(5);
    cout<<"b1 = "<<b1<<endl;
    BigInt b2(7);
    cout<<"b2 = "<<b2<<endl;
    bt = b1 - b2;
    bt.PrintData();
    return 0;
}

/*
int main()
{
    BigInt bt;
    BigInt bt1(5);
    BigInt bt2(3);
    BigInt::Power(bt, bt1, bt2);
    cout<<bt<<endl;
    return 0;
}

/*
int main()
{
    BigInt bt, bt1, bt2;
    
    bt1.LoadData(1500);
    cout<<"bt1 = ";
    bt1.PrintData();

    sleep(1);
    cout<<"bt2 = ";
    bt2.LoadData(500);
    bt2.PrintData();

    //bt = bt1 + bt2;
    //BigInt::Add(bt,bt1,bt2);
    //BigInt::Sub(bt,bt1,bt2);
    //BigInt::Mul(bt,bt1,bt2);
    BigInt::Div(bt,bt1,bt2);
    cout<<"bt = ";
    bt.PrintData();

    return 0;
}
*/
