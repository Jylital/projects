/** \file Element.cpp
  * \brief Toteutustiedosto luokalle Element
  */

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <exception>
#include "catch.hpp"
#include "Element.h"

IntElement operator+(const IntElement& i_1, const IntElement& i_2)
{
    return IntElement{i_1.getVal() + i_2.getVal()};
}

IntElement operator-(const IntElement& i_1, const IntElement& i_2)
{
    return IntElement{i_1.getVal() - i_2.getVal()};
}

IntElement operator*(const IntElement& i_1, const IntElement& i_2)
{
    return IntElement{i_1.getVal() * i_2.getVal()};
}

std::ostream& operator<<(std::ostream& out, const Element& e)
{
    e.print(out);
    return out;
}

TEST_CASE("Testing variable-elements", "[variable][element]")
{
    VariableElement ve1;
    CHECK(ve1.getVal() == 0);

    VariableElement ve2{'y'};
    CHECK(ve2.getVal() == 'y');

    ve1.setVal('y');
    CHECK(ve1.getVal() == 'y');

    Element* ve3 = ve1.clone();
    CHECK(ve3->toString() == "y");
    delete ve3;

    std::stringstream ss1;
    ve1.print(ss1);
    CHECK(ss1.str() == "y");

    Valuation val;
    val['y'] = 10;
    CHECK(ve1.evaluate(val) == 10);

    ve2.setVal('z');
    CHECK_THROWS(ve2.evaluate(val));

    CHECK(!(ve1 == ve2));
    CHECK(ve1 == ve1);

    std::stringstream ss2;
    ss2 << ve1;
    CHECK(ve1.toString() == ss2.str());
}

TEST_CASE("Testing int-elements", "[int][element]")
{
    IntElement ie1;
    CHECK(ie1.getVal() == 0);

    IntElement ie2{2};
    CHECK(ie2.getVal() == 2);

    ie1.setVal(4);
    CHECK(ie1.getVal() == 4);

    Element* cie = ie1.clone();
    CHECK(cie->toString() == "4");
    delete cie;

    std::stringstream ss1;
    ie1.print(ss1);
    CHECK(ss1.str() == "4");

    Valuation val;
    val['y'] = 10;
    CHECK(ie1.evaluate(val) == 4);

    ie1 += ie2;
    CHECK(ie1.getVal() == 6);
    CHECK((ie1-ie2).getVal() == 4);

    ie1 -= ie2;
    CHECK(ie1.getVal() == 4);
    CHECK((ie1+ie2).getVal() == 6);

    ie1 *= ie2;
    CHECK(ie1.getVal() == 8);

    IntElement ie3 = ie1 * ie2;
    CHECK(ie3.getVal() == 16);

    CHECK(ie3 == IntElement(ie3));
    CHECK(!(ie3 == ie2));

    std::stringstream ss2;
    ss2 << ie3;
    CHECK(ie3.toString() == ss2.str());
}
