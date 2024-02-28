#ifndef STIMUS_H
#define STIMUS_H

#include <functional>
#include <unordered_map>
#include <iostream>
#include <random>
#include <limits>
#include <cstdint>
#include "../model/struct.h"

template<typename T>
class Stimus {
public:
    uint64_t get_mant_mask(const int&mant_w){
        uint64_t res=0;
        for(int i=0;i<mant_w;i++){
            res = 1+(res<<1);
        }   
        return res;
    }
    uint32_t set_expo_max(int expo_w){
        uint32_t result=0;
        for(int i=0;i<expo_w;i++){
            result = 1+(result<<1);
        }
        return result;
    }
    using FunctionPointer = std::function<T()>;
    T get_rand_reg() {
        T res;
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<int> bool_dist(0, 1);
        bool random_bool = bool_dist(gen) != 0;

        std::uniform_int_distribution<uint32_t> uint32_dist;
        uint32_t random_uint32 = uint32_dist(gen);

        std::uniform_int_distribution<uint64_t> uint64_dist;
        uint64_t random_uint64 = uint64_dist(gen);

        uint64_t mant_mask = get_mant_mask(res.mant_w);
        uint32_t expo_mask = set_expo_max(res.expo_w);
        uint64_t res_mant  = random_uint64 & mant_mask;

        res.sign      = random_bool;
        res.expo      = expo_mask&random_uint32;
        res.mant      = res_mant;
        return res; 
    }

    T get_rand_nan() {
        T res;
        std::cout<<"into rand_nan"<<std::endl;
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<int> bool_dist(0, 1);
        bool random_bool = bool_dist(gen) != 0;

        std::uniform_int_distribution<uint64_t> uint64_dist;
        uint64_t random_uint64 = uint64_dist(gen);

        // std::cout << "Random bool: " << random_bool << std::endl;
        // std::cout << "Random uint64_t: " << random_uint64 << std::endl;
        
        uint64_t mant_mask = get_mant_mask(res.mant_w);
        uint64_t res_mant  = random_uint64 & mant_mask;

        uint32_t expo_mask = set_expo_max(res.expo_w);
        res.sign      = random_bool;
        res.expo      = expo_mask&0xffffffff;
        res.mant      = res_mant;
        return res;
    }

    T get_rand_zero() {
        // 实现获取随机数的逻辑
        T res;
        std::random_device rd;
        std::mt19937_64 gen(rd());

        std::uniform_int_distribution<int> bool_dist(0, 1);
        bool random_bool = bool_dist(gen) != 0;
        res.sign      = random_bool;
        res.expo      = 0;
        res.mant      = 0;
        return res;
    }

    T get_rand_inf() {
        // 实现获取随机数的逻辑
        T res;
        std::random_device rd;
        std::mt19937_64 gen(rd());
        std::uniform_int_distribution<int> bool_dist(0, 1);
        bool random_bool = bool_dist(gen) != 0;
        uint32_t expo_mask = set_expo_max(res.expo_w);

        res.sign      = random_bool;
        res.expo      = expo_mask&0xffffffff;
        res.mant      = 0;
        return res;
    }

    std::unordered_map<std::string, FunctionPointer> stimus_table = {
        {"reg",  std::bind(&Stimus::get_rand_reg, this)},
        {"nan",  std::bind(&Stimus::get_rand_nan, this)},
        {"zero", std::bind(&Stimus::get_rand_zero, this)},
        {"inf",  std::bind(&Stimus::get_rand_inf, this)}
    };
};


#endif