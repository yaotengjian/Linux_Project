#include"BigInt.h"
#include<stdlib.h>
#include<unistd.h>

ostream& operator<<(ostream &out, const BigInt &bt)
{
    for(u_long i=bt.size(); i>=1; --i)
    {
        out<<(int)bt[i];
    }
    return out;
}

/////////////////////////////////////////////////////////
BigInt::BigInt()
{}
BigInt::BigInt(long v)
{
    u_long x;
    if(v == 0)
    {
        big.push_back(0);
        return;
    }
    if(v < 0)
    {
        x = abs(v);
        set_symbol(NEGATIVE);
    }
    else
        x = v;
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
    ClearHeadZero();
}
void BigInt::PrintData()const
{
    if(get_symbol() == 1)
        cout<<"-";
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
void BigInt::ClearHeadZero()
{
    for(u_long i=big.size(); i>0; --i)
    {
        if(big[i] == 0)
            big.pop_back();
        else
            break;
    }
}
////////////////////////////////////////////////////////////////////
BigInt BigInt::operator+(const BigInt &bt)
{
    BigInt tmp;
    BigInt::Add(tmp,*this,bt);
    return tmp;
}
BigInt BigInt::operator-(const BigInt &bt)
{
    BigInt tmp;
    BigInt::Sub(tmp,*this,bt);
    return tmp;
}
BigInt BigInt::operator*(const BigInt &bt)
{
    BigInt tmp;
    BigInt::Mul(tmp,*this,bt);
    return tmp;
}
BigInt BigInt::operator/(const BigInt &bt)
{
    BigInt tmp;
    BigInt::Div(tmp,*this,bt);
    return tmp;
}
BigInt BigInt::operator^(const size_t n)
{
    if(n == 0)
        return BigInt(1);
    if(n == 1)
        return *this;

    BigInt bt;
    bt.Clear();
    if(n == 2)
        Square(bt, *this);
    else
    {
        BigInt tmp(1);
        for(int i=0; i<n; ++i)
        {
            Mul(bt, tmp, *this);
            tmp = bt;
        }
    }
    return bt;
}

////////////////////////////////////////////////////////////////////
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
BigInt BigInt::operator++(int)
{
    BigInt tmp = *this;
    ++*this;
    return tmp;
}
BigInt& BigInt::operator--()
{
    u_long i = 1;
    u_char sign = 0;
    big[i] = SubItem(big[i],1,sign);
    ++i;
    while(sign>0 && i<=size())
    {
        big[i] = SubItem(big[i],0,sign);
        i++;
    }
    ClearHeadZero();
    return *this;
}
BigInt BigInt::operator--(int)
{
    BigInt tmp = *this;
    --*this;
    return tmp;
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

BigInt& BigInt::operator-=(const BigInt &bt)
{
    u_long i, j;
    i = j = 1;
    u_char sign = 0;
    while(i<=big.size() && j<=bt.size())
    {
        big[i] = SubItem(big[i],bt[j],sign);
        i++;
        j++;
    }
    while(sign>=1 && i<=big.size())
    {
        big[i] = SubItem(big[i],0,sign);
        i++;
    }
    ClearHeadZero();
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
    if(bt1 == bt2)
    {
        bt.push_back(0);
        return;
    }
    if(bt1 < bt2)
    {
        bt.set_symbol(NEGATIVE);
        u_long i, j;
        i = j = 1;
        u_char sign = 0;
        u_char sub;
        while(j<=bt2.size() && i<=bt1.size())
        {
            sub = SubItem(bt2[j++], bt1[i++], sign);
            bt.push_back(sub);
        }
        while(j <= bt2.size())
        {
            sub = SubItem(bt2[j++], 0, sign);
            bt.push_back(sub);
        }
    }
    else
    {
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
}

#if 0
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
#endif
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

#if 0
void BigInt::Div(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    if(bt1 < bt2)
    {
        bt.push_back(0);
        return;
    }
    else if(bt1 == bt2)
    {
        bt.push_back(1);
        return;
    }

    BigInt tmp(bt1);
    while(tmp >= bt2)
    {
        tmp -= bt2;
        ++bt;
    }
}
#endif

void BigInt::Div(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    if(bt1 < bt2)
    {
        bt.push_back(0);
        return;
    }
    else if(bt1 == bt2)
    {
        bt.push_back(1);
        return;
    }
    BigInt btd;
    u_long i, j;
    i = bt1.size();
    j = bt2.size();
    
    int k = i-j;
    for(u_long x=1; x<=bt2.size(); ++x)
    {
        btd.push_back(bt1[k+x]);
    }

    k = i-j;
    u_char div = 0;
    while(k >= 0)
    {
        while(btd >= bt2)
        {
            btd -= bt2;
            ++div;
            btd.ClearHeadZero();
        }
        bt.push_front(div);
        if(k > 0)
            btd.push_front(bt1[k]);
        div = 0;
        --k;
    }
    bt.ClearHeadZero();
}

void BigInt::Mod(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    if(bt1 < bt2)
    {
        bt = bt1;
        return;
    }
    else if(bt1 == bt2)
    {
        bt.push_back(0);
        return;
    }

    BigInt btd;
    u_long i, j;
    i = bt1.size();
    j = bt2.size();
    int k = i-j;
    for(u_long x=1; x<=bt2.size(); ++x)
    {
        btd.push_back(bt1[k+x]);
    }
    k = i-j;
    while(k >= 0)
    {
        while(btd >= bt2)
        {
            btd -= bt2;
            btd.ClearHeadZero();
        }
        if(k > 0)
            btd.push_front(bt1[k]);
        --k;
    }
    bt = btd;
    if(bt != 0)
        bt.ClearHeadZero();
}
void BigInt::Square(BigInt &bt, const BigInt &bt1)
{
    Mul(bt, bt1, bt1);
}
void BigInt::Power(BigInt &bt, const BigInt &bt1, const BigInt &bt2)
{
    BigInt tmp(1);
    for(BigInt i=0; i<bt2; ++i)
    {
        Mul(bt, tmp, bt1);
        tmp = bt;
    }
}

void BigInt::PowMod( BigInt &bt, const BigInt &bt1, const BigInt &bt2, const BigInt &bt3)
{
    BigInt tmp;
    tmp.Clear();
    Power(tmp,bt1,bt2);
    cout<<"tmp = "<<tmp<<endl;
    Mod(bt, tmp, bt3);
}













