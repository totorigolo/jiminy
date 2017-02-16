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


static const arma::uword NB_A{80};
namespace
{
    enum StateElem : arma::uword
    {
        CONSTANT,
        PHI,
        ABS_PHI,
        PHI_P,
        ABS_PHI_P,
        ABS_X,
        X_P,
        ABS_X_P,
        DIM_S
    };
}

Brain_ContinuousQLearning::Brain_ContinuousQLearning() :
        mTheta(arma::zeros<arma::mat>(NB_A, DIM_S)),
        mS(arma::zeros<arma::vec>(DIM_S)),
        mA(NB_A / 2)
{
//    mQ.load("Q.jld", arma::file_type::arma_ascii);

    do
    {
//        mTheta = arma::randu < arma::mat > (NB_A, DIM_S) * 1. - 0.5;
    } while (rand() % 100 < 80);
}

Brain_ContinuousQLearning::~Brain_ContinuousQLearning()
{
    onSave();
    mGP_theta << "exit\n";
    mGP_theta.flush();
    mGP_Q << "exit\n";
    mGP_Q.flush();
}

void Brain_ContinuousQLearning::onSave() const
{
    if (mSimulated)
    {
        mlpack::data::Save("theta.csv", mTheta);
    }
}

static inline bool absorb(const arma::vec &s)
{
    return abs(s(PHI)) > 80.f;// || abs(s(X)) > 180.f;
}

static inline double r(const arma::vec &sp)
{
    const double &phi(sp(PHI));
    const double &phiP(sp(PHI_P));
    const double &xP(sp(X_P));
    const double &abs_x(sp(ABS_X));

    if (abs(phi) < 0.5f && abs(phiP) < 0.1f && abs(xP) < 0.1f) return 10.;
    double pp_surplus = max(1. - abs(phiP), 0.) / 2.;
    double xp_surplus = max(4. - abs(xP), 0.) / 3.;
    double phi_r = 1. - abs(phi) / 5.;
    double x_r = (1. - abs_x / 180.) * 0.1f;
    return x_r + phi_r + pp_surplus + xp_surplus;

//    return max(1. - abs(phi) / 6., -0.1) / 4.;
}

void Brain_ContinuousQLearning::onThink()
{
    // Observe S'
    arma::vec sp(getS());
//    sp = arma::normalise(sp);

    // Calculate a'
    arma::vec Q(mTheta * mS);
    arma::uword ap(Q.index_max());
    std::cout << Q.max();

    // Learning
    const double alpha(0.0001);
    const double gamma(0.00099);
    //                          dQ/dt           R(s')              <max_a'(Q(a',s'))> - Q
//    mTheta.row(mA) += alpha * mTheta.row(mA) * (r(sp) + gamma * (mTheta * sp).max() - Q);
    for (arma::uword i_s = 0; i_s < DIM_S; ++i_s)
    {
        mTheta(mA, i_s) += alpha * sp(i_s) * (r(sp) + gamma * (mTheta * sp).max() - Q(i_s));
    }
    mTheta = arma::normalise(mTheta);

    // Execute A
    float force = (float(mA) - float(NB_A / 2)) / 2.f;
    if (mA <= 3 || mA >= NB_A - 3)
    {
        force *= 5.f;
    }
    mActions["x.."](force);

//    for (auto it = mLearningQueue.crbegin(); it != mLearningQueue.crend(); ++it)
//    {
//        arma::uword a(*it);
//        for (arma::uword i_s = 0; i_s < DIM_S; ++i_s)
//        {
//            mTheta(mA, i_s) += alpha * sp(i_s) * (r(sp) + gamma * (mTheta * sp).max() - Q(i_s));
//        }
//        mTheta = arma::normalise(mTheta);
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
    if (absorb(sp))
    {
        mFailed = true;
    }

    // GNU Plot
    mGP_theta << "splot '-' with lines title 'theta'\n";
    mGP_theta.send2d(mTheta);
    mGP_theta.flush();
    mGP_Q << "plot '-' with lines title 'Q'\n";
    mGP_Q.send1d(Q);
    mGP_Q.flush();
}

void Brain_ContinuousQLearning::onReseted()
{
    mLearningQueue.clear();

    mS = getS();
    mA = NB_A / 2;
}

arma::vec Brain_ContinuousQLearning::getS()
{
    const float phi = mod_theta(mInfo["theta"]() * (180.f / b2_pi));
    const float phiP = mInfo["theta."]();
    const float x = mInfo["x"]();
    const float xP = mInfo["x."]();
    return {1, phi, abs(phi), phiP, abs(phiP), abs(x), xP, abs(xP)};
//    return {1, phi};
//    return {phi};
}
