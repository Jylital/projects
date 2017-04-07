/** \file ElementaryMatrix.h
  * \brief ElementarySquareMatrix-luokan otsikkotiedosto
  */

#ifndef ELEMENTARYMATRIX_H
#define ELEMENTARYMATRIX_H

#include <vector>
#include <memory>
#include <algorithm>
#include <chrono>
#include <random>
#include <thread>
#include <future>
#include "SquareMatrix.h"
#include "Element.h"

using Valuation = std::map<char,int>;

/** \class ElementarySquareMatrix
  * \brief Malliluokka nxn-neliomatriiseille
  * \tparam Type IntElement- tai VariableElement-tyyppinen malliparametri
  */
template <typename Type>
class ElementarySquareMatrix : public SquareMatrix
{
    public:
        /** \brief Oletusmuodostin
          */
        ElementarySquareMatrix();

        /** \brief Parametrillinen muodostin
          * \param str_m Matriisin sisalto string-tyyppisena
          * \throw std::invalid_argument jos parametri ei kelpaa
          */
        ElementarySquareMatrix(const std::string& str_m);

        /** \brief Parametrillinen muodostin, joka muodostaa satunnaisen neliomatriisin
          * \param m Matriisin koko
          * \throw std::invalid_argument jos argumentti ei kelpaa
          */
        ElementarySquareMatrix(int m);

        /** \brief Kopiomuodostin
          * \param m Viite olioon, joka kopioidaan
          */
        ElementarySquareMatrix(const ElementarySquareMatrix& m);

        /** \brief Siirtomuodostin
          * \param Rvalue-viite
          */
        ElementarySquareMatrix(ElementarySquareMatrix&& m);

        /** \brief Saantimetodi attribuutille n
          * \return Attribuutin n arvo
          */
        unsigned int getSize() const;

        /** \brief Saantimetodi vektorille elements
          * \return Attribuutin elements viite
          */
        const std::vector< std::vector<std::unique_ptr<Type>> >& getElem() const;

        /** \brief Asetusmetodi attribuutille n
          * \param i_n Attribuutiksi n asetettava arvo
          * \throw std::invalid_argument jos parametrina annettava arvo ei kelpaa tai ei vastaa matriisin alkioiden lukumaaraa
          */
        void setSize(unsigned int i_n);

        /** \brief Oletushajotin
          */
        ~ElementarySquareMatrix() = default;

        /** \brief Ylikuormitettu sijoitusoperaattori
          * \param m Viite siihen olioon, joka sijoitetaan kyseessa olevaksi olioksi
          * \return Viite kyseessa olevaan olioon
          */
        ElementarySquareMatrix& operator=(const ElementarySquareMatrix& m);

        /** \brief Sijoitusoperaattori
          * \param Rvalue-viite
          * \return Viite kyseessa olevaan olioon
          */
        ElementarySquareMatrix& operator=(ElementarySquareMatrix&& m);

        /** \brief Matriisin kloonaus
          * \return Osoitin taman olion kopioon
          */
        SquareMatrix* clone() const override;

        /** \brief Neliomatriisin transpoosi
          * \return Palauttaa taman neliomatriisin transpoosin
          */
        ElementarySquareMatrix transpose() const;

        /** \brief Ylikuormitettu summa- ja sijoitusoperaattori
          * \param m Viite siihen olioon, joka lisataan kyseessa olevaan matriisiin
          * \return Summattavien olioiden summa
          * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
          */
        ElementarySquareMatrix& operator+=(const ElementarySquareMatrix& m);

        /** \brief Ylikuormitettu erotus- ja sijoitusoperaattori
          * \param m Viite siihen olioon, joka vahennetaan kyseessa olevasta matriisista
          * \return Vahennettavien olioiden erotus
          * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
          */
        ElementarySquareMatrix& operator-=(const ElementarySquareMatrix& m);

        /** \brief Ylikuormitettu tulo- ja sijoitusoperaattori
          * \param m Viite siihen olioon, jolla kyseessa oleva matriisi kerrotaan
          * \return Kerrottavien matriisien tulo
          * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
          */
        ElementarySquareMatrix& operator*=(const ElementarySquareMatrix& m);

        /** \brief Ylikuormitettu vertailuoperaattori
          * \param m Viite siihen olioon, jota verrataan kyseessa olevaan matriisiin
          * \return Palauttaa false, jos matriisit ovat erilaiset, muuten true
          */
        bool operator==(const ElementarySquareMatrix& m) const;

        /** \brief Tulostaa matriisin
          * \param os Se virta, johon matriisi tulostetaan
          */
        void print(std::ostream& os) const override;

        /** \brief Matriisi string-muodon maaritys
          * \return Kyseessa oleva neliomatriisi string-tyyppisena
          */
        std::string toString() const override;

        /** \brief Matriisin evaluointi
          * \param valuation Sisaltaa muuttujiksi sidottavat arvot
          * \return Evaluoitu matriisi
          * \throw std::exception jos jotain muuttujaa ei ole maaritelty
          */
        ElementarySquareMatrix<IntElement> evaluate(const Valuation& valuation) const override;

    private:
        unsigned int n;
        std::vector<std::vector<std::unique_ptr<Type>>> elements;
};

using ConcreteSquareMatrix = ElementarySquareMatrix<IntElement>;
using SymbolicSquareMatrix = ElementarySquareMatrix<Element>;

/** \brief Ylikuormitettu summaoperaattori
  * \param m_1 Summan ensimmainen tekija
  * \param m_2 Summan toinen tekija
  * \return Tekijoiden summa
  * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
  */
ConcreteSquareMatrix operator+(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2);

/** \brief Ylikuormitettu erotusoperaattori
  * \param m_1 Erotuksen ensimmainen tekija
  * \param m_2 Erotuksen toinen tekija
  * \return Tekijoiden erotus
  * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
  */
ConcreteSquareMatrix operator-(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2);

/** \brief Ylikuormitettu tulo-operaattori
  * \param m_1 Tulon ensimmainen tekija
  * \param m_2 Tulon toinen tekija
  * \return Tekijoiden tulo
  * \throw std::invalid_argument jos parametrina annettava neliomatriisi ei kooltaan vastaa kyseessa olevaa neliomatriisia
  */
ConcreteSquareMatrix operator*(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2);

/** \brief Operaatio vektoreille
  * \param m_1 Ensimmainen tekija
  * \param m_2 Toinen tekija
  * \param opr Vektoreille suoritettava operaatio
  * \throw std::invalid_argument jos parametrien koot eivat ole yhteensopivia
  */
void vectorOp(std::vector<std::unique_ptr<IntElement>>& leftop, const std::vector<std::unique_ptr<IntElement>>& rightop,
              const std::function<IntElement(const IntElement&, const IntElement&)>& opr);

/** \brief Kertolasku vektorille ja matriisille
  * \param product Vektori, johon tulos sijoitetaan
  * \param leftop Tulossa esiintyva vektori
  * \param rightop Tulossa esiintyva matriisi
  * \throw std::invalid_argument jos parametrien koot eivat ole yhteensopivia
  */
void vectorMult(std::vector<std::unique_ptr<IntElement>>& product, const std::vector<std::unique_ptr<IntElement>>& leftop, const std::vector<std::vector<std::unique_ptr<IntElement>>>& rightop);

/** \brief Luo satunnaisen vektorin
  * \param m Vektorin koko
  * \return Satunnainen vektori
  */
std::vector<std::unique_ptr<IntElement>> randomVect(unsigned int m);

template <typename Type>
ElementarySquareMatrix<Type>::ElementarySquareMatrix() : n{2}
{
    elements.resize(2);
    for(auto& vekt : elements)
    {
        vekt.push_back(std::unique_ptr<Type> {new IntElement{}});
        vekt.push_back(std::unique_ptr<Type> {new IntElement{}});
    }
}

template <>
inline ElementarySquareMatrix<IntElement>::ElementarySquareMatrix(const std::string& str_m) : n{0}
{
    std::vector< std::vector<std::unique_ptr<IntElement>> > matr;
    std::stringstream ss;
    ss << str_m;
    char c = ' ';
    int val = 0;
    ss >> c;
    if(!ss.good() || c != '[')
    {
        throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
    }
    ss >> c;
    if(!ss.good())
    {
        throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
    }
    while(c != ']')
    {
        if(c != '[')
        {
            throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
        }
        std::vector<std::unique_ptr<IntElement>> vekt;
        while(c != ']')
        {
            ss >> val;
            if(!ss.good())
            {
                throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
            }
            vekt.push_back(std::unique_ptr<IntElement> {new IntElement{val}});
            ss >> c;
            if(!ss.good() || (c != ',' && c != ']'))
            {
                throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
            }
        }
        matr.push_back(std::move(vekt));
        ss >> c;
        if(!ss.good())
        {
            throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
        }
    }
    auto m_n = matr.size();
    if(std::any_of(matr.cbegin(), matr.cend(), [m_n](const std::vector<std::unique_ptr<IntElement>>& vect){return(vect.size() != m_n);}))
    {
        throw std::invalid_argument{"Argumentti ei ole oikeaa muotoa."};
    }
    std::swap(matr, elements);
    setSize(elements.size());
}

template <typename Type>
ElementarySquareMatrix<Type>::ElementarySquareMatrix(const std::string& str_m) : n{0}
{
    std::vector< std::vector<std::unique_ptr<Type>> > matr;
    std::stringstream ss;
    ss << str_m;
    char c = ' ';
    int val = 0;
    ss >> c;
    if(!ss.good() || c != '[')
    {
        throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
    }
    ss >> c;
    if(!ss.good())
    {
        throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
    }
    while(c != ']')
    {
        if(c != '[')
        {
            throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
        }
        std::vector<std::unique_ptr<Type>> vekt;
        while(c != ']')
        {
            auto inp = ss.peek();
            if(ss.good() && std::isalpha(inp))
            {
                ss >> c;
                vekt.push_back(std::unique_ptr<Type> {new VariableElement{c}});
            }
            else
            {
                ss >> val;
                if(ss.good())
                {
                    vekt.push_back(std::unique_ptr<Type> {new IntElement{val}});
                }
                else
                {
                    throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
                }
            }
            ss >> c;
            if(!ss.good() || (c != ',' && c != ']'))
            {
                throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
            }
        }
        matr.push_back(std::move(vekt));
        ss >> c;
        if(!ss.good())
        {
            throw std::invalid_argument("Argumentti ei ole oikeaa muotoa.");
        }
    }
    auto m_n = matr.size();
    if(std::any_of(matr.cbegin(), matr.cend(), [m_n](const std::vector<std::unique_ptr<Type>>& vect){return(vect.size() != m_n);}))
    {
        throw std::invalid_argument{"Argumentti ei ole oikeaa muotoa."};
    }
    std::swap(matr, elements);
    setSize(elements.size());
}

template <>
inline ElementarySquareMatrix<IntElement>::ElementarySquareMatrix(int m) : n{0}
{
    if(m <= 0)
    {
        throw std::invalid_argument{"Argumentti ei ole oikeaa muotoa."};
    }
    elements.resize(m);
    std::vector<std::future<std::vector<std::unique_ptr<IntElement>>>> vector_t;
    for(int i = 0; i < m; i++)
    {
        vector_t.push_back(std::async(std::launch::async, randomVect, m));
    }
    auto iter = vector_t.begin();
    for(auto& row : elements)
    {
        row = iter->get();
        iter++;
    }
    setSize(m); // Ei voi heittaa poikkeusta
}

template <typename Type>
ElementarySquareMatrix<Type>::ElementarySquareMatrix(const ElementarySquareMatrix<Type>& m)
{
    elements.resize(m.getSize());
    auto iter = elements.begin();
    for(const auto& i : m.getElem())
    {
        for(const auto& j : i)
        {
            iter->push_back(std::unique_ptr<Type> {dynamic_cast<Type*>(j->clone())});
        }
        iter++;
    }
    setSize(m.getSize()); // Ei voi heittaa poikkeusta
}

template <typename Type>
ElementarySquareMatrix<Type>::ElementarySquareMatrix(ElementarySquareMatrix<Type>&& m) : n{m.getSize()}
{
    std::swap(elements, m.elements);
}

template <typename Type>
unsigned int ElementarySquareMatrix<Type>::getSize() const
{
    return n;
}

template <typename Type>
const std::vector< std::vector<std::unique_ptr<Type>> >& ElementarySquareMatrix<Type>::getElem() const
{
    return elements;
}

template <typename Type>
void ElementarySquareMatrix<Type>::setSize(unsigned int i_n)
{
    if(getElem().size() != i_n)
    {
        throw std::invalid_argument{"Neliomatriisin rivien ja sarakkeiden maaran taytyy vastata sen alkioiden lukumaaraa."};
    }
    n = i_n;
}

template <typename Type>
ElementarySquareMatrix<Type>& ElementarySquareMatrix<Type>::operator=(const ElementarySquareMatrix<Type>& m)
{
    if(this == &m)
    {
        return *this;
    }
    std::vector< std::vector<std::unique_ptr<Type>> > matriisi{m.getSize()};
    auto iter = matriisi.begin();
    for(const auto& i : m.getElem())
    {
        for(const auto& j : i)
        {
            iter->push_back(std::unique_ptr<Type> {dynamic_cast<Type*>(j->clone())});
        }
        iter++;
    }
    std::swap(elements, matriisi);
    setSize(m.getSize()); // Ei voi heittaa poikkeusta
    return *this;
}

template <typename Type>
ElementarySquareMatrix<Type>& ElementarySquareMatrix<Type>::operator=(ElementarySquareMatrix<Type>&& m)
{
    if(this == &m)
    {
        return *this;
    }
    std::swap(elements, m.elements);
    setSize(m.getSize()); // Ei voi heittaa poikkeusta
    return *this;
}

template <typename Type>
SquareMatrix* ElementarySquareMatrix<Type>::clone() const
{
    return new ElementarySquareMatrix<Type>{*this};
}

template <typename Type>
ElementarySquareMatrix<Type> ElementarySquareMatrix<Type>::transpose() const
{
    std::vector< std::vector<std::unique_ptr<Type>> > temp{getSize()};
    for(const auto& i : getElem())
    {
        auto iter = temp.begin();
        std::for_each(i.cbegin(), i.cend(), [&iter](const std::unique_ptr<Type>& j)
        {
            iter->push_back(std::unique_ptr<Type> {dynamic_cast<Type*>(j->clone())});
            iter++;
        });
    }
    ElementarySquareMatrix<Type> matriisi;
    std::swap(matriisi.elements, temp);
    matriisi.setSize(getSize()); // Ei voi heittaa poikkeusta
    return matriisi;
}

template <>
inline ElementarySquareMatrix<IntElement>& ElementarySquareMatrix<IntElement>::operator+=(const ElementarySquareMatrix<IntElement>& m)
{
    if(getSize() != m.getSize())
    {
        throw std::invalid_argument{"Neliomatriisien taytyy olla samaa kokoa."};
    }

    std::vector<std::thread> vector_t;
    auto iter_1 = m.getElem().cbegin();
    for(auto& i : elements)
    {
        vector_t.push_back(std::thread{vectorOp, std::ref(i), std::ref(*iter_1), std::plus<IntElement>{}});
        iter_1++;
    }
    for(auto& thr : vector_t)
    {
        thr.join();
    }

    return *this;
}

template <>
inline ElementarySquareMatrix<IntElement>& ElementarySquareMatrix<IntElement>::operator-=(const ElementarySquareMatrix<IntElement>& m)
{
    if(getSize() != m.getSize())
    {
        throw std::invalid_argument{"Neliomatriisien taytyy olla samaa kokoa."};
    }

    auto iter_1 = m.getElem().cbegin();
    std::vector<std::thread> vector_t;
    for(auto& i : elements)
    {
        vector_t.push_back(std::thread{vectorOp, std::ref(i), std::ref(*iter_1), std::minus<IntElement>{}});
        iter_1++;
    }
    for(auto& thr : vector_t)
    {
        thr.join();
    }

    return *this;
}

template <>
inline ElementarySquareMatrix<IntElement>& ElementarySquareMatrix<IntElement>::operator*=(const ElementarySquareMatrix<IntElement>& m)
{
    if(getSize() != m.getSize())
    {
        throw std::invalid_argument{"Neliomatriisien taytyy olla samaa kokoa."};
    }

    ElementarySquareMatrix<IntElement> transp = m.transpose();
    const auto& temp = transp.getElem();
    std::vector< std::vector<std::unique_ptr<IntElement>> > matriisi{getSize()};
    std::vector<std::thread> vector_t;
    auto iter_matr = matriisi.begin();
    for(const auto& i : elements)
    {
        vector_t.push_back(std::thread{vectorMult, std::ref(*iter_matr), std::ref(i), std::ref(temp)});
        iter_matr++;
    }
    for(auto& thr : vector_t)
    {
        thr.join();
    }
    std::swap(matriisi, elements);
    return *this;
}

template <typename Type>
bool ElementarySquareMatrix<Type>::operator==(const ElementarySquareMatrix<Type>& m) const
{
    if(getSize() != m.getSize())
    {
        return false;
    }

    auto iter_1 = m.getElem().cbegin();
    for(const auto& i : elements)
    {
        auto iter_2 = iter_1->cbegin();
        for(const auto& j : i)
        {
            if(j->toString() != (*iter_2)->toString())
            {
                return false;
            }
            iter_2++;
        }
        iter_1++;
    }
    return true;
}

template <typename Type>
void ElementarySquareMatrix<Type>::print(std::ostream& os) const
{
    os<< "[";
    for(auto i = getElem().cbegin(); i != getElem().cend(); i++)
    {
        os << "[";
        for(auto j = i->cbegin(); j != i->cend(); j++)
        {
            if(j+1 != i->cend())
            {
                os << **j << ",";
            }
            else
            {
                os << **j << "]";
            }
        }
    }
    os << "]";
}

template <typename Type>
std::string ElementarySquareMatrix<Type>::toString() const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}

template <>
inline ElementarySquareMatrix<IntElement> ElementarySquareMatrix<IntElement>::evaluate(const Valuation& valuation) const
{
    return *this;
}

template <typename Type>
ElementarySquareMatrix<IntElement> ElementarySquareMatrix<Type>::evaluate(const Valuation& valuation) const
{
    std::string str = "[";
    for(const auto& row : getElem())
    {
        str += "[";
        for(auto col = row.cbegin(); col != row.cend(); col++)
        {
            std::stringstream ss;
            ss << (*col)->evaluate(valuation);
            str += ss.str();
            if(col+1 != row.cend())
            {
                str += ",";
            }
            else
            {
                str += "]";
            }
        }
    }
    str += "]";
    return ElementarySquareMatrix<IntElement>{str};
}

#endif // ELEMENTARYMATRIX_H
