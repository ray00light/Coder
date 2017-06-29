#include <iostream>
#include <vector>
#include <ctime>

#include "Packet.h"
#include "Coder.h"



void runTest(int numPack,int maxSize,int extraPacks,int packetsToLose){
    int start_s=clock();

    std::vector<Packet> k,n,r;

    Coder c;

    k.reserve(numPack);
    n.reserve(numPack + extraPacks);
    r.reserve(numPack);

    //srand(time(NULL));

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
//    Packet p0(new ByteGF[3],3);
//    p0.data_[0] = 20;
//    p0.data_[1] = 51;
//    p0.data_[2] = 38;
//    k.push_back(p0);
//    Packet p1(new ByteGF[4],4);
//    p1.data_[0] = 251;
//    p1.data_[1] = 193;
//    p1.data_[2] = 90;
//    p1.data_[3] = 121;
//    k.push_back(p1);
//    Packet p2(new ByteGF[3],3);
//    p2.data_[0] = 151;
//    p2.data_[1] = 25;
//    p2.data_[2] = 174;
//    k.push_back(p2);
//    Packet p(new ByteGF[2], 2);
//    p.data_[0] = 4;
//    p.data_[1] = 7;
//    k.push_back(p);

    //print out original data k
//    std::cout<< std::endl << "Original packages K" << std::endl;
//    for (int j = 0; j < k.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        k.at(j).toString();
//        std::cout << std::endl;
//    }

    //encode k
    int start_e=clock();
    n = c.encode(k,extraPacks);
    int stop_e = clock();


    //print out data encoded
//    std::cout << std::endl << "Encoded packages N" << std::endl;
//    for (int j = 0; j < n.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        n.at(j).toString();
//        std::cout << std::endl;
//    }
    std::cout << std:: endl << "Encode time execution: " << (stop_e-start_e)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;

    //lose packets simulation
//    n = c.losePackets(n);
    n = c.losePackets(n,packetsToLose);
    //print out data encoded
//    std::cout << std::endl << "Encoded packages N after lose emulation " << std::endl;
//    for (int j = 0; j < n.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        n.at(j).toString();
//        std::cout << std::endl;
//    }

    //decode n
    int start_d=clock();
    r = c.decode(n);
    int stop_d = clock();

    //print out data decoded
//    std::cout << std::endl << "Decoded packages R" << std::endl;
//    for (int j = 0; j < r.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        r.at(j).toString();
//        for (int i = r.at(j).getPilo_size_(); i <= maxSize; ++i){
//            std::cout << "\t";
//            if((r.at(j).getPilo_size_() == maxSize) && i == maxSize)
//                std::cout << "\t";
//        }
//
//        k.at(j).toString();
//        std::cout << std::endl;
//    }
    std::cout << std:: endl << "Decode time execution: " << (stop_d-start_d)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;


    //end program
    int stop_s = clock();
    std::cout << std:: endl << "Total time  execution: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000
              << " miliseconds " <<  std::endl;


}

int main() {

    int numPack = 100;
    int maxSize = 1500;
    int extraPacks = 1;
    int packetsToLose = 1;
    int numIterations = 1;

    for (int i=0; i < numIterations; ++i){
//        srand(time(NULL));
        srand(i);
        std::cout << i;
        runTest(numPack,maxSize,extraPacks,packetsToLose);
    }

    return 0;
}



//
//
//int main() {
//    int start_s=clock();
//
//    std::vector<Packet> k,n,r;
//
//    int numPack = 3;
//    int maxSize = 4;
//    int extraPacks = 1;
//    int packetsToLose = 1;
//
//    Coder c;
//
//    k.reserve(numPack);
//    n.reserve(numPack + extraPacks);
//    r.reserve(numPack);
//
//    srand(time(NULL));
//
//    // initialize K with random packet sizes and random data
//    for (int i = 0; i < numPack; ++i) {
//        //initialize packet
//        int size = rand() % maxSize + 1;
//        Packet p(new ByteGF[size], size);
//        for (int j = 0; j < p.pilo_size_ ; ++j) {
//            p.data_[j] = rand() % 255;
//        }
//        k.push_back(p);
//    }
////    Packet p0(new ByteGF[3],3);
////    p0.data_[0] = 20;
////    p0.data_[1] = 51;
////    p0.data_[2] = 38;
////    k.push_back(p0);
////    Packet p1(new ByteGF[4],4);
////    p1.data_[0] = 251;
////    p1.data_[1] = 193;
////    p1.data_[2] = 90;
////    p1.data_[3] = 121;
////    k.push_back(p1);
////    Packet p2(new ByteGF[3],3);
////    p2.data_[0] = 151;
////    p2.data_[1] = 25;
////    p2.data_[2] = 174;
////    k.push_back(p2);
////    Packet p(new ByteGF[2], 2);
////    p.data_[0] = 4;
////    p.data_[1] = 7;
////    k.push_back(p);
//
//    //print out original data k
//    std::cout<< std::endl << "Original packages K" << std::endl;
//    for (int j = 0; j < k.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        k.at(j).toString();
//        std::cout << std::endl;
//    }
//
//    //encode k
//    int start_e=clock();
//    n = c.encode(k,extraPacks);
//    int stop_e = clock();
//
//
//    //print out data encoded
//    std::cout << std::endl << "Encoded packages N" << std::endl;
//    for (int j = 0; j < n.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        n.at(j).toString();
//        std::cout << std::endl;
//    }
//    std::cout << std:: endl << "Encode time execution: " << (stop_e-start_e)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
//
//    //lose packets simulation
////    n = c.losePackets(n);
//    n = c.losePackets(n,packetsToLose);
//    //print out data encoded
//    std::cout << std::endl << "Encoded packages N after lose emulation " << std::endl;
//    for (int j = 0; j < n.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        n.at(j).toString();
//        std::cout << std::endl;
//    }
//
//    //decode n
//    int start_d=clock();
//    r = c.decode(n);
//    int stop_d = clock();
//
//    //print out data decoded
//    std::cout << std::endl << "Decoded packages R" << std::endl;
//    for (int j = 0; j < r.size(); ++j) {
//        std::cout << "Packet " << j << ": ";
//        r.at(j).toString();
//        for (int i = r.at(j).getPilo_size_(); i < maxSize; ++i)std::cout << "\t";
//        k.at(j).toString();
//        std::cout << std::endl;
//    }
//    std::cout << std:: endl << "Decode time execution: " << (stop_d-start_d)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
//
//
//    //end program
//    int stop_s = clock();
//    std::cout << std:: endl << "Total time  execution: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
//
//
//    return 0;
//}