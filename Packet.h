//
// Created by ramon on 08/06/17.
//

#ifndef CODER_PACKET_H
#define CODER_PACKET_H


#include <memory>

#include "ByteGF.h"

class Packet {
public:
    ByteGF *data_;
    unsigned int pilo_size_;
    unsigned int max_size_packet_;
    bool encoded_;
    unsigned int *original_sizes_;
    unsigned long sequence_;
    unsigned long num_original_packets_;
    unsigned int seed_batch;

    Packet(); // default constructor

    Packet(const Packet &p); // copy constructor

    Packet(ByteGF *data, unsigned int size):data_(data),pilo_size_(size),max_size_packet_(0),encoded_(false),sequence_(0),num_original_packets_(0),seed_batch(0)
    {
    }

    ByteGF *getData_() const;

    void setData_(ByteGF *data_);

    unsigned int getPilo_size_() const;

    void setPilo_size_(unsigned int size_);

    unsigned int getMax_size_packet_() const;

    void setMax_size_packet_(unsigned int header_size_);

    bool isEncoded_() const;

    void setEncoded_(bool encoded_);

    unsigned int getSequence_() const;

    void setSequence_(unsigned int sequence_);

    unsigned int getNum_original_pakets() const;

    void setNum_original_packets(unsigned int num_packets_batch);

    unsigned int getSeed_batch() const;

    void setSeed_batch(unsigned int seed_batch);

    void toString ();

    void retrieveHeader();

    ~Packet(){}

    void relase();

    ByteGF *addHeader();

    ByteGF *removeHeader();
};



#endif //CODER_PACKET_H
