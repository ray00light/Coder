//
// Created by ramon on 08/06/17.
//

#ifndef CODER_CODER_H
#define CODER_CODER_H


#include <vector>

#include "Packet.h"

class Coder {
public:
    unsigned int seed;

    Coder();
    virtual ~Coder();

    std::vector<Packet> encode(std::vector<Packet> k, int extraBytes);

    int maxSizePackN(std::vector<Packet> packets);

    std::vector<Packet> decode(std::vector<Packet> n);

    int maxSizePackK(std::vector<Packet> packets);

    int getMaxSeq(std::vector<Packet> n);

    std::vector<Packet> losePackets(std::vector<Packet> n);

    std::vector<Packet> losePackets(std::vector<Packet> n, int numPacketsToLose);
};



#endif //CODER_CODER_H
