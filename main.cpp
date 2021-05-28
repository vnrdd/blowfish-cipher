#include <iostream>
#include "image/image.h"
#include "cipher/blowfish.cpp"

void DEFAULT(char** argv);
void OFB(char** argv);

int main(int argc, char** argv) {
     std::vector<uint8_t> key;
     readLine(&key, "key");
     Blowfish::keyExtension(P, &key);

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


            for(int i = 0; i < source.size(); ++i)
                Blowfish::blowfish(argv[1][0], &source[i], &source[i], P);

            writeImage(&source, argv[3]);
            break;
        }
    }
}





