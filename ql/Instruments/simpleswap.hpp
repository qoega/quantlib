
/*
 Copyright (C) 2000-2004 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it under the
 terms of the QuantLib license.  You should have received a copy of the
 license along with this program; if not, please email quantlib-dev@lists.sf.net
 The license is also available online at http://quantlib.org/html/license.html

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

/*! \file simpleswap.hpp
    \brief Simple fixed-rate vs Libor swap
*/

#ifndef quantlib_simple_swap_hpp
#define quantlib_simple_swap_hpp

#include <ql/Instruments/swap.hpp>
#include <ql/Indexes/xibor.hpp>
#include <ql/schedule.hpp>

namespace QuantLib {

    //! Simple fixed-rate vs Libor swap
    /*! \ingroup instruments */
    class SimpleSwap : public Swap {
      public:
        class arguments;
        class results;
        SimpleSwap(bool payFixedRate,
                   Real nominal,
                   const Schedule& fixedSchedule,
                   Rate fixedRate,
                   const DayCounter& fixedDayCount,
                   const Schedule& floatSchedule,
                   const boost::shared_ptr<Xibor>& index,
                   Integer indexFixingDays,
                   Spread spread,
                   const Handle<YieldTermStructure>& termStructure);
        // results
        Rate fairRate() const;
        Spread fairSpread() const;
        Real fixedLegBPS() const;
        Real floatingLegBPS() const;
        // inspectors
        Rate fixedRate() const;
        Spread spread() const;
        Real nominal() const;
        bool payFixedRate() const;
        const std::vector<boost::shared_ptr<CashFlow> >& fixedLeg() const;
        const std::vector<boost::shared_ptr<CashFlow> >& floatingLeg() const;
        // other
        void setupArguments(Arguments* args) const;
      private:
        bool payFixedRate_;
        Rate fixedRate_;
        Spread spread_;
        Real nominal_;
    };

    //! %Arguments for simple swap calculation
    class SimpleSwap::arguments : public virtual Arguments {
      public:
        arguments() : payFixed(false),
                      nominal(Null<Real>()),
                      currentFloatingCoupon(Null<Real>()) {}
        bool payFixed;
        Real nominal;
        std::vector<Time> fixedResetTimes;
        std::vector<Time> fixedPayTimes;
        std::vector<Real> fixedCoupons;
        std::vector<Time> floatingAccrualTimes;
        std::vector<Time> floatingResetTimes;
        std::vector<Time> floatingPayTimes;
        std::vector<Spread> floatingSpreads;
        Real currentFloatingCoupon;
        void validate() const;
    };

    //! %Results from simple swap calculation
    class SimpleSwap::results : public Value {};


    // inline definitions

    inline Rate SimpleSwap::fairRate() const {
        return fixedRate_ - NPV()/fixedLegBPS();
    }

    inline Spread SimpleSwap::fairSpread() const {
        return spread_ - NPV()/floatingLegBPS();
    }

    inline Real SimpleSwap::fixedLegBPS() const {
        return (payFixedRate_ ? firstLegBPS() : secondLegBPS());
    }

    inline Real SimpleSwap::floatingLegBPS() const {
        return (payFixedRate_ ? secondLegBPS() : firstLegBPS());
    }

    inline Rate SimpleSwap::fixedRate() const {
        return fixedRate_;
    }

    inline Spread SimpleSwap::spread() const {
        return spread_;
    }

    inline Real SimpleSwap::nominal() const {
        return nominal_;
    }

    inline bool SimpleSwap::payFixedRate() const {
        return payFixedRate_;
    }

    inline const std::vector<boost::shared_ptr<CashFlow> >&
    SimpleSwap::fixedLeg() const {
        return (payFixedRate_ ? firstLeg_ : secondLeg_);
    }

    inline const std::vector<boost::shared_ptr<CashFlow> >&
    SimpleSwap::floatingLeg() const {
        return (payFixedRate_ ? secondLeg_ : firstLeg_);
    }

}


#endif
