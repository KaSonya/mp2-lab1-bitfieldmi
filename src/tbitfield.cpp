// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len < 0)  
		throw invalid_argument("Error: negativ bit"); 

	BitLen = len;
	MemLen = (len + sizeof(TELEM) * 8 - 1) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];

	for (int i = 0; i < MemLen; i++)
	{
		pMem[i] = 0;
	}
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen; 
	MemLen = bf.MemLen;
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++) 
		pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	return n / (sizeof(TELEM)*8); 
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n % (sizeof(TELEM) * 8)); 			
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0 || n >= BitLen)  
		throw out_of_range("Error: bit index out of range - [0;BitLen]");  

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] |= mask; 
}

void TBitField::ClrBit(const int n) 
{
	if (n < 0 || n >= BitLen)  
		throw out_of_range("Error: bit index out of range - [0;BitLen]");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	pMem[index] &= ~mask; 
}

int TBitField::GetBit(const int n) const
{
	if (n < 0 || n >= BitLen)
		throw out_of_range("Error: bit index out of range - [0;BitLen]");

	int index = GetMemIndex(n);
	TELEM mask = GetMemMask(n);
	return (pMem[index] & mask) != 0;
}
// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf) { 
		delete[] pMem; 

		BitLen = bf.BitLen; 
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen]; 

		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i]; 
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (BitLen != bf.BitLen)
		return 0;

	for (int i = 0; i < MemLen; i++) 
		if (pMem[i] != bf.pMem[i])
			return 0;

	return 1; 
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	return !(*this == bf); 
}

TBitField TBitField::operator|(const TBitField &bf) 
{
	int maxLen = max(BitLen, bf.BitLen); 
	TBitField result(maxLen); 

	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = pMem[i]; 

	for (int i = 0; i < bf.MemLen; i++)
		result.pMem[i] |= bf.pMem[i]; 

	return result;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int maxLen = max(BitLen, bf.BitLen);
	TBitField result(maxLen);

	int minMemLen = min(MemLen, bf.MemLen);
	for (int i = 0; i < minMemLen; i++)
		result.pMem[i] = pMem[i] & bf.pMem[i]; 

	return result;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField result(BitLen); 

	for (int i = 0; i < MemLen; i++)
		result.pMem[i] = ~pMem[i]; 

	
	int FaleBits = BitLen % (sizeof(TELEM) * 8);
	if (FaleBits != 0) {
		TELEM mask = (1 << FaleBits) - 1;
		result.pMem[MemLen - 1] &= mask; 
	}

	return result;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string input;
	istr >> input;

	for (size_t i = 0; i < input.length() && i < bf.BitLen; i++) {
		if (input[i] == '1')
			bf.SetBit(i);
		else
			bf.ClrBit(i);
	}

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++) {
		ostr << (bf.GetBit(i) ? '1' : '0'); 
	}
	return ostr;
}
//1