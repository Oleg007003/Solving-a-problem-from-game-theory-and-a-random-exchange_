//
// Created by oleg0 on 19.07.2021.
//

#ifndef UNTITLED7_MODEL_H
#define UNTITLED7_MODEL_H

#include <deque>
#include <iostream>

// Можно доделать позже
struct Rn {
    std::vector<double> x;
    Rn(const size_t &s, bool iszero = 1) {
        if (iszero) {
            x.resize(s + 1, 0);
        } else {
            x.resize(s + 1);
            for (size_t i = 0; i < x.size(); i++) {
                x[i] = uniform_dist(e);
            }
        }
    }
    Rn(std::vector<double> &v) {
        x = v;
    }
    Rn(Rn &v) {
        x = v.x;
    }
    Rn() {

    }
    friend Rn operator + (Rn &first, Rn &second) {
        Rn ans(first.getSize());
        for (size_t i = 0; i < first.getSize(); i++) {
            ans.x[i] = first.x[i] + second.x[i];
        }
        return ans;
    }
/*    friend Rn operator * (Rn &first, double &second) {
        Rn ans(first);
        for (auto &i : ans.x) {
            i *= second;
        }
    }*/

    const size_t getSize() {
        return x.size();
    }
};

class Model {
private:
    std::vector<double> v;
    std::vector<double> cache;
    std::vector<double> params;
    std::deque<std::pair<std::vector<double>, double>> bag;
    std::vector<double> memory;
public:
    Model(size_t sz) {
        v.resize(sz + 1, 0);
        cache.resize(sz + 1, 0);
        params.resize(sz + 1);
        for (size_t i = 0; i < sz + 1; i++) {
            params[i] = uniform_dist(e) / 100;
        }
    }
    Model() {
    }
    void installSize(size_t sz) {
        v.resize(sz + 1, 0);
        cache.resize(sz + 1, 0);
        params.resize(sz + 1);
        for (size_t i = 0; i < sz + 1; i++) {
            params[i] = uniform_dist(e) / 100;
        }
    }
    std::vector<double> grad(std::vector<std::vector<double>> x, const std::vector<double> &real_value) {
        for (size_t i = 0; i < x.size(); i++) {
            x[i].push_back(1);
        }
        std::vector<double> ans(params.size(), 0);
        for (size_t j = 0; j < x.size(); j++) {
            double pred = 0;
            for (size_t i = 0; i < x[j].size(); i++) {
                pred += params[i] * x[j][i];
            }
            for (size_t i = 0; i < x[j].size(); i++) {
                ans[i] += (pred - real_value[j]) * x[j][i];
                if (i < x[j].size() - 1) {
                    ans[i] += 0.2 * params[i];
                }
            }
        }
        for (size_t j = 0; j < params.size(); j++) {
            ans[j] /= x.size();
        }
        return ans;
    }
    void fit(std::vector<double> &x, const double &real_value) {
        //todo
        if (bag.size() > 5) {
            if (bag.size() > 100) {
                bag.pop_back();
            }
            std::vector<std::vector<double>> v(7);
            std::vector<double> y(7);
            v[0] = x; y[0] = real_value;
            v[1] = bag[0].first; y[1] = bag[0].second;
            v[2] = bag[1].first; y[2] = bag[1].second;
            v[3] = bag[2].first; y[3] = bag[2].second;
            for (size_t i = 4; i < 7; i++) {
                size_t rd = size_t(std::floor(uniform_dist(e) / 1000) * (bag.size() - 1));
                v[i] = bag[rd].first;
                y[i] = bag[rd].second;
            }
            for (size_t i = 0; i < 50; i++) {
                gradient_step(v, y);
            }
        }
        bag.push_front({x, real_value});
    }
    void gradient_step(std::vector<std::vector<double>> &x, const std::vector<double> &real_value) {
        std::vector<double> f = grad(x, real_value);
        for (size_t i = 0; i < f.size(); i++) {
            v[i] = const_ypsilon * v[i] + (1 - const_ypsilon) * f[i];
            cache[i] = const_beta * cache[i] + (1 - const_beta) * (f[i] * f[i]);
            params[i] -= const_alpha * v[i] / std::sqrt(cache[i] + const_epsilon);
        }
    }
    void setMemory(std::vector<double> &v) {
        memory = v;
    }
    void constructBag(double ans) {
        fit(memory, ans);
    }
    double predict(std::vector<double> x) {
        x.push_back(1);
        double ans = 0.0;
        for (size_t i = 0; i < x.size(); i++) {
            ans += params[i] * x[i];
        }
        return ans;
    }
    void Clear() {
        bag.clear();
        memory.clear();
    }
    size_t Size() {
        return bag.size();
    }
    bool isMemoryFull() {
        return !memory.empty();
    }
    void printParams() {
        for (auto i : params) {
            std::cout << i << '\n';
        }
    }
};
#endif //UNTITLED7_MODEL_H
