#ifndef _TEST_H
#define _TEST_H

#include"util.h"
#include<string.h>

class BigInt;

template<typename Type>
class SeqList
{
    friend class BigInt;
public:
    SeqList(int sz = DEFAULT_SIZE);
    SeqList(const SeqList<Type> &st);
    SeqList<Type>& operator=(const SeqList<Type> &st);
    ~SeqList();
public:
    bool INC()
    {
        Type *new_base = new Type[capacity + INC_SIZE + 1];
        if(NULL == new_base)
            return false;
        memcpy(new_base,base,sizeof(Type)*(capacity+1));
        capacity += INC_SIZE;
        delete base;
        base = new_base;
        return true;
    }
public:
    bool IsFull()const
    {return len>=capacity;}
    bool IsEmpty()const
    {return len == 0;}
public:
    void push_back(const Type &x);
    void push_front(const Type &x);
    void pop_back();
    void pop_front();
    void show_seqlist()const;
    void insert(const Type &x, int pos);
    void erase(int pos);
    u_long find(const Type &key);
    u_long size()const;
    void clear();
    void sort();
    void reverse();
public:
    Type& back(){return base[len];}
    const Type& back()const{return base[len];}
    Type& front(){return base[1];}
    const Type& front()const{return base[1];}
    Type& at(int pos);
    Type& operator[](int pos);
    const Type& operator[](int pos)const;
private:
    enum{DEFAULT_SIZE=8, INC_SIZE=5};
    Type *base;
    u_long capacity;
    u_long len;
};
template<typename Type>
const Type& SeqList<Type>::operator[](int pos)const
{
    assert(pos>=1 && pos<=len);
    return base[pos];
}
template<typename Type>
Type& SeqList<Type>::operator[](int pos)
{
    assert(pos>=1 && pos<=len);
    return base[pos];
}

template<typename Type>
Type& SeqList<Type>::at(int pos)
{
    assert(pos>=1 && pos<=len);
    return base[pos];
}

template<typename Type>
void SeqList<Type>::reverse()
{
    assert(len != 0);
    u_long i,j;
    i = 1;
    j = len;
    while(i < j)
    {
        Type tmp = base[i];
        base[i] = base[j];
        base[j] = tmp;
        i++;
        j--;
    }
}
template<typename Type>
void SeqList<Type>::sort()
{
    cout<<"len = "<<size()<<endl;
    for(u_long i=1; i<len; ++i)
    {
        for(u_long j=1; j<=len-i; ++j)
        {
            if(base[j] > base[j+1])
            {
                Type tmp = base[j];
                base[j] = base[j+1];
                base[j+1] = tmp;
            }
        }
    }
}

template<typename Type>
void SeqList<Type>::clear()
{
    len = 0;
}

template<typename Type>
u_long SeqList<Type>::size()const
{
    return len;
}

template<typename Type>
u_long SeqList<Type>::find(const Type &key)
{
    for(u_long i=1; i<=len; ++i)
    {
        if(base[i] == key)
            return i;
    }
    return 0;
}

template<typename Type>
void SeqList<Type>::erase(int pos)
{
    if(!IsEmpty())
    {
        assert(pos>=1 && pos<=len);
        for(u_long i=pos; i<len; ++i)
        {
            base[i] = base[i+1];
        }
        len--;
    }
}

template<typename Type>
void SeqList<Type>::insert(const Type &x, int pos)
{
    if(!IsFull() || INC())
    {
        assert(pos>=1 && pos<=len+1);
        for(u_long i=len+1; i>pos; --i)
        {
            base[i] = base[i-1];
        }
        base[pos] = x;
        len++;
    }
}

template<typename Type>
void SeqList<Type>::pop_back()
{
    if(!IsEmpty())
    {
        len--;
    }
}
template<typename Type>
void SeqList<Type>::pop_front()
{
    if(!IsEmpty())
    {
        for(u_long i=1; i<len; ++i)
        {
            base[i] = base[i+1];
        }
        len--;
    }
}


template<typename Type>
void SeqList<Type>::push_front(const Type &x)
{
    if(!IsFull() || INC())
    {
        for(u_long i=len; i>=1; --i)
        {
            base[i+1] = base[i];
        }
        base[1] = x;
        len++;
    }
}
template<typename Type>
SeqList<Type>::SeqList(int sz) 
{
    capacity = sz > DEFAULT_SIZE ? sz : DEFAULT_SIZE;
    base = new Type[capacity+1];
    memset(base,0,sizeof(Type)*(capacity+1));
    len = 0;
}
template<typename Type>
SeqList<Type>::SeqList(const SeqList<Type> &st)
{
    capacity = st.capacity;
    base = new Type[capacity+1];
    memcpy(base,st.base,sizeof(Type)*(capacity+1));
    len = st.len;
}
template<typename Type>
SeqList<Type>& SeqList<Type>::operator=(const SeqList<Type> &st)
{
    if(this != &st)
    {
        if(st.len>capacity)
        {
            delete []base;
            capacity = st.capacity;
            base = new Type[capacity+1];
        }
        memcpy(base,st.base,sizeof(Type)*(st.len+1));
        len = st.len;
    }
    return *this;
}
template<typename Type>
SeqList<Type>::~SeqList()
{
    delete []base;
    base = NULL;
    capacity = len = 0;
}
template<typename Type>
void SeqList<Type>::push_back(const Type &x)
{
    if(!IsFull() || INC())
    {
        base[++len] = x;
    }
}
template<typename Type>
void SeqList<Type>::show_seqlist()const
{
    for(int i=1; i<=len; ++i)
    {
        cout<<base[i]<<" ";
    }
    cout<<endl;
}

#endif









