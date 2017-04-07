/** \file CompositeSquareMatrix.h
  * \brief CompositeSquareMatrix-luokan otsikkotiedosto
  */

#ifndef COMPOSITESQUAREMATRIX_H
#define COMPOSITESQUAREMATRIX_H

#include <functional>
#include <memory>
#include "SquareMatrix.h"
#include "ElementaryMatrix.h"


using Valuation = std::map<char,int>;

/** \class CompositeSquareMatrix
  * \brief Luokka komposiitti-neliomatriiseille
  */
class CompositeSquareMatrix : public SquareMatrix
{
    public:
        /** \brief Parametrillinen muodostin
          * \param op1 Ensimmainen neliomatriisitekija
          * \param op2 Toinen neliomatriisitekija
          * \param opr Neliomatriiseihin sovellettava operaattori
          * \param opc Operaattorin symboli
          */
        CompositeSquareMatrix(const SquareMatrix& op1, const SquareMatrix& op2, const std::function<ConcreteSquareMatrix(const ConcreteSquareMatrix&, const ConcreteSquareMatrix&)>& opr, char opc);

        /** \brief Kopiomuodostin
          * \param m Viite olioon, joka kopioidaan
          */
        CompositeSquareMatrix(const CompositeSquareMatrix& m);

        /** \brief Siirtomuodostin
          * \param Rvalue-viite
          */
        CompositeSquareMatrix(CompositeSquareMatrix&& m);

        /** \brief Oletushajotin
          */
        ~CompositeSquareMatrix() = default;

        /** \brief Ylikuormitettu sijoitusoperaattori
          * \param m Viite siihen olioon, joka sijoitetaan kyseessa olevaksi olioksi
          * \return Viite kyseessa olevaan olioon
          */
        CompositeSquareMatrix& operator=(const CompositeSquareMatrix& m);

        /** \brief Sijoitusoperaattori
          * \param Rvalue-viite
          * \return Viite kyseessa olevaan olioon
          */
        CompositeSquareMatrix& operator=(CompositeSquareMatrix&& m);

        /** \brief Matriisin kloonaus
          * \return Osoitin taman olion kopioon
          */
        SquareMatrix* clone() const override;

        /** \brief Tulostaa matriisin
          * \param os Se virta, johon matriisi tulostetaan
          */
        void print(std::ostream& os) const override;

        /** \brief Palauttaa matriisin string-tyyppisena
          * \return Kyseessa oleva neliomatriisi string-tyyppisena
          */
        std::string toString() const override;

        /** \brief Matriisin evaluointi
          * \param val Sisaltaa muuttujiksi sidottavat arvot
          * \return Tata matriisia vastaava konkreettinen matriisi
          * \throw std::exception jos jotain muuttujaa ei ole maaritelty
          */
        ConcreteSquareMatrix evaluate(const Valuation& val) const override;

    private:
        std::unique_ptr<SquareMatrix> oprnd1;
        std::unique_ptr<SquareMatrix> oprnd2;
        std::function<ConcreteSquareMatrix(const ConcreteSquareMatrix&, const ConcreteSquareMatrix&)> oprtor;
        char op_char;
};

std::ostream& operator<<(std::ostream& out, const SquareMatrix& m);

#endif // COMPOSITESQUAREMATRIX_H
