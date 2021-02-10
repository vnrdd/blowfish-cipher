#include "../util/bits.h"

class Blowfish {
public:
    static uint32_t F(uint32_t block) {
        uint8_t splitted[4];
        split32to8(block, splitted);
        for(int i = 0; i < 4; ++i)
            block += S[i][splitted[i]];
        return block;
    }

    static void bfRound(uint32_t *left, uint32_t *right, uint32_t key) {
        *left ^= key;
        uint32_t temp = *left;
        *left = F(*left);
        *left ^= *right;
        *right = temp;
    }

public:
    static void blowfish(char mode, std::vector<uint8_t> *src, std::vector<uint8_t> *dst, uint32_t *extendedKey) {
        uint32_t left, right;
        for (int i = 0; i < src->size(); i += 8) {
            split64to32(unite8by8(src, i), &left, &right);
            if(mode == 'E') {
                for (int round = 0; round < 16; ++round) {
                    bfRound(&left, &right, extendedKey[round]);
                }
                swap(&left, &right);
                right ^= extendedKey[16];
                left ^= extendedKey[17];
            }
            else if(mode == 'D') {
                for (int round = 17; round > 1; --round)
                    bfRound(&left, &right, extendedKey[round]);
                swap(&left, &right);
                left ^= extendedKey[0];
                right ^= extendedKey[1];
            }
            split64to8(unite2by32(left, right), dst, i);
        }
    }

    static void OFB(std::vector<uint8_t> *src, std::vector<uint8_t> *dst, std::vector<uint8_t> *init, uint32_t *extendedKey){
        for(int i = 0; i < src->size(); i+=8){
            blowfish('E', init, init, extendedKey);
            xorWrite(src, dst, init, i);
        }
    }

    static void keyExtension(uint32_t *lowKey, std::vector<uint8_t> *highKey) {
        for (int i = 0; i < 18; ++i)
            lowKey[i] ^= unite4by8(highKey, (i * 4) % highKey->size());

        std::vector<uint8_t> init(8, 0);
        for (int i = 0; i < 18; i += 2) {
            blowfish('E', &init, &init, lowKey);
            lowKey[i] = unite4by8(&init, 0);
            lowKey[i + 1] = unite4by8(&init, 4);
        }
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 256; j += 2) {
                blowfish('E', &init, &init, lowKey);
                S[i][j] = unite4by8(&init, 0);
                S[i][j + 1] = unite4by8(&init, 4);
            }
        }
    }
};

