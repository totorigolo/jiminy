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

#pragma once

#include <functional>
#include <string>
#include <list>
#include <map>

#include <mlpack/core.hpp>
#include <armadillo>


constexpr arma::uword NB_S_THETA{120};
constexpr arma::uword NB_S_THETA_P{41};
constexpr arma::uword NB_A{101};

constexpr size_t learning_offset = 60;

struct SnapLearn
{
    arma::uword s_theta;
    arma::uword s_thetaP;
    arma::uword a;
};

using learning_queue = std::list<SnapLearn>;

class Brain
{
public:
    Brain();

    virtual ~Brain();

    void Save() const;

    void Think();

    void ThinkOld();

    bool Failed() const;

    void Reseted();

    // TODO: Make mActions private
    std::map<std::string, std::function<float(void)>> mInfo;
    std::map<std::string, std::function<void(float)>> mActions;
private:

    arma::uword thetaPtos(float thetaP);

    unsigned long long int mIteration;
    bool mSimulated;
    arma::cube::fixed<NB_S_THETA, NB_A, NB_S_THETA_P> mQ;
    arma::vec mTP;
    arma::uword mS_theta;
    arma::uword mS_thetaP;
    arma::uword mA;
    learning_queue mLearningQueue;
    bool mFailed;
};

