/** \file Element.h
  * \brief Element-kantaluokan otsikkotiedosto
  */

#ifndef ELEMENT_H_INCLUDED
#define ELEMENT_H_INCLUDED
#include <map>

using Valuation = std::map<char,int>;

/** \class Element
  * \brief Element-kantaluokka matriisin alkioille
  */
class Element
{
    public:
        /** \brief Oletushajotin
          */
        virtual ~Element() = default;

        /** \brief Palauttaa osoittimen taman kopioon
          * \return Osoitin taman kopioon
          */
        virtual Element* clone() const = 0;

        /** \brief Tulostaa alkion
          * \param os Se virta, johon alkio tulostetaan
          */
        virtual void print(std::ostream& os) const = 0;

        /** \brief Palauttaa attribuutin var string-tyyppisena
          * \return Attribuutti var string-tyyppisena
          */
        virtual std::string toString() const = 0;

        /** \brief Alkion evaluointi
          * \param valuation Sisaltaa muuttujiksi sidottavat arvot
          * \return Palauttaa alkion sisalto evaluoituna
          * \exception Poikkeus heitetaan, jos muuttujaa ei ole maaritelty
          */
        virtual int evaluate(const Valuation& val) const = 0;
};

/** \class TElement
  * \brief Element-luokan aliluokka TElement matriisin alkioille
  * \tparam Type Int- tai char-tyyppia oleva malliparametri
  */
template<typename Type>
class TElement : public Element
{
    public:
        /** \brief Oletusmuodostin
          */
        TElement();

        /** \brief Parametrillinen muodostin
          * \param v Attribuutille val asetettava arvo
          */
        TElement(Type v);

        /** \brief Oletushajotin
          */
        ~TElement() = default;

        /** \brief Saantimetodi attribuutille val
          * \return Attribuutin val sisalto
          */
        Type getVal() const;

        /** \brief Asetusmetodi attribuutille val
          * \param v Attribuutille val asetettava arvo
          */
        void setVal(Type v);

        /** \brief Palauttaa osoittimen taman kopioon
          * \return Osoitin taman kopioon
          */
        Element* clone() const override;

        /** \brief Tulostaa alkion
          * \param os Se virta, johon alkio tulostetaan
          */
        void print(std::ostream& os) const override;

        /** \brief Palauttaa attribuutin val string-tyyppisena
          * \return Attribuutti val string-tyyppisena
          */
        std::string toString() const override;

        /** \brief Alkion evaluointi
          * \param valuation Sisaltaa muuttujiksi sidottavat arvot
          * \return Palauttaa val evaluoituna
          * \exception std::exception jos muuttujaa ei ole maaritelty
          */
        int evaluate(const Valuation& valuation) const override;

        /** \brief Ylikuormitettu summa- ja sijoitusoperaattori
          * \param i Viite siihen olioon, joka lisataan kyseessa olevaan olioon
          * \return Summattavien alkioiden summa
          */
        TElement& operator+=(const TElement& i);

        /** \brief Ylikuormitettu erotus- ja sijoitusoperaattori
          * \param i Viite siihen olioon, joka vahennetaan kyseessa olevasta oliosta
          * \return Vahennettavien alkioiden erotus
          */
        TElement& operator-=(const TElement& i);

        /** \brief Ylikuormitettu tulo- ja sijoitusoperaattori
          * \param i Viite siihen olioon, jolla kyseessa oleva olio kerrotaan
          * \return Kerrottavien alkioiden tulo
          */
        TElement& operator*=(const TElement& i);

        /** \brief Ylikuormitettu vertailuoperaattori
          * \param i Viite olioon, johon tata verrataan
          * \return true, jos parametrina annettava olio vastaa tata oliota, ja false muuten
          */
        bool operator==(const TElement& i) const;

    private:
        Type val;
};

using IntElement = TElement<int>;
using VariableElement = TElement<char>;

template<typename Type>
TElement<Type>::TElement() : val{0}
{
}

template<typename Type>
TElement<Type>::TElement(Type v) : val{v}
{
}

template<typename Type>
Type TElement<Type>::getVal() const
{
    return val;
}

template<typename Type>
void TElement<Type>::setVal(Type v)
{
    val = v;
}

template<typename Type>
Element* TElement<Type>::clone() const
{
    return new TElement<Type> {*this};
}

template<typename Type>
void TElement<Type>::print(std::ostream& os) const
{
    os << val;
}

template<typename Type>
std::string TElement<Type>::toString() const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}

template<>
inline int TElement<int>::evaluate(const Valuation& valuation) const
{
    return this->val;
}

template<typename Type>
int TElement<Type>::evaluate(const Valuation& valuation) const
{
    auto iter = valuation.find(val);
    if(iter == valuation.end())
    {
        throw std::exception();
    }
    else
    {
        return iter->second;
    }
}

template<typename Type>
TElement<Type>& TElement<Type>::operator+=(const TElement<Type>& i)
{
    setVal(val + i.val);
    return *this;
}

template<typename Type>
TElement<Type>& TElement<Type>::operator-=(const TElement<Type>& i)
{
    setVal(val - i.val);
    return *this;
}

template<typename Type>
TElement<Type>& TElement<Type>::operator*=(const TElement<Type>& i)
{
    setVal(val*i.val);
    return *this;
}

template<typename Type>
bool TElement<Type>::operator==(const TElement<Type>& i) const
{
    if(val != i.val)
    {
        return false;
    }
    else
    {
        return true;
    }
}

/** \brief Ylikuormitettu summaoperaattori
  * \param i_1 Summan ensimmainen tekija
  * \param i_2 Summan toinen tekija
  * \return Tekijoiden int-arvoja vastaava summa
  */
IntElement operator+(const IntElement& i_1, const IntElement& i_2);

/** \brief Ylikuormitettu erotusoperaattori
  * \param i_1 Erotuksen ensimmainen tekija
  * \param i_2 Erotuksen toinen tekija
  * \return Tekijoiden int-arvoja vastaava erotus
  */
IntElement operator-(const IntElement& i_1, const IntElement& i_2);

/** \brief Ylikuormitettu tulo-operaattori
  * \param i_1 Tulon ensimmainen tekija
  * \param i_2 Tulon toinen tekija
  * \return Tekijoiden int-arvoja vastaava tulo
  */
IntElement operator*(const IntElement& i_1, const IntElement& i_2);

/** \brief Ylikuormitettu tulostusoperaattori
  * \param out Tulostusvirta
  * \param e Viite siihen olioon, jonka tiedot tulostetaan
  * \return Se tulostusvirta, jolla tulostetaan
  */
std::ostream& operator<<(std::ostream& out, const Element& e);

#endif // ELEMENT_H_INCLUDED
