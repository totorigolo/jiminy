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
#include <cmath>

#include <Box2D/Box2D.h>

#include "Brain_DiscretQLearning.hpp"
#include "utils.hpp"


Brain_DiscretQLearning::Brain_DiscretQLearning() :
        mQ(arma::zeros<arma::cube>(NB_S_THETA, NB_A, NB_S_THETA_P)), // theta, x
        mTP(arma::zeros<arma::vec>(NB_S_THETA_P)),
        mS_theta(NB_S_THETA / 2),
        mS_thetaP(NB_S_THETA_P / 2),
        mA(NB_A / 2)
{
    mQ.load("Q.jld", arma::file_type::arma_ascii);

//    mQ = arma::randu<arma::cube>(NB_S_THETA, NB_A, NB_S_THETA_P);// * 2. + 15.;
}

Brain_DiscretQLearning::~Brain_DiscretQLearning()
{
    onSave();
}

void Brain_DiscretQLearning::onSave() const
{
    if (mSimulated)
    {
        mlpack::data::Save("TP.csv", mTP);

        mQ.save("Q.jld", arma::file_type::arma_ascii);

        for (arma::uword i = 0; i < NB_S_THETA_P; ++i)
        {
            std::stringstream filename;
            filename << "Q" << std::setw(2) << std::setfill('0') << i << ".csv";
            mlpack::data::Save(filename.str(), mQ.slice(i));
        }
    }
}

inline float Brain_DiscretQLearning::stotheta(const arma::uword &s)
{
    return 2.f * float(s) - (NB_S_THETA / 2);
}

inline arma::uword Brain_DiscretQLearning::thetatos(float theta)
{
    long r = long((theta + 90.f) / 180.f * float(NB_S_THETA));
    return (arma::uword) r;
}

inline float Brain_DiscretQLearning::stothetaP(const arma::uword &s)
{
    return 2.f * float(s) - (NB_S_THETA_P / 2);
}

arma::uword Brain_DiscretQLearning::thetaPtos(float thetaP)
{
    float t = 8.f * abs(thetaP);
    ++mTP(enbounds(arma::uword(t), arma::uword(0), NB_S_THETA_P - 1));
    return arma::uword(t);
}

static inline bool absorb(float theta, float x)
{
    return abs(theta) > 80.f || abs(x) > 180.f;
}

static double r(float theta, float thetaP, float x, float xP)
{
    if (abs(theta) < 0.5f && abs(thetaP) < 0.1f && abs(xP) < 0.1f) return 10.;
    double tp_surplus = max(1. - double(abs(thetaP)), 0.) / 2.;
    double xp_surplus = max(4. - double(abs(xP)), 0.) / 3.;
    double theta_r = 1. - abs(theta) / 5.;
    double x_r = (1. - abs(x) / 180.) * 0.1f;
    return x_r + theta_r + tp_surplus + xp_surplus;
}

void Brain_DiscretQLearning::onThink()
{
    mSimulated = true;

    std::cout << '\r';
    std::cout << "#" << std::setw(9) << ++mIteration << ", ";

    static constexpr float gamma = 0.99f;
    static constexpr float alpha = 0.1f;

    const float theta = mod_theta(mInfo["theta"]() * (180.f / b2_pi));
//    std::cout << "th=" << std::setw(9) << theta << ", ";
    const float thetaP = mInfo["theta."]();
//    std::cout << "thP=" << std::setw(12) << thetaP << ", ";
    const float x = mInfo["x"]();
//    std::cout << "xP=" << std::setw(12) << xP << ", ";
    const float xP = mInfo["x."]();
//    std::cout << "xP=" << std::setw(12) << xP << ", ";

//        std::cout << "st=" << std::setw(4) << mS_theta << ", "
//                  << "stP=" << std::setw(4) << mS_thetaP << ", "
//                  << "a=" << std::setw(4) << mA << ", ";

    // Execute A
    float force = (float(mA) - float(NB_A / 2)) / 2.f;
    if (mA <= 3 || mA >= NB_A - 3)
    {
        force *= 5.f;
    }
    mActions["x.."](force);
//        std::cout << "F=" << std::setw(9) << force << ", ";

    // Observe S'
    const arma::uword stp = enbounds(thetatos(theta), arma::uword(0), NB_S_THETA - 1);
//        std::cout << "stp=" << std::setw(4) << stp << ", ";
    const arma::uword stpp = enbounds(thetaPtos(thetaP), arma::uword(0), NB_S_THETA_P - 1);
//        std::cout << "stpp=" << std::setw(4) << stpp << ", ";

    // a' <- argmax(Q(s',a))
    constexpr bool SOME_RANDOM(false);
    arma::uword ap(0);
    if (SOME_RANDOM && rand() % 100 < 2)
    {
        ap = rand() % NB_A;
        std::cout << "Random : a'=" << ap << std::endl;
        mLearningQueue.clear();
    }
    else
    {
        double maxA = -50000.;
        if (mS_theta < NB_S_THETA / 2)
        {
            for (arma::uword i_a = 0; i_a < NB_A; ++i_a)
            {
                const double q_a = mQ(stp, i_a, stpp);
                if (q_a > maxA)
                {
                    maxA = q_a;
                    ap = i_a;
                }
            }
        }
        else
        {
            for (arma::uword i_a = 0; i_a < NB_A; ++i_a)
            {
                arma::uword ri_a(NB_A - i_a - 1);
                const double q_a = mQ(stp, ri_a, stpp);
                if (q_a > maxA)
                {
                    maxA = q_a;
                    ap = ri_a;
                }
            }
        }
    }
//    std::cout << "ap=" << std::setw(4) << ap << ", ";

    // Q(s,a) <- Q(s,a) + alpha * (R(s') + gamma*Q(s',a') - Q(s,a))
//    float rS = r(theta);//r(sp);
//    std::cout << "R=" << std::setw(9) << rS << ", ";
//    float DQ = alpha * (rS + gamma * mQ(sp, ap) - mQ(mS, mA));
//    std::cout << "Q=" << std::setw(9) << mQ(sp, ap) << ", ";
//    std::cout << "DQ=" << std::setw(9) << DQ << ", ";
//    mQ(mS, mA) += DQ;
//    mQ(mS, mA) = min(max(mQ(mS, mA), -300.), 300.);

    double coeff = 0.1f / 60.f;
    for (auto it = mLearningQueue.crbegin(); it != mLearningQueue.crend(); ++it)
    {
        auto &state(*it);
        double &qstate(mQ(state.s_theta, state.a, state.s_thetaP));
        double rS = r(theta, thetaP, x, xP);
//            std::cout << "R=" << std::setw(9) << rS << ", ";
        double DQ = coeff * (rS + gamma * mQ(mS_theta, mA, mS_thetaP) - qstate);
//            std::cout << "Q=" << std::setw(9) << qstate << ", ";
//            std::cout << "DQ=" << std::setw(9) << DQ << ", ";
//        std::cout << DQ << "\n";
        qstate += DQ;
        qstate = enbounds(qstate, -1000., 1000.);
        coeff /= 1.05f;
    }
//    std::cout << std::flush;

    mLearningQueue.push_back({mS_theta, mS_thetaP, mA});
    if (mLearningQueue.size() > learning_offset)
    {
        mLearningQueue.pop_front();
    }

    mS_theta = stp;
    mS_thetaP = stpp;
    mA = ap;

//    std::cout << '\n';

    if (absorb(theta, x))
    {
        mFailed = true;
    }
    std::cout << std::flush;
}

void Brain_DiscretQLearning::ThinkOld()
{
    float theta = mInfo["theta"]() * (180.f / b2_pi);// + 90.f;
//    float thetaP = mInfo["theta."]();
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

void Brain_DiscretQLearning::onReseted()
{
    mLearningQueue.clear();
}
