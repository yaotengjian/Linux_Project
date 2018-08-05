#ifndef _BIGINT_H
#define _BIGINT_H

#include"SeqList.h"

class BigInt;
ostream& operator<<(ostream &out, const BigInt &);

class BigInt
{
    friend ostream& operator<<(ostream &out, const BigInt &);
public:
    BigInt();
    BigInt(long x);
    BigInt(const BigInt &bt);
    BigInt& operator=(const BigInt &bt);
    ~BigInt();
public:
    void LoadData(u_long sz);
    void PrintData()const;
    void Clear();
    void BigIntCopy(char *buf, size_t len, size_t &pos);
    void ClearHeadZero();
public:
    u_long size()const
    {return big.size();}
    void push_back(u_char x)
    {big.push_back(x);}
    void push_front(u_char x)
    {big.push_front(x);}
    u_char& operator[](u_long i)
    {return big[i];}
    const u_char& operator[](u_long i)const
    {return big[i];}
    void set_symbol(SYMBOL symbol)
    {big.set_symbol(symbol);}
    SYMBOL get_symbol()const
    {return big.get_symbol();}
public:
    //bt = bt1 + bt2;
    static u_char AddItem(u_char a, u_char b, u_char &sign);
    static void Add(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
    //bt1 >= bt2
    static u_char SubItem(u_char a, u_char b, u_char &sign);
    static void Sub(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
    static void MulItem(BigInt &bt, const BigInt &bt1, u_char x);
    static void MoveAdd(BigInt &bt, const BigInt &bt1, u_long pos);
    static void Mul(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
    //bt2 != 0
    static void Div(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
    static void Mod(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
    static void PowMod(BigInt &bt, const BigInt &bt1, const BigInt &bt2, const BigInt &bt3);
    static void Square(BigInt &bt, const BigInt &bt1);
    static void Power(BigInt &bt, const BigInt &bt1, const BigInt &bt2);
public:
    BigInt operator+(const BigInt &bt);
    BigInt operator-(const BigInt &bt);
    BigInt operator*(const BigInt &bt);
    BigInt operator/(const BigInt &bt);
    BigInt operator%(const BigInt &bt);
    BigInt operator^(const size_t n);
    //BigInt operator^(const BigInt &bt);
public:
    bool operator==(const BigInt &bt)const;
    bool operator!=(const BigInt &bt)const;
    bool operator <(const BigInt &bt)const;
    bool operator>=(const BigInt &bt)const;
    bool operator<=(const BigInt &bt)const;
    bool operator >(const BigInt &bt)const;
public:
    BigInt& operator++();
    BigInt  operator++(int);
    BigInt& operator--();
    BigInt  operator--(int);
    BigInt& operator+=(const BigInt &bt);
    BigInt& operator-=(const BigInt &bt);
    BigInt& operator*=(const BigInt &bt);
    BigInt& operator/=(const BigInt &bt);
    BigInt& operator^=(const BigInt &bt);
private:
    SeqList<u_char> big;
};

#endif
