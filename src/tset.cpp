// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

TSet::TSet(int mp) : BitField(mp), MaxPower(mp)
{
    if (mp < 0)
        throw invalid_argument("Error: negativ number");
    
}

// конструктор копирования
TSet::TSet(const TSet& s) : BitField(s.BitField), MaxPower(s.MaxPower)
{
}

// конструктор преобразования типа
TSet::TSet(const TBitField &bf) : BitField(bf), MaxPower(bf.GetLength())
{
}

TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower)
        throw invalid_argument("Error: out of range (0,MaxPower)"); 

    return BitField.GetBit(Elem); 
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw invalid_argument("Error: out of range (0,MaxPower)"); 

    return BitField.SetBit(Elem); 
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower)
        throw invalid_argument("Error: out of range (0,MaxPower)"); 

    return BitField.ClrBit(Elem); 
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet &s) // присваивание
{
    if (this != &s) { 
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet &s) const // сравнение (равенство)
{
    if (MaxPower == s.MaxPower && BitField == s.BitField)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int TSet::operator!=(const TSet &s) const // сравнение (неравенство)
{
    return !(*this == s);  
}

TSet TSet::operator+(const TSet &s) // объединение
{
    int maxPower = max(MaxPower, s.MaxPower); 
    TSet rez(maxPower); 
    rez.BitField = BitField | s.BitField; 
    return rez;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower)
        throw invalid_argument("Error: out of range (0,MaxPower)"); 
    TSet rez(*this); 
    rez.InsElem(Elem); 
    return rez; 
}

TSet TSet::operator-(const int Elem) 
{
    if (Elem < 0 || Elem >= MaxPower)
        throw invalid_argument("Error: out of range (0,MaxPower)");
    
    TSet rez(*this);
    rez.DelElem(Elem);
    return rez;
}

TSet TSet::operator*(const TSet &s) // пересечение
{
    int maxPower = max(MaxPower, s.MaxPower); 
    TSet rez(maxPower); 
    rez.BitField = BitField & s.BitField; 
    return rez; 
}

TSet TSet::operator~(void) // дополнение
{
    TSet rez(MaxPower); 
    rez.BitField = ~BitField;

    for (int i = MaxPower; i < rez.BitField.GetLength(); i++)
    {
        rez.BitField.ClrBit(i);
    }
    return rez;
}

// перегрузка ввода/вывода

istream &operator>>(istream &istr, TSet &s) // ввод
{
    string input;
    istr >> input;

    for (size_t i = 0; i < input.length() && i < s.MaxPower; i++)
    {
        if (input[i] == '1')
        {
            s.InsElem(i);
        }
        else
        {
            s.DelElem(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream &ostr, const TSet &s) // вывод
{
    for (int i = 0; i < s.MaxPower; i++)
    {
        ostr << (s.IsMember(i) ? '1' : '0');
    }

    return ostr; 
}
