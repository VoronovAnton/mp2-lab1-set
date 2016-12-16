// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)//:BitLen(len)
{
    if (len < 0)
        throw "len must be non-negative";
    else {
        this->BitLen = len;
        this->MemLen = (len - 1) / (8 * sizeof(TELEM)) + 1;
        this->pMem   = new TELEM[MemLen];
        if (this->pMem != NULL)
            for (int i = 0; i < this->MemLen; i++)
                this->pMem[i] = 0;
    }
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
    this->MemLen = bf.MemLen;
    this->BitLen = bf.BitLen;
    this->pMem   = new TELEM[MemLen];

    if (this->pMem != NULL)
        for (int i = 0; i < this->MemLen; i++)
            this->pMem[i] = bf.pMem[i];
}

TBitField::~TBitField()
{
    delete [] this->pMem ;
    this->pMem = NULL;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    return n >> 5;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
    return 1 << (n & 31);

}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
    return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if (n < 0)
        throw "n must be non-negative";

    if (this->BitLen < n)
        throw "n must be greater than BitLen";

    if ((-1 < n) && (n < this->BitLen))
        this->pMem[this->GetMemIndex(n)] |= this->GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n < 0)
        throw "n must be non-negative";

    if (this->BitLen < n)
        throw "n must be greater than BitLen";

    if ((-1 < n) && (n < this->BitLen))
        this->pMem[this->GetMemIndex(n)] &= ~(this->GetMemMask(n));
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if (n < 0)
        throw "n must be non-negative";

    if (this->BitLen < n)
        throw "n must be greater than BitLen";

    if ((-1 < n) && (n < this->BitLen))
        return this->pMem[this->GetMemIndex(n)] & this->GetMemMask(n);
    else
        return 0;
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    this->BitLen = bf.BitLen;

    delete [] this->pMem;
    this->pMem = NULL;

    this->pMem = new TELEM[this->MemLen];

    if(this->pMem != NULL)
        for (int i = 0; i < this->MemLen; i++)
            this->pMem[i] = bf.pMem[i];

    return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение(возвращает 0, если не равны, и 1, если равны)
{
    int isEqual = 1;

    if (this->BitLen == bf.BitLen) {
        for (int i = 0; i < this->MemLen; i++)
            if (this->pMem[i] != bf.pMem[i]) {
                isEqual = 0;
                break;
            }
    }
    else
        isEqual = 0;

    return isEqual;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение(возвращает 0, если не равны, и 1, если равны)
{
    int isNotEqual = 0;

    if (this->BitLen == bf.BitLen) {
        for (int i = 0; i < this->MemLen; i++)
            if (this->pMem[i] != bf.pMem[i]) {
                isNotEqual = 1;
                break;
            }
    }
    else
        isNotEqual = 1;

    return isNotEqual;
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
    int len = this->BitLen;

    if (len < bf.BitLen)
        len = bf.BitLen;

    TBitField disjunction(len);
    for (int i = 0; i < this->MemLen; i++)
        disjunction.pMem[i] = this->pMem[i];

    for (int i = 0; i < bf.MemLen; i++)
        disjunction.pMem[i] |= bf.pMem[i];

    return disjunction;
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    int len = this->BitLen;

    if (len < bf.BitLen)
        len = bf.BitLen;

    TBitField conjunction(len);
    for (int i = 0; i < this->MemLen; i++)
        conjunction.pMem[i] = this->pMem[i];

    for (int i = 0; i < bf.MemLen; i++)
        conjunction.pMem[i] &= bf.pMem[i];

    return conjunction;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField negation(*this);

    for (int i = 0; i < this->MemLen - 1; i++)
        negation.pMem[i] = ~negation.pMem[i];

    for (int i = (this->MemLen - 1) << 5; i < this->BitLen; i++)
        if (negation.GetBit(i))
            negation.ClrBit(i);
        else
            negation.SetBit(i);

    return negation;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    char ts = 0;
    do {
        istr >> ts;
    } while (ts != ' ');

    int i = 0;
    while (1) {
        istr >> ts;
        if (ts == '0')
            bf.ClrBit(i++);
        else
            if (ts == '1')
                bf.SetBit(i++);
            else
                break;
    }

    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
    int len = bf.GetLength();

    for (int i = 0; i < len; i++)
        if (bf.GetBit(i))
            ostr << '0';
        else
            ostr << '1';

    return ostr;
}
