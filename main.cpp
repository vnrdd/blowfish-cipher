#include <iostream>
#include "image/image.h"
#include "cipher/blowfish.cpp"

void DEFAULT(char** argv);
void OFB(char** argv);

int main(int argc, char** argv) {
     std::vector<uint8_t> key;
     readLine(&key, "key");
     Blowfish::keyExtension(P, &key);

    // std::vector<uint8_t> test(8, 0);
    // randFill(&test);

    // std::cout << "\033[33mORIGINAL: \033[0m" ;
    // std::bitset<64> popa(unite8by8(&test, 0));
    // std::cout << popa << "\n";
    // //printVector(&test);

    // std::vector<uint8_t> inversed2 = inverseBits(unite8by8(&test, 0), 2);
    // std::cout << "\n\033[33mINVERSED (every second): \033[0m" ;
    // printVector(&inversed2);
    // std::bitset<64> bOld(unite8by8(&inversed2,0));
    // uint32_t left, right;
    // split64to32(unite8by8(&inversed2, 0), &left, &right);
    // std::cout << " origin: " << bOld << "\n";
    // for(int i = 0; i < 5; ++i) {
    //     Blowfish::bfRound(&left, &right, P[i]);
    //     std::bitset<64> b(unite2by32(left, right));
    //     std::cout << i+1 << " round: ";
    //     compareSamples(&bOld, &b, std::to_string(i+1));
    //     bOld = b;
    // }
    // split64to8(unite2by32(left, right), &inversed2, 0);
    // std::cout << "\033[33mENCRYPTED (5 rounds): \033[0m";
    // printVector(&inversed2);
    // std::cout << "\033[4mFREQUENCY TEST:\033[0m " << frequencyTest(&inversed2) << "\n";
    // std::cout << "\033[4mBATCH TEST:\033[0m " << batchTest(&inversed2, "2") << "\n";
    // std::cout << "\033[4mAUTOCORRELATION TEST:\033[0m " << autocorrelationTest(&inversed2) << "\n";
    // //autocorrelationPlot(&inversed2);

    // std::vector<uint8_t> inversed8 = inverseBits(unite8by8(&test, 0), 8);
    // std::cout << "\n\033[33mINVERSED (every eighth): \033[0m" ;
    // printVector(&inversed8);
    // std::bitset<64> bOld2(unite8by8(&inversed2,0));
    // split64to32(unite8by8(&inversed8, 0), &left, &right);
    // std::cout << " origin: " << bOld2 << "\n";
    // for(int i = 0; i < 5; ++i) {
    //     Blowfish::bfRound(&left, &right, P[i]);
    //     std::bitset<64> b2(unite2by32(left, right));
    //     std::cout << i+1 << " round: ";
    //     compareSamples(&bOld2, &b2, std::to_string(i+1));
    //     bOld2 = b2;
    // }
    // split64to8(unite2by32(left, right), &inversed8, 0);
    // std::cout << "\033[33mENCRYPTED (5 rounds): \033[0m" ;
    // printVector(&inversed8);
    // std::cout << "\033[4mFREQUENCY TEST:\033[0m " << frequencyTest(&inversed8) << "\n";
    // std::cout << "\033[4mBATCH TEST:\033[0m " << batchTest(&inversed8, "8") << "\n";
    // std::cout << "\033[4mAUTOCORRELATION TEST:\033[0m " << autocorrelationTest(&inversed8) << "\n\n";
    // autocorrelationPlot(&inversed8);

    // compareSamples(&inversed2, &inversed8);
   if(argc == 4) DEFAULT(argv);
   if(argc == 3) OFB(argv);

    return 0;
}

void OFB(char** argv){
    switch(fileTypeChecker(argv[1])) {
        case 't': {
            std::vector<uint8_t> source;
            readFromFile(&source, argv[1]);
            std::vector<uint8_t> init(8, 0);
            randFill(&init);
            Blowfish::OFB(&source, &source, &init, P);
            writeToFile(&source, argv[2]);
            break;
        }

        case 'i' : {
            std::vector<std::vector<uint8_t> > source;
            readImage(&source, argv[1]);
            source[0][0] = 0; source[0][1] = 0; source[0][2] = 0;
            std::vector<uint8_t> init(8, 0);
            randFill(&init);
            for(int i = 0; i < source.size(); ++i)
                Blowfish::OFB(&source[i], &source[i], &init, P);

            writeImage(&source, argv[2]);
            break;
        }
    }
}

void DEFAULT(char** argv){
    switch(fileTypeChecker(argv[2])) {
        case 't': {
            std::vector<uint8_t> source;
            readFromFile(&source, argv[2]);
            Blowfish::blowfish(argv[1][0], &source, &source, P);
            writeToFile(&source, argv[3]);
            break;
        }

        case 'i' : {
            std::vector<std::vector<uint8_t> > source;
            readImage(&source, argv[2]);

            std::vector<std::vector<std::vector<uint8_t> > > rounds; fillWithNulls(&rounds, 16, source.size(), source[0].size());

//            for(int i = 0; i < source.size(); ++i) {
//                uint32_t left, right;
//                for (int j = 0; j < source[i].size(); j += 8) {
//                    split64to32(unite8by8(&source[i], j), &left, &right);
//                    if (argv[1][0] == 'E') {
//                        for (int round = 0; round < 16; ++round) {
//                            Blowfish::bfRound(&left, &right, P[round]);
//                            split64to8(unite2by32(left, right), &rounds[round][i], j);
//                        }
//                        swap(&left, &right);
//                        right ^= P[16];
//                        left ^= P[17];
//                    }
//                    else if (argv[1][0] == 'D') {
//                        for (int round = 17; round > 1; --round)
//                            Blowfish::bfRound(&left, &right, P[round]);
//                        swap(&left, &right);
//                        left ^= P[0];
//                        right ^= P[1];
//                    }
//                    split64to8(unite2by32(left, right), &source[i], j);
//                }
//            }
//
//            for(int i = 0; i < rounds.size(); ++i){
//                std::string buf = "res/r" + std::to_string(i) + ".bmp";
//                writeImage(&rounds[i], buf.c_str());
//            }

            for(int i = 0; i < source.size(); ++i)
                Blowfish::blowfish(argv[1][0], &source[i], &source[i], P);

            writeImage(&source, argv[3]);
            break;
        }
    }
}





