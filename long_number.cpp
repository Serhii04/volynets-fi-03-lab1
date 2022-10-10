#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stdio.h>


typedef uint32_t base_t;
const int num_size = 32;
const int first_num_size = 4294967295;
const base_t MAX_NUM = -1;


class LongNumber{
public:
    LongNumber(unsigned int number, unsigned int size);
    void print() const;

    std::vector<base_t> *number = nullptr;
    int size;
};

unsigned long int get_mask_1(int n){  // fill first n with 1
    unsigned long int mask = 1;
    for(int i = 0; i < n; ++i){
        mask = mask << 1;
        mask = mask | 1; 
    }
    return mask;
}

LongNumber::LongNumber(unsigned int number, unsigned int size):size(size){
    this->number = new std::vector<base_t>;
    this->number->resize(size);
    
    for(int i = 0; i < this->size; ++i){
        this->number->at(i) = (base_t)0;
    }

    // dont work with nuge  numbers
    for(int i = 0; i < this->size; ++i){
        this->number->at(i) = (base_t)(number & get_mask_1(num_size));
        for(int i = 0; i < num_size; ++i){  // because >> 64 don't work with int
            number = number >> 1;
        }
    }
}

void LongNumber::print() const {
    for(int i = 0; i < this->number->size(); ++i){
        std::cout << (int)this->number->at(i) << ", ";  // for better times (std::bitset<8>)
    }
    std::cout << std::endl;
}


LongNumber LongAdd(LongNumber A, LongNumber B){
    int max_size = std::max(A.size, B.size);
    int min_size = std::min(A.size, B.size);

    LongNumber C = LongNumber(0, max_size);

    base_t carry = 0;
    int64_t temp = 0;
    for(int i = 0; i < min_size; ++i){
        temp = A.number->at(i) + B.number->at(i) + carry;
        C.number->at(i) = temp & MAX_NUM;
        carry = temp >> num_size;
    }
    return C;
}
