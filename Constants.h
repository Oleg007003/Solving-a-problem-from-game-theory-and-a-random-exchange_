    //
    // Created by oleg0 on 14.07.2021.
    //

    #ifndef UNTITLED7_CONSTANTS_H
    #define UNTITLED7_CONSTANTS_H


    #include <cstdint>
    #include <random>
    #include <ctime>

    const int64_t const_cntPlayers = 240;
    const int64_t const_cntGoodPlayer = 210;
    const int64_t const_rounds = 20;
    const double const_start_price = 300.0;
    const int64_t const_start_shares = 5;
    const size_t const_length_of_round = 3000;
    const double const_start_money = 500.0;
    const size_t const_cnt_mutation = 20;
    std::uniform_real_distribution<double> uniform_dist(0.0, 1000.0);
    std::default_random_engine e(std::time(nullptr));

    //Model_params
    const double const_ypsilon = 0.7;
    const double const_beta = 0.7;
    const double const_alpha = 0.025;
    const double const_epsilon = 0.0001;
#endif //UNTITLED7_CONSTANTS_H
