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
#include "gnuplot-iostream.h"


class Brain_ContinuousQLearning : public Brain
{
    static constexpr size_t learning_offset = 700;
    using learning_queue = std::list<arma::uword>;

    static const arma::uword DIM_S{5};
    static const arma::uword NB_A{80};

public:
    Brain_ContinuousQLearning();

    virtual ~Brain_ContinuousQLearning();

    virtual void Save() const override;

    virtual void Reseted() override;

protected:
    virtual void think() override;

private:
    Gnuplot mGP;
    arma::mat mTheta;
    arma::mat mS;
    arma::uword mA;

    learning_queue mLearningQueue;
};

