// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len <= 0) throw "Length should be higher than 0";
	else
	{
		BitLen = len;
		MemLen = (BitLen + 31) / 32;
		pMem = new TELEM[MemLen];
		for (int i = 0; i < MemLen; i++)
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
	BitLen = 0;
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if (n < 0) throw "Bit number should be higher";
	if (n > BitLen) throw "Bit number should be lower";
	return n / 32;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if (n < 0) throw "Bit number should be higher";
	if (n > BitLen) throw "Bit number should be lower";
	return 1 << (n % 32);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if (n < 0) throw "Bit number should be higher";
	if (n > BitLen) throw "Bit number should be lower";
	unsigned int p = n / 32;
	unsigned int mask = GetMemMask(n);
	pMem[p] |= mask;
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if (n < 0) throw "Bit number should be higher";
	if (n > BitLen) throw "Bit number should be lower";
	pMem[GetMemIndex(n)] = pMem[GetMemIndex(n)] & (~GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if (n < 0) throw "Bit number should be higher";
	if (n > BitLen) throw "Bit number should be lower";
	return pMem[GetMemIndex(n)] & GetMemMask(n);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		delete[] this->pMem;
		this->pMem = new TELEM[bf.MemLen];
		this->BitLen = bf.BitLen;
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
	}
	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (this->BitLen == bf.BitLen)
	{
		for (int i = 0; i < BitLen; i++)
		{
			if (pMem[i] != bf.pMem[i])
				return 0;
		}
		return 1;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (!(*this == bf))
		return 1;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int max = 0;
	if (BitLen > bf.BitLen)
		max = BitLen;
	else
		max = bf.BitLen;
	TBitField tmp(max);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = bf.pMem[i];
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] |= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int max = 0;
	if (BitLen > bf.BitLen)
		max = BitLen;
	else
		max = bf.BitLen;
	TBitField tmp(max);
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] = bf.pMem[i];
	for (int i = 0; i < MemLen; i++)
		tmp.pMem[i] &= bf.pMem[i];
	return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField tmp(BitLen);
	for (int i = 0; i < MemLen; i++)
		if (GetBit(i) == 0)
			tmp.SetBit(i);
	return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	string tmp;
	istr >> tmp;
	if (tmp.size() != bf.GetLength())
		throw "Incorrect size";
	for (int i = 0; i < bf.BitLen; i++)
		if (tmp[i] == '0')
			bf.ClrBit(i);
		else
			if (tmp[i] == '1')
				bf.SetBit(i);
			else
				throw "Incorrect input";
	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	for (int i = 0; i < bf.BitLen; i++)
		if (bf.GetBit(i))
			ostr << '1';
		else
			ostr << '0';
	return ostr;
}
