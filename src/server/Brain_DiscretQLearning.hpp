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

#include "Brain.hpp"


class Brain_DiscretQLearning : public Brain
{
    static constexpr arma::uword NB_S_THETA{120};
    static constexpr arma::uword NB_S_THETA_P{41};
    static constexpr arma::uword NB_A{101};

    static constexpr size_t learning_offset = 700;

    struct SnapLearn
    {
        arma::uword s_theta;
        arma::uword s_thetaP;
        arma::uword a;
    };

    using learning_queue = std::list<SnapLearn>;

public:
    Brain_DiscretQLearning();

    virtual ~Brain_DiscretQLearning();

    virtual void Save() const override;

    void ThinkOld();

    virtual void Reseted() override;

protected:
    virtual void think() override;

private:
    static inline float stotheta(const arma::uword &s);
    static inline arma::uword thetatos(float theta);
    static inline float stothetaP(const arma::uword &s);
    arma::uword thetaPtos(float thetaP);

    arma::cube::fixed<NB_S_THETA, NB_A, NB_S_THETA_P> mQ;
    arma::vec mTP;
    arma::uword mS_theta;
    arma::uword mS_thetaP;
    arma::uword mA;
    learning_queue mLearningQueue;
};

