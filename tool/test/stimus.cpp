#include "stimus.h"
void Stimus::get_rand_reg(FpBase* res){
    std::random_device rd;
    std::mt19937_64 gen(rd());

    std::uniform_int_distribution<int> bool_dist(0, 1);
    bool random_bool = bool_dist(gen) != 0;

    std::uniform_int_distribution<uint32_t> uint32_dist;
    uint32_t random_uint32 = uint32_dist(gen);

    std::uniform_int_distribution<uint64_t> uint64_dist;
    uint64_t random_uint64 = uint64_dist(gen);

    uint64_t mant_mask = get_mask((*res).mant_w);
    uint32_t expo_mask = set_expo_max((*res).expo_w);
    uint64_t res_mant  = random_uint64 & mant_mask;

    (*res).sign      = random_bool;
    (*res).expo      = expo_mask&random_uint32;
    (*res).mant      = res_mant;
}

void Stimus::get_rand_nan(FpBase* res){
    std::random_device rd;
    std::mt19937_64 gen(rd());

    std::uniform_int_distribution<int> bool_dist(0, 1);
    bool random_bool = bool_dist(gen) != 0;

    std::uniform_int_distribution<uint64_t> uint64_dist;
    uint64_t random_uint64 = uint64_dist(gen);
    uint64_t mant_mask = get_mask((*res).mant_w);
    uint64_t res_mant  = random_uint64 & mant_mask;

    uint32_t expo_mask = set_expo_max((*res).expo_w);
    (*res).sign      = random_bool;
    (*res).expo      = expo_mask&0xffffffff;
    (*res).mant      = res_mant;
}

void Stimus::get_rand_zero(FpBase* res){
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> bool_dist(0, 1);
    bool random_bool = bool_dist(gen) != 0;
    (*res).sign      = random_bool;
    (*res).expo      = 0;
    (*res).mant      = 0;
}

void Stimus::get_rand_inf(FpBase* res){
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> bool_dist(0, 1);
    bool random_bool = bool_dist(gen) != 0;
    uint32_t expo_mask = set_expo_max((*res).expo_w);
    (*res).sign      = random_bool;
    (*res).expo      = expo_mask&0xffffffff;
    (*res).mant      = 0;
}