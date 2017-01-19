// This file is part of Jiminy.
// 
// Jiminy is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Jiminy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Jiminy.  If not, see <http://www.gnu.org/licenses/>.

#include <iostream>
#include <cmath>

#include <Box2D/Box2D.h>

#include "Brain.hpp"


Brain::Brain() :
        mQ(arma::zeros<arma::mat>(91, 201)), // theta, x
        mS(45),
        mA(100),
        mSimulated(false)
{
//    for (int i = 0; i < 401; ++i)
//    {
//        for (int j = 0; j < 81; ++j)
//        {
//            mQ(i, j) = j / 80.f;
//        }
//    }

    mlpack::data::Load("Q.csv", mQ);

//    mQ = arma::randg<arma::mat>(91, 201, arma::distr_param(2, 1));// + 15.f;
}

Brain::~Brain()
{
    if (mSimulated)
    {
        mlpack::data::Save("Q.csv", mQ);
        std::cout << "Saving..." << std::endl;
    }

//    if (mSimulated)
//    {
//        for (int i = 0; i < 401; ++i)
//        {
//            for (int j = 0; j < 81; ++j)
//            {
//                std::cout << mQ(i, j) << " ";
//            }
//            std::cout << "\n";
//        }
//        std::cout << std::flush;
//
//    }
}

float stotheta(const arma::uword &s)
{
    return 2.f * float(s) - 90.f;
}

arma::uword thetatos(float theta)
{
    return (arma::uword) (theta + 90) / 2;
}

float sgn(float a)
{
    return (a < 0) ? -1.f : 1.f;
}

bool absorb(const arma::uword &s)
{
    float theta = stotheta(s);
    return abs(theta) > 70;
}

float r(float theta)//const arma::uword &s)
{
//    float theta = -stotheta(s);
    theta -= 1;
    float r = 1.f - abs(theta) / 40.f;//2.f;
    if (r < 0)
    {
//        r /= 10.f;
    }
    else
    {
        r *= r;
    }
    return r;
//    return -abs(theta) / 40.f + 1.f;
}

template<typename T>
T min(T a, T b)
{
    return (a < b) ? a : b;
}

template<typename T>
T max(T a, T b)
{
    return (a > b) ? a : b;
}

float mod_theta(float theta)
{
    while (theta <= 0)
    {
        theta += 360;
    }
    while (theta > 360)
    {
        theta -= 360;
    }

    if (theta > 180)
    {
        theta -= 360;
    }

    return theta;
}

void Brain::Think()
{
    mSimulated = true;
//    float theta = mInfo["theta"]() * (180.f / b2_pi);// + 90.f;
//    mActions["x."](force);

    std::cout << '\r';

    static constexpr float gamma = 0.99f;
    static constexpr float alpha = 1.1f;

    float theta = mInfo["theta"]() * (180.f / b2_pi);
    theta = mod_theta(theta);
    std::cout << "th=" << std::setw(9) << theta << ", ";

//    if (!absorb(mS))
//    {
    std::cout << "s=" << std::setw(4) << mS << ", "
              << "a=" << std::setw(4) << mA << ", ";

    // Execute A
    float force = (float(mA) - 100) / 2.f;
    if (mA <= 3 || mA >= 197)
    {
        force *= 5.f;
    }
//    float force = float(mA) / 5.f * (-theta / 90.f);
    mActions["x.."](force);
    std::cout << "F=" << std::setw(9) << force << ", ";

    // Observe S'
    arma::uword sp = thetatos(theta);
    sp = min(max(sp, arma::uword(0)), arma::uword(90));

    std::cout << "sp=" << std::setw(4) << sp << ", ";

    // a' <- argmax(Q(s',a))
    arma::uword ap = (arma::uword) mQ.row(sp).index_max();

    std::cout << "ap=" << std::setw(4) << ap << ", ";

    // Q(s,a) <- Q(s,a) + alpha * (R(s') + gamma*Q(s',a') - Q(s,a))
    float rS = r(theta);//r(sp);
    std::cout << "R=" << std::setw(9) << rS << ", ";
    float DQ = alpha * (rS + gamma * mQ(sp, ap) - mQ(mS, mA));
    std::cout << "Q=" << std::setw(9) << mQ(sp, ap) << ", ";
    std::cout << "DQ=" << std::setw(9) << DQ << ", ";
    mQ(mS, mA) += DQ;
    mQ(mS, mA) = min(max(mQ(mS, mA), -100.), 100.);

    mS = sp;
    mA = ap;

//    std::cout << '\n';
    std::cout << std::flush;
//    }
}

void Brain::ThinkOld()
{
    float theta = mInfo["theta"]() * (180.f / b2_pi);// + 90.f;
    float thetaP = mInfo["theta."]();
    float x = mInfo["x"]();

    float force = -6.f * theta - 20.f * x * ((abs(x) > 30) ? 1 : 0);

    std::cout << "\n"
              << "theta=" << theta << ", "
              << "x=" << abs(x) << ", "
              //              << "thetaP=" << thetaP << ", "
              //              << "error=" << currentError << ", "
              << "force=" << force << std::endl;

    mActions["x."](force);
//    mActions["theta."](thetaP);
}
