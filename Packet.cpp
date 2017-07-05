//
// Created by ramon on 08/06/17.
//

#include "Packet.h"


#include "Packet.h"
#include <iostream>
#include <vector>


bool Packet::equalTo(const Packet &p){
    for (int i = 0; i < p.getPilo_size_(); ++i) {
        if (this->data_[i] != p.data_[i]) return false;
    }
    return true;
}

Packet::Packet(const Packet &p) {
    ByteGF *temp = new ByteGF[p.pilo_size_];
    for (int i = 0; i < p.pilo_size_ ; ++i) {
        temp[i] = p.data_[i] ;
    }
    this->data_ = temp;
    this->pilo_size_ = p.pilo_size_;
    this->max_size_packet_ = p.max_size_packet_;
    this->encoded_ = p.encoded_;
    this->sequence_ = p.sequence_;
    this->num_original_packets_ = p.num_original_packets_;
    this->seed_batch = p.seed_batch;

//    delete temp;
}

void Packet::toString(){
    for (int i = 0; i < pilo_size_ ; ++i) {
        std::cout << this->data_[i] << " ";
    }
}

void Packet::relase() {
    this->data_ = nullptr;
    this->pilo_size_ = 0;
}

ByteGF *Packet::getData_() const {
    return data_;
}

void Packet::setData_(ByteGF *data_) {
    Packet::data_ = data_;
}

unsigned int Packet::getPilo_size_() const {
    return pilo_size_;
}

void Packet::setPilo_size_(unsigned int size_) {
    Packet::pilo_size_ = size_;
}

unsigned int Packet::getMax_size_packet_() const {
    return max_size_packet_;
}

void Packet::setMax_size_packet_(unsigned int header_size_) {
    Packet::max_size_packet_ = header_size_;
}

bool Packet::isEncoded_() const {
    return encoded_;
}

void Packet::setEncoded_(bool encoded_) {
    Packet::encoded_ = encoded_;
//    if (encoded_){
//        if (Packet::encoded_){
//
//        } else {
//            //add header at *data
//            Packet::data_ = addHeader();
//        }
//    } else{
//        if(Packet::encoded_){
//            //take off the header from *data
//            Packet::data_ = removeHeader();
//        }
//    }
}

unsigned int Packet::getSequence_() const {
    return sequence_;
}

void Packet::setSequence_(unsigned int sequence_) {
    Packet::sequence_ = sequence_;
}

unsigned int Packet::getNum_original_pakets() const {
    return num_original_packets_;
}

void Packet::setNum_original_packets(unsigned int num_packets_batch) {
    Packet::num_original_packets_ = num_packets_batch;
}

unsigned int Packet::getSeed_batch() const {
    return seed_batch;
}

void Packet::setSeed_batch(unsigned int seed_batch) {
    Packet::seed_batch = seed_batch;
}

ByteGF *Packet::addHeader() {
    ByteGF *dataWithHeader = new ByteGF[Packet::pilo_size_ + Packet::max_size_packet_];

    for (int i = 0; i < Packet::pilo_size_; ++i) {
        dataWithHeader[i] = Packet::data_[i];
    }

    return dataWithHeader;
}

ByteGF *Packet::removeHeader() {
    ByteGF *dataWithoutHeader = new ByteGF[sizeof(this->data_)/sizeof(ByteGF) -
                                           (sizeof(int) + sizeof(long) + sizeof(long) + sizeof(int))];

    return dataWithoutHeader;
}

void Packet::retrieveHeader() {
//    ByteGF *temp = new ByteGF[this->getPilo_size_()];
    //retrieve header from data
    union
    {
        unsigned long i;
        unsigned char byte[sizeof(unsigned long)];
    } temp;
    //retrieve seed
    for (int i = 0; i < sizeof(int) ; ++i) {
        ByteGF t = this->data_[this->pilo_size_ - sizeof(int) + i];
        temp.byte[i] = t.poly();
    }
    this->setSeed_batch((int)temp.i);
    //retrieve sequence
    temp.i = 0;
    for (int i1 = 0; i1 < sizeof(long); ++i1) {
        ByteGF t = this->data_[this->pilo_size_ - sizeof(int) - sizeof(long) +i1];
        temp.byte[i1] = t.poly();
    }
    this->setSequence_(temp.i);
    //retrieve number of original packages
    temp.i = 0;
    for (int i2 = 0; i2 < sizeof(long) ; ++i2) {
        ByteGF t = this->data_[this->pilo_size_ - sizeof(int) - sizeof(long) - sizeof(long) +i2];
        temp.byte[i2] = t.poly();
    }
    this->setNum_original_packets(temp.i);
    //retrieve original sizes
    unsigned int *original_sizes = new unsigned int[this->getNum_original_pakets()];
    for (int j = 0; j < this->getNum_original_pakets() ; ++j) {
        temp.i=0;
        for (int i = 0; i < sizeof(int); ++i) {
            ByteGF t = this->data_[this->pilo_size_ - sizeof(int) - sizeof(long) - sizeof(long) -
                    ((this->getNum_original_pakets()*sizeof(int))) + (i + (j * sizeof(int)))];
            temp.byte[i] = t.poly();
        }
        original_sizes[j] = temp.i;
    }
    this->original_sizes_ = original_sizes;
    //retrieve max size packet
    temp.i = 0;
    for (int i2 = 0; i2 < sizeof(int) ; ++i2) {
        ByteGF t = this->data_[this->pilo_size_ - sizeof(int) - sizeof(long) - sizeof(long)-
                (this->getNum_original_pakets()*sizeof(int)) - sizeof(int) +i2];
        temp.byte[i2] = t.poly();
    }
    this->setMax_size_packet_(temp.i);
    //retrieve size of original package
    temp.i = 0;
    for (int i3 = 0; i3 < sizeof(int) ; ++i3) {
        ByteGF t = this->data_[this->pilo_size_ - sizeof(int) - sizeof(long) - sizeof(long) -
                (this->getNum_original_pakets()*sizeof(int)) - sizeof(int) - sizeof(int)
                               +i3];
        temp.byte[i3] = t.poly();
    }
    this->setPilo_size_(temp.i);
}