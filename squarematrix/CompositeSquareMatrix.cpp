/** \file CompositeSquareMatrix.cpp
  * \brief CompositeSquareMatrix-luokan toteutustiedosto
  */

#include <sstream>
#include "CompositeSquareMatrix.h"
#include "catch.hpp"

CompositeSquareMatrix::CompositeSquareMatrix(const SquareMatrix& op1, const SquareMatrix& op2,
                                             const std::function<ConcreteSquareMatrix(const ConcreteSquareMatrix&, const ConcreteSquareMatrix&)>& opr, char opc)
                                             : oprnd1{std::unique_ptr<SquareMatrix> (op1.clone())}, oprnd2{std::unique_ptr<SquareMatrix> (op2.clone())}, oprtor{opr}, op_char{opc}
{
}

CompositeSquareMatrix::CompositeSquareMatrix(const CompositeSquareMatrix& m) : oprtor{m.oprtor}, op_char{m.op_char}
{
    oprnd1 = std::unique_ptr<SquareMatrix> {m.oprnd1->clone()};
    oprnd2 = std::unique_ptr<SquareMatrix> {m.oprnd2->clone()};
}

CompositeSquareMatrix::CompositeSquareMatrix(CompositeSquareMatrix&& m) : oprtor{m.oprtor}, op_char{m.op_char}
{
    std::swap(oprnd1, m.oprnd1);
    std::swap(oprnd2, m.oprnd2);
}

CompositeSquareMatrix& CompositeSquareMatrix::operator=(const CompositeSquareMatrix& m)
{
    if(this == &m)
    {
        return *this;
    }

    oprnd1 = std::unique_ptr<SquareMatrix> {m.oprnd1->clone()};
    oprnd2 = std::unique_ptr<SquareMatrix> {m.oprnd2->clone()};
    oprtor = m.oprtor;
    op_char = m.op_char;
    return *this;
}

CompositeSquareMatrix& CompositeSquareMatrix::operator=(CompositeSquareMatrix&& m)
{
    if(this == &m)
    {
        return *this;
    }

    std::swap(oprnd1, m.oprnd1);
    std::swap(oprnd2, m.oprnd2);
    oprtor = m.oprtor;
    op_char = m.op_char;
    return *this;
}

SquareMatrix* CompositeSquareMatrix::clone() const
{
    return new CompositeSquareMatrix{*this};
}

void CompositeSquareMatrix::print(std::ostream& os) const
{
    os << '(' << *oprnd1 << op_char << *oprnd2 << ')';
}

std::string CompositeSquareMatrix::toString() const
{
    std::stringstream ss;
    print(ss);
    return ss.str();
}

ConcreteSquareMatrix CompositeSquareMatrix::evaluate(const Valuation& val) const
{
    return oprtor(oprnd1->evaluate(val), oprnd2->evaluate(val));
}

std::ostream& operator<<(std::ostream& out, const SquareMatrix& m)
{
    m.print(out);
    return out;
}

TEST_CASE("Testing composite matrices", "[composite][squarematrix]")
{
    SymbolicSquareMatrix sm_1{"[[1,x][y,4]]"};
    ConcreteSquareMatrix sm_2{"[[5,6][7,8]]"};
    std::plus<ConcreteSquareMatrix> oprtor_1{};
    CompositeSquareMatrix csm_1{sm_1, sm_2, oprtor_1, '+'};
    CHECK(csm_1.toString() == "([[1,x][y,4]]+[[5,6][7,8]])");
    Valuation val;
    val['x'] = 2;
    val['y'] = 3;
    ConcreteSquareMatrix sm_3 = csm_1.evaluate(val);
    CHECK(sm_3.toString() == "[[6,8][10,12]]");

    ConcreteSquareMatrix sm_4{200};
    ConcreteSquareMatrix sm_5{200};
    std::multiplies<ConcreteSquareMatrix> oprtor_2{};
    CompositeSquareMatrix csm_2{sm_4, sm_5, oprtor_2, '*'};
    ConcreteSquareMatrix sm_6 = csm_2.evaluate(val);

    CompositeSquareMatrix csm_3{csm_1};
    CHECK(csm_3.toString() == "([[1,x][y,4]]+[[5,6][7,8]])");

    CompositeSquareMatrix csm_4{std::move(csm_3)};
    CHECK(csm_4.toString() == "([[1,x][y,4]]+[[5,6][7,8]])");

    SymbolicSquareMatrix sm_7{"[[1,x][y,4]]"};
    ConcreteSquareMatrix sm_8{"[[5,6][7,8]]"};
    std::minus<ConcreteSquareMatrix> oprtor_3{};
    CompositeSquareMatrix csm_5{sm_7, sm_8, oprtor_3, '-'};
    csm_3 = csm_5;
    CHECK(csm_3.toString() == "([[1,x][y,4]]-[[5,6][7,8]])");
    csm_3 = csm_3;
    CHECK(csm_3.toString() == "([[1,x][y,4]]-[[5,6][7,8]])");

    CompositeSquareMatrix csm_6{csm_1, csm_5, oprtor_2, '*'};
    CHECK(csm_6.toString() == "(([[1,x][y,4]]+[[5,6][7,8]])*([[1,x][y,4]]-[[5,6][7,8]]))");
    ConcreteSquareMatrix sm_val = csm_6.evaluate(val);
    CHECK(sm_val.toString() == "[[-56,-56][-88,-88]]");

    csm_4 = std::move(csm_5);
    CHECK(csm_4.toString() == "([[1,x][y,4]]-[[5,6][7,8]])");
    csm_4 = std::move(csm_4);
    CHECK(csm_4.toString() == "([[1,x][y,4]]-[[5,6][7,8]])");

    SquareMatrix* sm_9 = csm_4.clone();
    CHECK(sm_9->toString() == "([[1,x][y,4]]-[[5,6][7,8]])");
    delete sm_9;

    std::stringstream ss;
    csm_1.print(ss);
    CHECK(ss.str() == "([[1,x][y,4]]+[[5,6][7,8]])");
    ss.str("");
    ss << csm_1;
    CHECK(ss.str() == "([[1,x][y,4]]+[[5,6][7,8]])");
}

TEST_CASE("Testing exceptions for composite matrices", "[composite][squarematrix][exception]")
{
    SymbolicSquareMatrix sm_1{"[[1,x][y,4]]"};
    ConcreteSquareMatrix sm_2{"[[5,6][7,8]]"};
    std::plus<ConcreteSquareMatrix> oprtor_1{};
    CompositeSquareMatrix csm_1{sm_1, sm_2, oprtor_1, '+'};
    Valuation val;
    val['x'] = 2;
    CHECK_THROWS(csm_1.evaluate(val));
}
