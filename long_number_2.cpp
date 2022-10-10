#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <stdio.h>

typedef uint32_t base_t;
const int num_size = 32;
const std::string HEX[16] = {
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "a",
    "b",
    "c",
    "d",
    "e",
    "f",
};


class LongNumber{
public:
    LongNumber(int size);
    LongNumber(std::vector<uint64_t> number);
    LongNumber(std::string number);
    void print() const;
    std::string str() const;
    std::string to_string() const;
    void set(unsigned long long number);
    void set(std::vector<uint64_t> number);
    LongNumber set_by_hex(std::string number);
    int size() const;
    void resize(int size);
    std::string to_hex_string() const;

    std::vector<base_t> *number = nullptr;
};

std::string LongNumber::to_string() const{
    return to_hex_string();
}

void LongNumber::resize(int size){
    std::vector<base_t> *numb = new std::vector<base_t>;
    numb->resize(size);

    for(int i = 0; i < numb->size(); ++i){
        numb->at(i) = (base_t)0;
    }

    int min_size = std::min(size, this->size());
    for(int i = 0; i < min_size; ++i){
        numb->at(i) = number->at(i);
    }

    number = numb;
}

int LongNumber::size() const{
    return number->size();
}

unsigned long int get_mask_1(int n){  // fill first n with 1
    unsigned long int mask = 1;
    if(n < 1){
        mask = 0;
        return mask;
    }

    if(n > 64){
        mask = -1;
        return mask;
    }
    n -=1;

    for(int i = 0; i < n; ++i){
        mask = mask << 1;
        mask = mask | 1; 
    }
    return mask;
}

LongNumber::LongNumber(int size = 1){
    this->number = new std::vector<base_t>;
    this->number->resize(size);
    
    for(int i = 0; i < this->size(); ++i){
        this->number->at(i) = (base_t)0;
    }
}

void LongNumber::print() const {
    std::cout << str() << std::endl;
}

std::string LongNumber::str() const {
    std::string out = "";
    unsigned long long temp = 0;
    for(int i = 0; i < this->number->size(); ++i){
        out = out + std::to_string(this->number->at(i)) + ", ";
    }
    return out;
}

void LongNumber::set(unsigned long long number){
    unsigned long long mask = get_mask_1(num_size);
    for(int i = 0; i < size(); ++i){
        this->number->at(i) = number & mask;
        number = number >> num_size;
    }

}

LongNumber LongAdd(LongNumber A, LongNumber B){
    int max_size = std::max(A.size(), B.size());
    unsigned long long mask = get_mask_1(num_size);
    A.resize(max_size);
    B.resize(max_size);


    LongNumber C = LongNumber(max_size);

    base_t carry = 0;
    uint64_t temp = 0;
    for(int i = 0; i < max_size; ++i){
        temp = uint64_t(A.number->at(i)) + uint64_t(B.number->at(i)) + uint64_t(carry);
        C.number->at(i) = temp & mask;
        carry = temp >> num_size;
    }
    return C;
}

LongNumber::LongNumber(std::vector<uint64_t> number){
    set(number);
}

LongNumber::LongNumber(std::string number){
    set_by_hex(number);
}

void LongNumber::set(std::vector<uint64_t> number){
    delete this->number;
    this->number = new std::vector<base_t>;
    this->number->resize(number.size());
    
    for(int i = 0; i < this->size(); ++i){
        this->number->at(i) = (base_t)number.at(i);
    }
}

int LongCmp(LongNumber A, LongNumber B){
    int max_size = std::max(A.size(), B.size());
    A.resize(max_size);
    B.resize(max_size);

    int i = A.size() - 1;
    while(i > -1 && A.number->at(i) == B.number->at(i)){
        i = i-1;
    }

    if (i == -1){
        return 0;
    }else{
        if(A.number->at(i) > B.number->at(i)){
            return 1;
        }else{
            return -1;
        }
    }
}

LongNumber LongSub(LongNumber A, LongNumber B){
    if(LongCmp(A, B) == -1){
        std::cout << "A leq B where A: " << A.to_hex_string() << " and B is: " << B.to_hex_string() << std::endl;
    }
    int max_size = std::max(A.size(), B.size());
    A.resize(max_size);
    B.resize(max_size);
    LongNumber C = LongNumber(max_size);
    
    int64_t borrow = 0;
    int64_t temp = 0;
    for(int i = 0; i < max_size; ++i){
        temp = int64_t(A.number->at(i)) - int64_t(B.number->at(i)) - int64_t(borrow);
        
        if(temp >= 0){
            C.number->at(i) = base_t(temp);
            borrow = 0;
        }else{
            C.number->at(i) = base_t((int64_t(1) << (num_size)) + temp);
            borrow = 1;
        }
    }

    return C;
}

std::string LongNumber::to_hex_string() const{
    std::string rez = "";
    unsigned long mask = get_mask_1(4);

    uint16_t temp = 0;
    for(int i = size() - 1; i > -1; --i){
        for(int j = num_size - 4; j > -1; j -= 4){
            temp = (number->at(i) >> j) & mask;
            rez += HEX[temp];
        }
    };

    rez.erase(0, rez.find_first_not_of('0'));
    if(rez == ""){
        rez = "0";
    }

    return rez;
}

uint64_t hex_to_uint64_t(std::string s){
    if(isdigit(s[0])){
        return uint64_t(stoi(s));
    }else if(s == "a"){
        return uint64_t(10);
    }else if(s == "b"){
        return uint64_t(11);
    }else if(s == "c"){
        return uint64_t(12);
    }else if(s == "d"){
        return uint64_t(13);
    }else if(s == "e"){
        return uint64_t(14);
    }else if(s == "f"){
        return uint64_t(15);
    }

    if(s == ""){
        std::cerr << "error: empty string" << std::endl;
    }

    std::cerr << "error: unnown symbol: " << s << ";" << std::endl;

    throw -23;
}

int hex_size_to_64(std::string s){
    if(s == ""){
        return 0;
    }

    if(s.size() % 8 == 0){
        return s.size()/8;
    }
    
    return s.size()/8 + 1;
}

std::string to_8_div_lenght(std::string s){
    int l = s.size() % 8;
    if(l == 0){
        return s;
    }

    l = 8 - l;
    for(int i = 0; i < l; ++i){
        s = "0" + s;
    }

    return s;
}

uint64_t get_number_at_8_hex(std::string hex){
    uint64_t number = 0;
    uint64_t temp = 0;
    for(int i = 0; i < 8; ++i){
        temp = hex_to_uint64_t(hex.substr(i, 1));
        number |= (temp << (7-i)*4);
    }

    return number;
}

LongNumber LongNumber::set_by_hex(std::string number){
    std::vector<uint64_t> *v = new std::vector<uint64_t>;
    v->resize(hex_size_to_64(number));
    number = to_8_div_lenght(number);

    std::string temp_str = "";
    for(int i = 0; i < v->size(); ++i){
        for(int j = 0; j < 8; ++j){
            temp_str = number.substr(number.size() - 8 - i*8, 8);
        }
        int numb = get_number_at_8_hex(temp_str);
        v->at(i) = numb;
    }

    set(*v);

    return *this;
}

LongNumber LongMulOneDigit(LongNumber A, unsigned long long b){
    uint64_t mask = get_mask_1(num_size);
    uint64_t carry = 0;
    uint64_t temp = 0;
    LongNumber C = LongNumber(1);
    C.resize(A.size() + 1);
    for(int i = 0; i < A.size(); ++i){
        temp = A.number->at(i) * b + carry;
        C.number->at(i) = temp & mask;
        carry = temp >> num_size;
    }
    C.number->at(A.size()) = carry;

    return C;
}

LongNumber LongLeftShift(LongNumber A){
    LongNumber rez = 0;
    rez.resize(A.size() + 1);
    base_t cary = 0;
    for(int j = 0; j < A.size(); ++j){
        rez.number->at(j) = (A.number->at(j) << 1);
        rez.number->at(j) = rez.number->at(j) | cary;
        cary = (A.number->at(j) >> num_size-1) & 1;
    }

    return rez;
}

LongNumber LongShiftDigitsToHigh(LongNumber A, int l){  // left shift
    if(l > num_size * A.size()){
        std::cerr << "error: shift number is grather than size of number (shift number is: " << l << ")" << std::endl;
        return LongNumber(0);
    }

    for(int i = 0; i < l; ++i){
        A = LongLeftShift(A);
    }

    return A;
}

LongNumber LongLeftShift_2(LongNumber A){
    LongNumber rez = 0;
    rez.resize(A.size() + 1);
    base_t cary = 0;
    for(int j = 0; j < A.size(); ++j){
        rez.number->at(j) = (A.number->at(j) << 1);
        rez.number->at(j) = rez.number->at(j) | cary;
        cary = (A.number->at(j) >> num_size-1) & 1;
    }

    rez.number->at(A.size()) |= cary;
    
    return rez;
}

LongNumber LongShiftDigitsToHigh_2(LongNumber A, int l){  // left shift with different size changing
    if(l > num_size * A.size()){
        A.resize((l/32) + 1);
    }

    for(int i = 0; i < l; ++i){
        A = LongLeftShift_2(A);
    }

    return A;
}

LongNumber LongMul(LongNumber A, LongNumber B){
    // std::cout << "A: " << A.to_hex_string() << "; B: " << B.to_hex_string() << std::endl;
    int max_size = std::max(A.size(), B.size());
    LongNumber C = LongNumber(2*max_size);
    A.resize(max_size);
    B.resize(max_size);
    
    LongNumber temp = LongNumber(1);
    for(int i = 0; i < max_size; ++i){
        temp = LongMulOneDigit(A, B.number->at(i));
        temp.resize(temp.size() + 1);
        temp = LongShiftDigitsToHigh(temp, i*num_size);
        C = LongAdd(C, temp); // багаторозрядне додавання!
        // std::cout << "c: " << C.to_hex_string() << "; temp: " << temp.to_hex_string() << std::endl;
        // std::cout << "B at i: " << B.number->at(i) << "; i: " << i << std::endl;
    }
    
    return C;
}

int BitLength(LongNumber A){
    for(int i = A.size() - 1; i > -1; --i){
        if(A.number->at(i) != 0){
            return (i+1) * 32;
        }
    }

    return 0;
}

LongNumber get_2_in_power_n(int power){
    LongNumber rez;
    rez.set(1);
    rez.resize((power / 32) + 1);
    rez = LongShiftDigitsToHigh(rez, power);

    return rez;
}

LongNumber LongDivMod(LongNumber A, LongNumber B){
    // int max_size = std::max(A.size(), B.size());
    // A.resize(max_size);
    // B.resize(max_size);
    
    if(B.to_hex_string() == "0"){
        return LongNumber("0");
    }

    LongNumber R, Q, C;
    int k = BitLength(B);
    R.set_by_hex(A.to_hex_string());
    
    int t = 0;
    LongNumber temp;
    while(LongCmp(R, B) != -1){  // багаторозрядне порівняння!
        t = BitLength(R);
        C = LongShiftDigitsToHigh_2(B, t - k);
        while (LongCmp(R, C) == -1){  // багаторозрядне порівняння! вийшло забагато?
            t = t - 1; // тоді повертаємось на біт назад
            C = LongShiftDigitsToHigh_2(B, t - k);
        }
        
        R = LongSub(R, C);
        temp = get_2_in_power_n(t - k);
        // temp.set(1 << (t - k));
        Q = LongAdd(Q, temp); // встановити в Q біт із номером (t – k)
        // std::cout << "Q: " << Q.to_hex_string() << std::endl;  // uncoment this line if you want to know that function is in progress
    }

    return Q;
}

LongNumber LongDivFraction(LongNumber A, LongNumber B){
    if(B.to_hex_string() == "0"){
        return LongNumber("0");
    }

    LongNumber R, Q, C;
    int k = BitLength(B);
    R.set_by_hex(A.to_hex_string());
    
    int t = 0;
    LongNumber temp;
    while(LongCmp(R, B) != -1){  // багаторозрядне порівняння!
        t = BitLength(R);
        C = LongShiftDigitsToHigh_2(B, t - k);
        while (LongCmp(R, C) == -1){  // багаторозрядне порівняння! вийшло забагато?
            t = t - 1; // тоді повертаємось на біт назад
            C = LongShiftDigitsToHigh_2(B, t - k);
        }
        
        R = LongSub(R, C);
        temp = get_2_in_power_n(t - k);
        // temp.set(1 << (t - k));
        Q = LongAdd(Q, temp); // встановити в Q біт із номером (t – k)
        // std::cout << "Q: " << Q.to_hex_string() << std::endl;  // uncoment this line if you want to know that function is in progress
    }

    return R;
}

LongNumber LongPower1(LongNumber A, LongNumber B){
    if(B.to_hex_string() == "0"){
        return LongNumber("1");
    }else if(A.to_hex_string() == "0"){
        return LongNumber("0");
    }else if(A.to_hex_string() == "1"){
        return LongNumber("1");
    }

    LongNumber C = LongNumber("1");
    for(int i = 0; i < B.size(); ++i){
        for(int j = 0; j < 8; ++j){
            if((B.number->at(i) >> j) & 1 == 1){
                C = LongMul(C, A);
            }
            A = LongMul(A, A);
        }
    }

    return C;
}

LongNumber LongPower2 (LongNumber A, LongNumber B){
    if(B.to_hex_string() == "0"){
        return LongNumber("1");
    }else if(A.to_hex_string() == "0"){
        return LongNumber("0");
    }else if(A.to_hex_string() == "1"){
        return LongNumber("1");
    }

    std::cout << "A: " << A.to_hex_string() << "; B: " << B.to_hex_string() << std::endl;

    LongNumber C = LongNumber("1");
    for(int i = B.size() - 1; i >= 0 ; --i){
        for(int j = 31; j >= 0; --j){
            if(((B.number->at(i) >> j) & 1) == 1){
                C = LongMul(C, A);
            }

            if((i != 0) || (j != 0)){
                C = LongMul(C, C);
            }
            
            std::cout << "i: " << i << "; j: " << j << "; C: " << C.to_hex_string() << std::endl;
        }
    }

    return C;
}


