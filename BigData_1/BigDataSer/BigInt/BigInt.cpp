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
bool BigInt::operator==(const BigInt &bt)const
{
    if(size() != bt.size())
        return false;
    u_long i,j;
    i = size();
    j = bt.size();
    while(i>=1 && j>=1)
    {
        if(big[i] != bt[j])
            return false;
        i++;
        j++;
    }
    return true;
}
bool BigInt::operator!=(const BigInt &bt)const
{
    return !(*this==bt);
}

bool BigInt::operator<(const BigInt &bt)const
{
    if(size() < bt.size())
        return true;
    else if(size() > bt.size())
        return false;
    u_long i,j;
    i = this->size();
    j = bt.size();
    while(i>=1 && j>=1)
    {
        if(big[i] > bt[j])
            return false;
        else if(big[i] < bt[j])
            return true;
        i--;
        j--;
    }
    return false;
}

bool BigInt::operator>=(const BigInt &bt)const
{
    return !(*this < bt);
}

BigInt& BigInt::operator++()
{
    if(size() == 0)
    {
        big.push_back(1);
        return *this;
    }
    u_long i = 1;
    u_char sign = 1;
    u_char sum = 0;
    while(sign>=1 && i<=size())
    {
        big[i] = big[i] + sign;
        if(big[i] >= 10)
        {
            big[i] -= 10;
            sign = 1;
        }
        else
            sign = 0;
        i++;
    }
    if(sign >= 1)
        big.push_back(sign);
    return *this;
}

BigInt& BigInt::operator+=(const BigInt &bt)
{
    u_long i, j;
    i = j = 1;
    u_char sign = 0;
    u_char add;
    while(i<=size() && j<=bt.size())
    {
        big[i] = AddItem(big[i],bt[j],sign);
        i++;
        j++;
    }
    while(i<=size())
    {
        big[i] = AddItem(big[i],0,sign);
        i++;
    }
    while(j<=bt.size())
    {
        add = AddItem(0,bt[j],sign);
        big.push_back(add);
        j++;
    }
    if(sign >= 1)
        big.push_back(sign);
}

/////////////////////////////////////////
u_char BigInt::AddItem(u_char a, u_char b, u_char &sign)
{
    u_char sum = a + b + sign;
    /*
    if(sum >= 10)
    {
        sum -= 10;
        sign = 1;
    }
    else
        sign = 0;
        */
    sign = sum / 10;
    sum %= 10;
    return sum;
}

void BigInt::Add(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    bt.Clear();
    u_long i, j;
    i = j = 1;
    u_char sum;
    u_char sign = 0;
    while(i<=bt1.size() && j<=bt2.size())
    {
        sum = AddItem(bt1.big[i++],bt2.big[j++],sign);
        bt.big.push_back(sum);
    }
    while(i<=bt1.size())
    {
        sum = AddItem(bt1.big[i++], 0, sign);
        bt.big.push_back(sum);
    }
    while(j<=bt2.size())
    {
        sum = AddItem(0, bt2.big[j++], sign);
        bt.big.push_back(sum);
    }
    if(sign >= 1)
        bt.big.push_back(sign);
}

u_char BigInt::SubItem(u_char a, u_char b, u_char &sign)
{
    u_char sub;
    if(a >= b+sign)
    {
        sub = a-b-sign;
        sign = 0;
    }
    else
    {
        sub = a+10-b-sign;
        sign = 1;
    }
    return sub;
}
void BigInt::Sub(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    bt.Clear();
    assert(bt1 >= bt2);
    if(bt1 == bt2)
    {
        bt.push_back(0);
        return;
    }
    u_long i,j;
    i = j = 1;
    u_char sign = 0;
    u_char sub;
    while(i<=bt1.size() && j<=bt2.size())
    {
        sub = SubItem(bt1[i++],bt2[j++],sign);
        bt.push_back(sub);
    }
    while(i <= bt1.size())
    {
        sub = SubItem(bt1[i++],0,sign);
        bt.push_back(sub);
    }
}

void BigInt::MulItem(BigInt &bt, const BigInt &bt1, u_char x)
{
    u_char res;
    u_char sign = 0;
    for(u_long i=1; i<=bt1.size(); ++i)
    {
        res = bt1[i] * x + sign;
        sign = res / 10;
        res %= 10;
        bt.push_back(res);
    }
    if(sign > 0)
        bt.push_back(sign);
}
void BigInt::MoveAdd(BigInt &bt, const BigInt &tmp, u_long pos)
{
    u_long i, j;
    i = pos;
    j = 1;
    u_char sign = 0;
    while(i<=bt.size() && j<=tmp.size())
    {
        bt[i] = AddItem(bt[i],tmp[j],sign);
        i++;
        j++;
    }
    while(sign>0 && i<=bt.size())
    {
        bt[i] = AddItem(bt[i],0,sign);
        i++;
    }
    while(j<=tmp.size())
    {
        u_char add = AddItem(0,tmp[j],sign);
        bt.push_back(add);
        j++;
    }
    if(sign >= 1)
        bt.push_back(sign);
}
void BigInt::Mul(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    bt.Clear();
    BigInt tmp;
    for(u_long i=1; i<=bt2.size(); ++i)
    {
        tmp.Clear();
        MulItem(tmp,bt1,bt2[i]);
        MoveAdd(bt,tmp,i);        
    }
}

#if 0
void BigInt::Mul(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    for(BigInt i=0; i<bt2; ++i)
    {
        bt += bt1;
    }
#if 0
    BigInt tmp(0)
    for(BigInt i=0; i<bt2; ++i)
    {
        bt.Clear();
        //bt += bt1;
        //i.PrintData();
        BigInt::Add(bt,bt1,tmp);
        tmp = bt;
    }
#endif
}
#endif



