#ifndef BLOWFISHFINAL_BITS_H
#define BLOWFISHFINAL_BITS_H
#include<vector>
#include<math.h>
#include<set>
#include "utils.h"
#include<bitset>

void split64to32(uint64_t block, uint32_t *left, uint32_t *right) {
    *left = block >> 32;
    *right = block;
}

void split64to8(uint64_t block, std::vector<uint8_t> *v, int index) {
    for (size_t i = 0; i < 8; ++i)
        (*v)[index + i] = block >> (56 - (i * 8));
}

void split32to8(uint32_t block, uint8_t *splitted) {
    for (uint8_t i = 0; i < 4; ++i)
        splitted[i] = block >> (24 - (i * 8));
}

uint64_t unite2by32(uint32_t left, uint32_t right) {
    uint64_t block;
    block = left;
    block = (block << 32) + right;
    return block;
}

std::vector<uint8_t> inverseBits(uint64_t block, int step){
    std::vector<uint8_t> res(8, 0);
    std::bitset<64> b(block);
    for(int i = 0; i < 64; i += step)
        b.flip(i);
    split64to8((uint64_t)b.to_ulong(), &res, 0);
    return res;
}


uint64_t unite8by8(std::vector<uint8_t> *v, int index) {
    uint64_t block;
    for (int i = index; i < index + 8; ++i)
        block = (block << 8) + (*v)[i];
    return block;
}

uint64_t unite4by8(std::vector<uint8_t> *v, int index) {
    uint64_t block;
    for (int i = index; i < index + 4; ++i)
        block = (block << 8) + (*v)[i];
    return block;
}

std::string frequencyTest(std::vector<uint8_t> *v){
    std::bitset<64> b(unite8by8(v, 0));
    int sigma = 0;
    for(int i = 0; i < b.size(); ++i)
        sigma += b[i];

    double Mx = b.size() / 2;
    double dev = 2 / sqrt(b.size());
    double fX = dev * (sigma - Mx);

    if(-3 < fX && fX < 3) return "\033[32mPASSED\033[0m";
    else return "\033[31mFAILED\033[0m";
}

int findMaxRow(std::bitset<64> *b){
    int row = 1, buf = (*b)[0];
    int maxRow = 1;
    for(int i = 1; i < b->size(); ++i){
        if((*b)[i] == buf) row++;
        else {
            if(row >= maxRow) maxRow = row;
            row = 1;
        }
        buf = (*b)[i];
    }
    return maxRow;
}

int rowCount(std::bitset<64> *b, int row, int bit){
    int count = 0;
    int bitRow = 0;
    for(int i = 1; i < b->size(); ++i){
        if((*b)[i] == bit) bitRow++;
        else {
            if(bitRow == row && (*b)[i] - row - 1 != bit)
                count++;
            bitRow = 0;
        }
    }
    return count;
}

double countSigma(int freqs[], int freqsSize, int sampleSize){
    double sum = 0;
    for(int i = 0; i < freqsSize; ++i){
        if(freqs[i] == 0) continue;
        sum += pow(freqs[i] - (sampleSize / pow(2, i+3)), 2) / (sampleSize / pow(2, i + 3));
    }
    return sum;
}

std::string batchTest(std::vector<uint8_t> *v, std::string filename){
    std::bitset<64> b(unite8by8(v, 0));
    int maxRow = findMaxRow(&b);
    int nulls[maxRow], ones[maxRow];
    for(int i = 1; i <= maxRow; ++i){
        nulls[i-1] = rowCount(&b, i, 0);
        ones[i-1] = rowCount(&b, i, 1);
    }

    std::ofstream out(filename + "0" + ".txt");
    for(int i = 0; i < maxRow; ++i)
        out << i+1 << " " << nulls[i] << "\n";
    out.close();

    out.open(filename + "1" + ".txt");
    for(int i = 0; i < maxRow; ++i)
        out << i+1 << " " << ones[i] << "\n";
    out.close();

    double X[] = {3.84146, 5.99146, 7.81473, 9.48773, 11.07050, 12.59159, 14.06714, 15.50731, 16.91898, 18.30704, 19.67514, 21.02607};
    if(countSigma(nulls, maxRow, 64) + countSigma(ones, maxRow, 64) < X[maxRow-1]) return "\033[32mPASSED\033[0m";
    else return "\033[31mFAILED\033[0m";
}

int countMatched(std::bitset<64> *first, std::bitset<64> *second){
    int count = 0;
    for(int i = 0; i < first->size(); ++i){
        if((*first)[i] == (*second)[i]) count++;
    }
    return count;
}
void autocorrelationPlot(std::vector<uint8_t> *v){
    std::bitset<64> source(unite8by8(v, 0));
    std::ofstream out("auto.txt");
    for(int i = 0; i < 64; ++i){
        std::bitset<64> shifted = source;
        shifted = shifted >> i;
        out << i << " " << (2*(double)countMatched(&source, &shifted) - 64) / (double)64<< "\n";
    }
    out.close();
}

std::string autocorrelationTest(std::vector<uint8_t> *v){
    std::bitset<64> b(unite8by8(v, 0));
    std::bitset<64> b2(unite8by8(v, 0));
    b2 = b2 >> 3;
    b2 ^= b;

    int sigma = 0;
    for(int i = 0; i < b2.size(); ++i)
        sigma += b2[i];

    double Mx = b2.size() / 2;
    double dev = 2 / sqrt(b2.size());
    double fX = dev * (sigma - Mx);

    if(-3 < fX && fX < 3) return "\033[32mPASSED\033[0m";
    else return "\033[31mFAILED\033[0m";
}

void compareSamples(std::vector<uint8_t> *v1, std::vector<uint8_t> *v2){
    std::bitset<64> b1(unite8by8(v1, 0));
    std::bitset<64> b2(unite8by8(v2, 0));
    std::set<int> notMatchedIndexes;

    for(int i = 0; i < b1.size(); ++i) {
        if (b1[i] != b2[i]) notMatchedIndexes.insert(i);
    }

    for(int i = 0; i < b1.size(); ++i){
        if(notMatchedIndexes.find(i) != notMatchedIndexes.end())
            std::cout << "\033[33m" << b1[i] << "\033[0m";
        else std::cout << b1[i];
    }
    std::cout << "\n";
    for(int i = 0; i < b2.size(); ++i){
        if(notMatchedIndexes.find(i) != notMatchedIndexes.end())
            std::cout << "\033[33m" << b2[i] << "\033[0m";
        else std::cout << b2[i];
    }
    std::cout << "\n";
}

void compareSamples(std::bitset<64> *b1, std::bitset<64> *b2, std::string color){
    std::set<int> notMatchedIndexes;

    for(int i = 0; i < b1->size(); ++i) {
        if ((*b1)[i] != (*b2)[i]) notMatchedIndexes.insert(i);
    }

    for(int i = 0; i < b2->size(); ++i){
        if(notMatchedIndexes.find(i) != notMatchedIndexes.end())
            std::cout << "\033[3" + color + "m" << (*b2)[i] << "\033[0m";
        else std::cout << (*b2)[i];
    }
    std::cout << "\n";
}
#endif //BLOWFISHFINAL_BITS_H
