/** \file SquareMatrix.h
  * \brief SquareMatrix-kantaluokan otsikkotiedosto
  */

#ifndef SQUAREMATRIX_H_INCLUDED
#define SQUAREMATRIX_H_INCLUDED

#include <iostream>
#include <map>
#include "Element.h"

using Valuation = std::map<char,int>;

template <typename Type>
class ElementarySquareMatrix;

/** \class SquareMatrix
  * \brief SquareMatrix-kantaluokka matriiseille
  */
class SquareMatrix
{
    public:
        /** \brief Oletushajotin
          */
        virtual ~SquareMatrix() = default;

        /** \brief Palauttaa osoittimen taman kopioon
          * \return Osoitin taman kopioon
          */
        virtual SquareMatrix* clone() const = 0;

        /** \brief Tulostaa matriisin
          * \param os Se virta, johon alkio tulostetaan
          */
        virtual void print(std::ostream& os) const = 0;

        /** \brief Matriisin string-esitys
          * \return Matriisi string-tyyppisena
          */
        virtual std::string toString() const = 0;

        /** \brief Matriisin evaluointi
          * \param valuation Sisaltaa muuttujiksi sidottavat arvot
          * \return Tata matriisia vastaava konkreettinen matriisi
          */
        virtual ElementarySquareMatrix<IntElement> evaluate(const Valuation& valuation) const = 0;
};

#endif // SQUAREMATRIX_H_INCLUDED
