//
// Created by oleg0 on 14.07.2021.
//

#ifndef UNTITLED7_MARKET_H
#define UNTITLED7_MARKET_H

#include "Player.h"
#include "Model.h"
#include <map>
#include <random>
#include <vector>
#include <iostream>

class Market {
private:
    Model longTrend, shortTrend;
    int64_t cntSellsOffers, cntBuysOffers;
    std::map<int64_t, std::pair<int64_t, int64_t>> sells_deals; //who wants sell
    std::map<int64_t, std::pair<int64_t, int64_t>> buys_deals; //who wants buy
    double rate, mxRt, mnRt, meanRt;
    int64_t day;

    std::vector<double> maxRate;
    std::vector<double> minRate;
    std::vector<double> endRate;
public:
    int64_t cntStocks;
    std::map<int64_t, Player> traders; //Это делается в игре, а не на рынке
    Market(int64_t start_rate, std::vector<Player> &v) {
        longTrend.installSize(3);
        shortTrend.installSize(1);
        rate = start_rate;
        cntSellsOffers = 0;
        cntBuysOffers = 0;
        cntStocks = 10;
        day = 0;
        for (int64_t i = 0; i < v.size(); i++) {
            traders[i] = v[i];
        }
    }

    void Loop() {
        newDay();
        shortTrend.Clear();
        PayDividents();
        CreateNewPlayer();
        ReleaseNewStocks();
        mxRt = rate;
        mnRt = rate;
        meanRt = 0;
        for (auto &[id, player] : traders) {
            double solution = player.simpleStrategy(rate);
            if (solution > 0) {
                WantBuySimple(id, solution); //Хочет купить
            } else if (solution < 0) {
                WantSellSimple(id, -1 * solution);
            }
            if (rate > mxRt) {
                mxRt = rate;
            } else if (rate < mnRt) {
                mnRt = rate;
            }
            meanRt += rate;
            if (shortTrend.isMemoryFull()) {
           //     shortTrend.constructBag(rate);
            }
            std::vector<double> tmp = {rate};
           // shortTrend.setMemory(tmp);
        }
        maxRate.push_back(mxRt);
        minRate.push_back(mnRt);
        endRate.push_back(meanRt / traders.size());
        std::vector<double> zanos = {mxRt, mnRt, meanRt / traders.size()};
        if (day > 0) {
            longTrend.constructBag(rate);
        }
        longTrend.setMemory(zanos);
        day++;
    }

    void ReleaseNewStocks() {
        if (day % 150 == 0) {
            cntStocks += 10 + (std::rand() % 50);
        }
    }
    void CreateNewPlayer() {
        if (rand() % 15 == 13) {
            int64_t position = traders.size();//
            traders.insert({position, Player(position)});
        }
    }
    void PayDividents() {
        if (day % 50 == 0) {
            for (auto &[id, player] : traders) {
                player.money += (rate / 20) * player.cnt_shares;
            }
        }
    }

    void updateRate(double delt) {
        rate += delt;
    }
    void newDay() {
        rate += uniform_dist(e) / 10 * 2 - 120;
        if (rate < 100) {
            rate = 100;
        }
    }
    void WantBuySimple(int64_t id, int64_t shares) { //Хочет купить акции
        double mn = std::min(shares, std::min(cntStocks, int64_t(std::floor(traders[id].money / rate))));
        traders[id].money -= shares * rate;
        traders[id].cnt_shares += mn ;
        updateRate(double(mn) / 30);
        cntStocks -= mn;
    }
    void WantSellSimple(int64_t id, double shares) { // Хочет продать акции
        traders[id].money += shares * rate;
        traders[id].cnt_shares -= shares;
        updateRate(-double(shares) / 30);
        cntStocks += shares;
    }

    std::vector<std::pair<int64_t,Player>> Score() {
        std::vector<std::pair<int64_t, Player>> ans;
        for (auto [id, player] : traders) {
            ans.push_back({player.money + player.cnt_shares * rate, player});
        }
        return ans;
    }

    double getRate() {
        return rate;
    }

    std::vector<double> getMaxRate() {
        return maxRate;
    }
    std::vector<double> getMinRate() {
        return minRate;
    }
    std::vector<double> getEndRate() {
        return endRate;
    }
    void PrintParams() {
        longTrend.printParams();
    }

    //Более продвинутые функции рынка будут реализиваны позже
    void WantBuy(Player &trader, int64_t shares) {
        //shares уже приходят корректные, игрок может их всех купить
        updateRate(shares / 30);
        for (auto &[id, deal] : sells_deals) {
            if (trader.money < rate || shares == 0) {
                return;
            }
            if (deal.first == trader.id) {
                continue;
            }
            if (deal.second > shares) {
                deal.second -= shares;
                traders[deal.first].want_sell -= shares;
                traders[deal.first].cnt_shares -= shares;
                traders[deal.first].money += shares * rate;
                trader.cnt_shares += shares;
                trader.money -= shares * rate;
                return;
            } else {
                traders[deal.first].want_sell -= deal.second;
                traders[deal.first].money += deal.second * rate;
                traders[deal.first].cnt_shares -= deal.second;
                trader.cnt_shares += deal.second;
                trader.money -= deal.second * rate;
                shares -= deal.second;
                deal.second = 0;
                sells_deals.erase(sells_deals.find(id));
            }
        }
        //Если после всех манипуляций у человека осталось ещё количество акций, которые он хотел бы купить
        if (shares > 0) {
            buys_deals[++cntBuysOffers] = {trader.id, shares};
            trader.want_buy += shares;
        }
    }
    void WantSell(Player &trader, int64_t shares) { //Хочет продать акции
        updateRate(-shares / 30);
        for (auto &[id, deal] : buys_deals) {
            if (shares == 0) {
                return;
            }
            if (deal.first == trader.id) {
                continue;
            }
            int64_t control_deal = std::min(deal.second, int64_t(std::floor(trader.money / rate)));
            if (control_deal > shares) {
                deal.second -= shares;

                trader.money += double(shares) * rate;
                traders[deal.first].money -= double(shares) * rate;
                trader.cnt_shares -= shares;
                traders[deal.first].cnt_shares += shares;
                traders[deal.first].want_buy -= shares;
                return;
            } else {
                trader.money += double(control_deal) * rate;
                traders[deal.first].money -= double(control_deal) * rate;
                trader.cnt_shares -= control_deal;
                traders[deal.first].cnt_shares += control_deal;
                traders[deal.first].want_buy -= deal.second;
                shares -= control_deal;
                buys_deals.erase(buys_deals.find(id));
            }
        }
        if (shares > 0) {
            sells_deals[++cntSellsOffers] = {trader.id, shares};
            trader.want_sell += shares;
        }
    }
};

#endif //UNTITLED7_MARKET_H
