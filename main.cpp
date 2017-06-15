#include <iostream>
#include <vector>
#include <ctime>

#include "Packet.h"
#include "Coder.h"


int main() {
    int start_s=clock();

    std::vector<Packet> k,n,r;

    int numPack = 3;
    int maxSize = 3;
    int extraPacks = 1;

    Coder c;

    k.reserve(numPack);
    n.reserve(numPack + extraPacks);
    r.reserve(numPack);

    srand(time(NULL));

    // initialize K with random packet sizes and random data
    for (int i = 0; i < numPack; ++i) {
        //initialize packet
        int size = rand() % maxSize + 1;
        Packet p(new ByteGF[size], size);
        for (int j = 0; j < p.pilo_size_ ; ++j) {
            p.data_[j] = rand() % 255;
        }
        k.push_back(p);
    }

    //print out original data k
    std::cout<< std::endl << "Original packages K" << std::endl;
    for (int j = 0; j < k.size(); ++j) {
        std::cout << "Packet " << j << ": ";
        k.at(j).toString();
        std::cout << std::endl;
    }

    //encode k
    int start_e=clock();
    n = c.encode(k,extraPacks);
    int stop_e = clock();


    //print out data encoded
    std::cout << std::endl << "Encoded packages N" << std::endl;
    for (int j = 0; j < n.size(); ++j) {
        std::cout << "Packet " << j << ": ";
        n.at(j).toString();
        std::cout << std::endl;
    }
    std::cout << std:: endl << "Encode time execution: " << (stop_e-start_e)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;

    //lose packets simulation

    //decode n
    int start_d=clock();
    r = c.decode(n);
    int stop_d = clock();

    //print out data decoded
    std::cout << std::endl << "Decoded packages R" << std::endl;
    for (int j = 0; j < r.size(); ++j) {
        std::cout << "Packet " << j << ": ";
        r.at(j).toString();
        std::cout << std::endl;
    }
    std::cout << std:: endl << "Decode time execution: " << (stop_d-start_d)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;


    //end program
    int stop_s = clock();
    std::cout << std:: endl << "Total time  execution: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;


    return 0;
}