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

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>

#include <Box2D/Box2D.h>

#include "Brain_ContinuousQLearning.hpp"
#include "utils.hpp"


Brain_ContinuousQLearning::Brain_ContinuousQLearning() :
        mTheta(arma::zeros<arma::mat>(NB_A, DIM_S)),
        mS(arma::zeros<arma::vec>(DIM_S)),
        mA(NB_A / 2),
        mGP()
{
//    mQ.load("Q.jld", arma::file_type::arma_ascii);

    mTheta = arma::randu < arma::mat > (NB_A, DIM_S) * 1. - 0.5;
}

Brain_ContinuousQLearning::~Brain_ContinuousQLearning()
{
    Save();
}

void Brain_ContinuousQLearning::Save() const
{
    if (mSimulated)
    {
        mlpack::data::Save("theta.csv", mTheta);

//        mQ.save("Q.jld", arma::file_type::arma_ascii);
//        std::cout << "\nSaved." << std::endl;
//
//        for (arma::uword i = 0; i < NB_S_THETA_P; ++i)
//        {
//            std::stringstream filename;
//            filename << "Q" << std::setw(2) << std::setfill('0') << i << ".csv";
//            mlpack::data::Save(filename.str(), mQ.slice(i));
//        }

        std::cout << "Saved." << std::endl;
    }
}

static constexpr bool absorb(float theta, float x)
{
    return abs(theta) > 80.f || abs(x) > 180.f;
}

static double r(const arma::vec &sp)
{
    const double &theta(sp(0));
    const double &thetaP(sp(1));
    const double &x(sp(2));
    const double &xP(sp(3));

    if (abs(theta) < 0.5f && abs(thetaP) < 0.1f && abs(xP) < 0.1f) return 10.;
    double tp_surplus = max(1. - abs(thetaP), 0.) / 2.;
    double xp_surplus = max(4. - abs(xP), 0.) / 3.;
    double theta_r = 1. - abs(theta) / 5.;
    double x_r = (1. - abs(x) / 180.) * 0.1f;
    return x_r + theta_r + tp_surplus + xp_surplus;
}

void Brain_ContinuousQLearning::think()
{
    // Execute A
    float force = (float(mA) - float(NB_A / 2)) / 2.f;
    if (mA <= 3 || mA >= NB_A - 3)
    {
        force *= 5.f;
    }
    mActions["x.."](force);

    mGP << "splot " << mGP.file2d(mTheta) << "with lines title 'theta'\n";
    mGP.flush();

//    std::cout << std::setw(4) << mA << '|'
//              << std::setw(9) << mS(1)
//              << std::setw(11) << mS(2)
//              << std::setw(9) << mS(3)
//              << std::setw(9) << mS(4) << '|';

    // Observe S'
    const float theta = mod_theta(mInfo["theta"]() * (180.f / b2_pi));
    const float thetaP = mInfo["theta."]();
    const float x = mInfo["x"]();
    const float xP = mInfo["x."]();
    arma::vec sp({1, theta, thetaP, x, xP});

    // Calculate a'
    arma::vec Q(mTheta * mS);
    arma::uword ap(Q.index_max());
//    std::cout << std::setw(4) << ap << '|'
//              << std::setw(9) << theta
//              << std::setw(11) << thetaP
//              << std::setw(9) << x
//              << std::setw(9) << xP;

    // Learning
    const double alpha(0.0001);
    const double gamma(0.00099);
    //                          dQ/dt           R(s')           < max_a'(Q(a',s'))    > - Q
//    mTheta.row(mA) += alpha * mTheta.row(mA) * (r(sp) + gamma * (mTheta * sp).max() - Q);
    for (arma::uword i_s = 0; i_s < DIM_S; ++i_s)
    {
        mTheta(mA, i_s) += alpha * mTheta(mA, i_s) * (r(sp) + gamma * (mTheta * sp).max() - Q(i_s));
    }
    mTheta = arma::normalise(mTheta);


//    for (auto it = mLearningQueue.crbegin(); it != mLearningQueue.crend(); ++it)
//    {
//        arma::uword a(*it);
//        for (arma::uword i_s = 0; i_s < DIM_S; ++i_s)
//        {
//            mTheta(a, i_s) += alpha * mTheta(a, i_s) * (r(sp) + gamma * (mTheta * sp).max() - Q(i_s));
//        }
//    }
//
//    mLearningQueue.push_back(ap);
//    if (mLearningQueue.size() > learning_offset)
//    {
//        mLearningQueue.pop_front();
//    }

    // Save new state
    mS = sp;
    mA = ap;

    // Is this state final ?
    if (absorb(theta, x))
    {
        mFailed = true;
    }
}

void Brain_ContinuousQLearning::Reseted()
{
    Brain::Reseted();
    mLearningQueue.clear();

    const float theta = mod_theta(mInfo["theta"]() * (180.f / b2_pi));
    const float thetaP = mInfo["theta."]();
    const float x = mInfo["x"]();
    const float xP = mInfo["x."]();
    arma::vec mS({1, theta, thetaP, x, xP});
    mA = NB_A / 2;
}
