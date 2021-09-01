//
// Created by oleg0 on 14.07.2021.
//

#ifndef UNTITLED7_GENETICALGORITHM_H
#define UNTITLED7_GENETICALGORITHM_H

#include <algorithm>
#include <iostream>
#include "Player.h"
#include "Market.h"
#include <vector>

class GeneticAlgorithm {
private:
    std::vector<int64_t> perm;
public:
    GeneticAlgorithm() {
        perm.resize(const_cntGoodPlayer);
        Game();
    }
    void Game() {
        std::vector<Player> v(const_cntGoodPlayer);
        for (int64_t i = 0; i < const_cntGoodPlayer; i++) {
            v[i] = Player(i);
            perm[i] = i;
        }
        int64_t epoch = 0;
        auto traders = v;
        for (; epoch <= const_rounds; epoch++) {
            traders = Round(traders, epoch);
            //std::cout << "epoch: " << epoch << '\n';
        }
    }
    std::vector<Player> Round(std::vector<Player> &players, int64_t &epoch) {
        std::cout << "epoch: " << epoch << '\n';
        Market market(const_start_price, players);
        std::cout << "epoch: " << epoch << '\n';
        for (int64_t i = 0; i < const_length_of_round; i++) {
            market.Loop();
           // std::cout << i << '\n';
        }
        std::cout << "epoch: " << epoch << '\n';
        //market.setOnFile(epoch);
        if (epoch == const_rounds) {
            std::vector<std::pair<int64_t, Player>> res = market.Score();
            std::sort(res.rbegin(), res.rend());
            std::cout << "int pos score panika risk money shares\n";
            int cnt = 0;
            for (auto  [p1,p] : res) {
                std::cout << ++cnt << ' '    << p.id << ' ' << p1 << ' ' << p.wish_sell << ' ' << p.wish_buy << ' ' << p.money << ' ' << p.cnt_shares << '\n';
            }
            std::cout << market.getRate() << ' ' <<  market.cntStocks << '\n';
            std::cout << "\nParams:\n";
            market.PrintParams();
            auto v1 = market.getMaxRate();
            auto v2 = market.getMinRate();
            auto v3 = market.getEndRate();
            std::cout << "\ncharacters:\n";
            for (auto [score, player] : res) {
                std::cout << score << ' ' << player.money << ' ' << player.cnt_shares << ' ' << player.id
                    << ' ' << player.wish_buy << ' ' << player.wish_sell;
                for (auto param : player.model.getParams()) {
                    std::cout << ' ' << param;
                }
                std::cout << '\n';
            }
            std::cout << "\nactions:\n";
            for (size_t i = 0; i < v1.size(); i++) {
                std::cout << v1[i] << ' ' << v2[i] << ' ' << v3[i] << '\n';
            }
        }
        std::vector<std::pair<int64_t, Player>> res = market.Score();
        std::sort(res.rbegin(), res.rend());
        std::vector<Player> traders = CrossingOver(res);
        Mutation(traders);
        AddRandoms(traders);
        return traders;
    }
    std::vector<Player> CrossingOver(std::vector<std::pair<int64_t, Player>> &v) {
        std::vector<Player> traders(const_cntGoodPlayer);
        for (int64_t i = 0; i < const_cntGoodPlayer; i++) {
            traders[i] = Player(i);
        }
        topAdd(traders, v, 0);
        topAdd(traders, v, 1);
        topAdd(traders, v, 2);
        v.clear();
        return traders;
    }
    void topAdd(std::vector<Player>& traders, std::vector<std::pair<int64_t, Player>> &v, int16_t mode) {
        std::random_shuffle(perm.begin(), perm.end());
        int64_t p = 0;
        size_t cnt = 0;
        size_t position = 0;
        size_t all = const_cntGoodPlayer;
        while (all > 0 && position < v.size()) {
            size_t cnt = size_t(std::floor(double(all) / 100 * 6));
            size_t one = 1;
            cnt = std::max(cnt, one);
            for (int64_t i = 0; i < cnt; i++) {
                if (mode == 0) {
                    traders[perm[p]].wish_buy = v[position].second.wish_buy;
                } else if (mode == 1) {
                    traders[perm[p]].wish_sell = v[position].second.wish_sell;
                } else if (mode == 2) {
                    traders[perm[p]].model = v[position].second.model;
                }
                p++;
            }
            position++;
            all -= cnt;
        }
    }


    void Mutation(std::vector<Player> &traders) {
        srand(std::time(0));
        for (int64_t i = 0; i < const_cnt_mutation; i++) {
            traders[rand() % const_cntGoodPlayer].wish_buy = uniform_dist(e) / 1000;
            traders[rand() % const_cntGoodPlayer].wish_sell = uniform_dist(e) / 1000;
            traders[rand() % const_cntGoodPlayer].updateModel();
        }
    }
    void AddRandoms(std::vector<Player> &v) {
        for (int i = 0; i < const_cntPlayers - const_cntGoodPlayer; i++) {
            v.emplace_back(Player(v.size() + i));
        }
    }
};

#endif //UNTITLED7_GENETICALGORITHM_H
