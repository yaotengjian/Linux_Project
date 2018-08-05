#ifndef _BIGINT_H
#define _BIGINT_H

#include"SeqList.h"

class BigInt
{
public:
    BigInt();
    BigInt(u_long x);
    BigInt(const BigInt &bt);
    BigInt& operator=(const BigInt &bt);
    ~BigInt();
public:
    void LoadData(u_long sz);
    void PrintData()const;
    void Clear();
    void BigIntCopy(char *buf, size_t len, size_t &pos);
public:
    u_long size()const
    {return big.size();}
    void push_back(u_char x)
    {big.push_back(x);}
    u_char& operator[](u_long i)
    {return big[i];}
    const u_char& operator[](u_long i)const
    {return big[i];}
private:
    SeqList<u_char> big;
};

#endif
