//
// Created by oleg0 on 14.07.2021.
//

#ifndef UNTITLED7_PLAYER_H
#define UNTITLED7_PLAYER_H

#include "Constants.h"

#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <list>

class Player {
public:
    int64_t cnt_shares;
    double wish_buy, wish_sell, money;
    int64_t want_buy, want_sell;
    //std::list<int64_t> buysroster;
    //std::list<int64_t> selsroster;
    int64_t id;
    Player(int64_t number) : id(number), want_sell(0), want_buy(0), money(const_start_money),
        cnt_shares(const_start_shares), wish_sell(uniform_dist(e)), wish_buy(uniform_dist(e)) {
    }
    Player() {
    }
    int64_t Strategy(int64_t cost, double trend) {
        //todo StrategyFunction with mode
        return 0;
    }
    double simpleStrategy(double &rate) {
        int64_t result = rand() % int64_t(std::floor(wish_sell + wish_buy));
        double ans;
        if (result >= wish_sell) {
            ans =  std::max(0.0, round(double((money - want_buy * rate) * (double(result) - wish_sell)) / (wish_buy + 1.1)));
             return std::min(int64_t(std::floor(result - wish_sell)), int64_t(std::floor(money / rate))); // - old_version
        } else {
            ans = - round(double((cnt_shares - want_sell) * result) / (wish_sell + 1.1));
            return std::max(-result, -cnt_shares); // - old_version
        }
        return ans; // - new_version
    }

    friend bool operator < (const Player& p1, const Player& p2) {
        return p1.money > p2.money;
    }
};

#endif //UNTITLED7_PLAYER_H


