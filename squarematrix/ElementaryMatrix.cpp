/** \file ElementaryMatrix.cpp
  * \brief ElementarySquareMatrix-malliluokan toteutustiedosto
  */

#include <iostream>
#include <stdexcept>
#include <exception>
#include <sstream>
#include "catch.hpp"
#include "ElementaryMatrix.h"

ConcreteSquareMatrix operator+(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2)
{
    ConcreteSquareMatrix m_3 {m_1};
    m_3 += m_2;
    return m_3;
}

ConcreteSquareMatrix operator-(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2)
{
    ConcreteSquareMatrix m_3 {m_1};
    m_3 -= m_2;
    return m_3;
}

ConcreteSquareMatrix operator*(const ConcreteSquareMatrix& m_1, const ConcreteSquareMatrix& m_2)
{
    ConcreteSquareMatrix m_3 {m_1};
    m_3 *= m_2;
    return m_3;
}

void vectorOp(std::vector<std::unique_ptr<IntElement>>& leftop, const std::vector<std::unique_ptr<IntElement>>& rightop,
              const std::function<IntElement(const IntElement&, const IntElement&)>& opr)
{
    if(leftop.size() != rightop.size())
    {
        throw std::invalid_argument("Matriisien koot eivat ole yhteensopivat.");
    }

    auto iter = rightop.cbegin();
    for(auto& el : leftop)
    {
        *el = opr(*el, **iter);
        iter++;
    }
}

void vectorMult(std::vector<std::unique_ptr<IntElement>>& product, const std::vector<std::unique_ptr<IntElement>>& leftop, const std::vector<std::vector<std::unique_ptr<IntElement>>>& rightop)
{
    if(leftop.size() != rightop.size())
    {
        throw std::invalid_argument{"Argumentit eivat ole oikeaa muotoa."};
    }
    auto sz = leftop.size();
    if(std::any_of(rightop.cbegin(), rightop.cend(), [sz](const std::vector<std::unique_ptr<IntElement>>& vect){return(vect.size() != sz);}))
    {
        throw std::invalid_argument{"Argumentit eivat ole oikeaa muotoa."};
    }

    for(const auto& elem_1 : rightop)
    {
        IntElement sum;
        auto iter_fact = leftop.cbegin();
        for(const auto& elem_2 : elem_1)
        {
            sum += (**iter_fact) * (*elem_2);
            iter_fact++;
        }
        product.push_back(std::unique_ptr<IntElement> {new IntElement{sum}});
    }
}

std::vector<std::unique_ptr<IntElement>> randomVect(unsigned int m)
{
    auto seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator {seed};
    std::uniform_int_distribution<int> distribution{-100,100};
    std::vector<std::unique_ptr<IntElement>> vect;
    for(unsigned int j = 0; j < m; j++)
    {
        vect.push_back(std::unique_ptr<IntElement> {new IntElement{distribution(generator)}});
    }
    return vect;
}

TEST_CASE("Testing concrete matrices", "[concrete][squarematrix]")
{
    ConcreteSquareMatrix sm_1;
    ConcreteSquareMatrix sm_2{sm_1};

    CHECK(sm_1.toString() == "[[0,0][0,0]]");
    CHECK(sm_2.toString() == "[[0,0][0,0]]");
    CHECK(sm_1.getSize() == 2);

    const auto& vekt = sm_1.getElem();
    CHECK(*((vekt[0])[0]) == IntElement{0});
    CHECK(*((vekt[0])[1]) == IntElement{0});
    CHECK(*((vekt[1])[0]) == IntElement{0});
    CHECK(*((vekt[1])[1]) == IntElement{0});

    sm_1.setSize(2);
    CHECK(sm_1.getSize() == 2);

    sm_1 = ConcreteSquareMatrix{"[[1,2][3,4]]"};
    CHECK(sm_1.toString() == "[[1,2][3,4]]");

    sm_1 = sm_1;
    CHECK(sm_1.toString() == "[[1,2][3,4]]");

    sm_2 = ConcreteSquareMatrix{"[[5,6][7,8]]"};
    CHECK((sm_1 + sm_2).toString() == "[[6,8][10,12]]");
    sm_1 += sm_2;
    CHECK(sm_1.toString() == "[[6,8][10,12]]");

    sm_1 = ConcreteSquareMatrix{"[[1,2][3,4]]"};
    CHECK((sm_1 - sm_2).toString() == "[[-4,-4][-4,-4]]");
    sm_1 -= sm_2;
    CHECK(sm_1.toString() == "[[-4,-4][-4,-4]]");

    sm_1 = ConcreteSquareMatrix {"[[1,2][3,4]]"};
    CHECK((sm_1 * sm_2).toString() == "[[19,22][43,50]]");
    sm_1 *= sm_2;
    CHECK(sm_1.toString() == "[[19,22][43,50]]");

    CHECK(sm_1.transpose().toString() == "[[19,43][22,50]]");

    ConcreteSquareMatrix sm_3{"[[1]]"};
    CHECK(!(sm_1 == sm_3));
    CHECK(!(sm_1 == sm_2));
    CHECK(sm_1 == sm_1);
    ConcreteSquareMatrix sm_3_t{"[[2]]"};
    sm_3 = sm_3_t;
    CHECK(sm_3.toString() == "[[2]]");

    std::stringstream ss;
    sm_1.print(ss);
    CHECK(ss.str() == "[[19,22][43,50]]");

    ConcreteSquareMatrix sm_4{std::move(ConcreteSquareMatrix{})};
    CHECK(sm_4.toString() == "[[0,0][0,0]]");

    ConcreteSquareMatrix sm_5 = std::move(sm_4);
    CHECK(sm_5.toString() == "[[0,0][0,0]]");

    sm_5 = std::move(sm_5);
    CHECK(sm_5.toString() == "[[0,0][0,0]]");

    auto start_t = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
    ConcreteSquareMatrix sm_6 {300};
    auto end_t = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
    std::cout << "Satunnaisen 300x300-matriisin luomiseen kului " << end_t.count() - start_t.count() << "ms." << std::endl;
    CHECK(sm_6.getSize() == 300);
    ConcreteSquareMatrix sm_6_f {300};
    start_t = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
    CHECK((sm_6 * sm_6_f).getSize() == 300);
    end_t = std::chrono::duration_cast<std::chrono::milliseconds> (std::chrono::system_clock::now().time_since_epoch());
    std::cout << "300x300-kokoisten matriisien kertolaskuun kului " << end_t.count() - start_t.count() << "ms." << std::endl;

    SquareMatrix* sm_7 = sm_5.clone();
    CHECK(sm_7->toString() == "[[0,0][0,0]]");
    delete sm_7;

    Valuation val;
    ConcreteSquareMatrix sm_8 = sm_5.evaluate(val);
    CHECK(sm_8.toString() == "[[0,0][0,0]]");
}

TEST_CASE("Testing exceptions for concrete matrices", "[concrete][squarematrix][exception]")
{
    CHECK_THROWS(ConcreteSquareMatrix sm_1{""});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"["});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[["});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1,"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1,2"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1,2]"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{","});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[,"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[,"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1."});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1],"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1,2][3,4][5,6]]"});
    CHECK_THROWS(ConcreteSquareMatrix sm_1{"[[1,x][y,4]]"});

    ConcreteSquareMatrix sm_1{"[[1,2][3,4]]"};
    CHECK_THROWS(sm_1.setSize(4));

    ConcreteSquareMatrix sm_2{"[[1]]"};
    CHECK_THROWS(sm_1 += sm_2);
    CHECK_THROWS(sm_1 -= sm_2);
    CHECK_THROWS(sm_1 *= sm_2);
    CHECK_THROWS(sm_1 + sm_2);
    CHECK_THROWS(sm_1 - sm_2);
    CHECK_THROWS(sm_1 * sm_2);

    CHECK_THROWS(ConcreteSquareMatrix {-10});
}

TEST_CASE("Testing symbolic matrices", "[symbolic][squarematrix]")
{
    SymbolicSquareMatrix sm_1;
    SymbolicSquareMatrix sm_2{sm_1};

    CHECK(sm_1.toString() == "[[0,0][0,0]]");
    CHECK(sm_2.toString() == "[[0,0][0,0]]");
    CHECK(sm_1.getSize() == 2);

    const auto& vekt = sm_1.getElem();
    CHECK(*dynamic_cast<IntElement*>(((vekt[0])[0]).get()) == IntElement{0});
    CHECK(*dynamic_cast<IntElement*>(((vekt[0])[1]).get()) == IntElement{0});
    CHECK(*dynamic_cast<IntElement*>(((vekt[1])[0]).get()) == IntElement{0});
    CHECK(*dynamic_cast<IntElement*>(((vekt[1])[1]).get()) == IntElement{0});

    sm_1.setSize(2);
    CHECK(sm_1.getSize() == 2);

    SymbolicSquareMatrix sm_1_t{"[[1,x][y,4]]"};
    sm_1 = sm_1_t;
    CHECK(sm_1.toString() == "[[1,x][y,4]]");

    sm_1 = sm_1;
    CHECK(sm_1.toString() == "[[1,x][y,4]]");

    CHECK(sm_1.transpose().toString() == "[[1,y][x,4]]");

    SymbolicSquareMatrix sm_3{"[[1]]"};
    CHECK(!(sm_1 == sm_3));
    CHECK(!(sm_1 == sm_2));
    CHECK(sm_1 == sm_1);

    std::stringstream ss;
    sm_1.print(ss);
    CHECK(ss.str() == "[[1,x][y,4]]");

    SymbolicSquareMatrix sm_4{std::move(SymbolicSquareMatrix{})};
    CHECK(sm_4.toString() == "[[0,0][0,0]]");

    SymbolicSquareMatrix sm_5 = std::move(sm_4);
    CHECK(sm_5.toString() == "[[0,0][0,0]]");

    sm_5 = std::move(sm_5);
    CHECK(sm_5.toString() == "[[0,0][0,0]]");

    Valuation val;
    val['x'] = 2;
    val['y'] = 3;
    ConcreteSquareMatrix sm_6 = sm_1.evaluate(val);

    SquareMatrix* sm_7 = sm_5.clone();
    CHECK(sm_7->toString() == "[[0,0][0,0]]");
    delete sm_7;
}

TEST_CASE("Testing exceptions for symbolic matrices", "[symbolic][squarematrix][exception]")
{
    CHECK_THROWS(SymbolicSquareMatrix sm_1{""});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"["});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[["});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1,"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1,2"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1,2]"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{","});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[."});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[,"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1."});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1],"});
    CHECK_THROWS(SymbolicSquareMatrix sm_1{"[[1,2][3,4][5,6]]"});

    SymbolicSquareMatrix sm_1{"[[1,2][3,4]]"};
    CHECK_THROWS(sm_1.setSize(4));

    SymbolicSquareMatrix sm_2{"[[1]]"};

    sm_1 = SymbolicSquareMatrix{"[[1,x][y,4]]"};
    Valuation val;
    val['x'] = 2;
    CHECK_THROWS(sm_1.evaluate(val));
}
