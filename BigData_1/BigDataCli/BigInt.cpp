#include"BigInt.h"
#include<stdlib.h>
#include<unistd.h>

BigInt::BigInt()
{}
BigInt::BigInt(u_long x)
{
    if(x == 0)
        big.push_back(0);
    while(x)
    {
        big.push_back(x%10);
        x /= 10;
    }
}
BigInt::BigInt(const BigInt &bt):big(bt.big)
{}
BigInt& BigInt::operator=(const BigInt &bt)
{
    if(this != &bt)
    {
        big = bt.big;
    }
    return *this;
}
BigInt::~BigInt()
{}
/////////////////////////////////////

void BigInt::LoadData(u_long sz)
{
    big.clear();
    srand(time(NULL));
    for(u_long i=0; i<sz; ++i)
    {
        big.push_back(rand() % 10);
    }
}
void BigInt::PrintData()const
{
    for(u_long i=big.size(); i>=1; --i)
    {
        cout<<(int)big[i];
    }
    cout<<endl;
}
void BigInt::Clear()
{
    big.clear();
}
void BigInt::BigIntCopy(char *buf, size_t len, size_t &pos)
{
    if(pos == 0)
    {
        memcpy(buf,big.base,1); //copy 0 index
        pos++;
    }
    memcpy(buf,big.base+pos, len);
}
//////////////////////////////////////////
