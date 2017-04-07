/** \file main.cpp
  * \brief Kayttoliittyma matriisilaskimelle
  */

#include <stack>
#include <string>
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "SquareMatrix.h"
#include "CompositeSquareMatrix.h"

/** \brief main-funktio
  * \return Palauttaa sen, kuinka moni testi epaonnistuu
  *
  * Matriisilaskimen kayttoliittyma, jolla kayttaja voi laskea eri matriiseille toteutettavia peruslaskutoimituksia +, - ja *.
  */
int main(int argc, char* argv[])
{
    int result = Catch::Session().run( argc, argv );
    std::string syote;
    std::cout << "i. Syota matriisi muodossa [[1,x][y,4]], jos haluat luoda matriisin pinoon" << std::endl;
    std::cout << "ii. Syota \"+\", \"-\" tai \"*\", jos haluat operaation tehtavan kahdelle paalimmaiselle matriisille" << std::endl;
    std::cout << "iii. Syota \"=\", jos haluat evaluoida pinon paalimmaisen matriisin" << std::endl;
    std::cout << "iv. Syota merkki, =-merkki ja kokonaisluku, jos haluat muuttaa valuaatiota" << std::endl;
    std::cout << "v. Syota \"quit\", jos haluat lopettaa" << std::endl;
    std::cout << "Syote > ";
    std::getline(std::cin, syote);
    std::stack<std::unique_ptr<SquareMatrix>> pino;
    Valuation val;
    while(syote != "quit")
    {
        if((syote == "+") || (syote == "-") || (syote == "*"))
        {
            if(pino.size() < 2)
            {
                std::cout << "Pinossa ei ole riittavasti matriiseja" << std::endl;
            }
            else
            {
                SquareMatrix* sm_1 {(pino.top())->clone()};
                pino.pop();
                SquareMatrix* sm_2 {(pino.top())->clone()};
                pino.pop();
                if(syote == "+")
                {
                    pino.push(std::unique_ptr<SquareMatrix> {new CompositeSquareMatrix{*sm_1, *sm_2, std::plus<ConcreteSquareMatrix>{}, '+'}});
                    pino.top()->print(std::cout);
                    std::cout << std::endl;
                }
                if(syote == "-")
                {
                    pino.push(std::unique_ptr<SquareMatrix> {new CompositeSquareMatrix{*sm_1, *sm_2, std::minus<ConcreteSquareMatrix>{}, '-'}});
                    pino.top()->print(std::cout);
                    std::cout << std::endl;
                }
                if(syote == "*")
                {
                    pino.push(std::unique_ptr<SquareMatrix> {new CompositeSquareMatrix{*sm_1, *sm_2, std::multiplies<ConcreteSquareMatrix>{}, '*'}});
                    pino.top()->print(std::cout);
                    std::cout << std::endl;
                }
                delete sm_1;
                delete sm_2;
            }
        }
        else if(syote == "=")
        {
            if(pino.size() == 0)
            {
                std::cout << "Pinossa ei ole riittavasti matriiseja" << std::endl;
            }
            else
            {
                try
                {
                    pino.top()->evaluate(val).print(std::cout);
                    std::cout << std::endl;
                }
                catch(const std::exception& e)
                {
                    std::cout << "Matriisia ei saatu evaluoitua" << std::endl;
                }
            }
        }
        else if(std::isalpha(syote.front()))
        {
            std::stringstream ss{syote};
            char c = ' ';
            char s = ' ';
            ss >> c;
            ss >> s;
            if(!ss.good() || s != '=')
            {
                std::cout << "Syote ei ole oikeaa muotoa" << std::endl;
            }
            else
            {
                int num = 0;
                ss >> num;
                if(ss.fail() || !ss.eof())
                {
                    std::cout << "Syote ei ole oikeaa muotoa" << std::endl;
                }
                else
                {
                    val[c] = num;
                    std::cout << "Valuaatiota muutettiin" << std::endl;
                }
            }
        }
        else
        {
            try
            {
                pino.push(std::unique_ptr<SquareMatrix> {new SymbolicSquareMatrix{syote}});
                std::cout << "Matriisi lisattiin pinoon" << std::endl;
            }
            catch(const std::invalid_argument& ia)
            {
                std::cout << "Syote ei ole oikeaa muotoa" << std::endl;
            }
        }
        std::cout << "Syote > ";
        std::getline(std::cin, syote);
    }

    return ( result < 0xff ? result : 0xff );
}
