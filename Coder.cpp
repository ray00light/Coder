//
// Created by ramon on 08/06/17.
//

#include <iomanip>
#include "Coder.h"

Coder::Coder() {
    this->seed = time(NULL);
}

Coder::~Coder() {

}

int Coder::maxSizePackN(std::vector<Packet> packets) {
    return packets.at(0).getMax_size_packet_();
}

std::vector<Packet> Coder::encode(std::vector<Packet> k, int numExtraPackets) {
    std::vector<Packet> n;
    int maxPacketSize = maxSizePackK(k);
    int newPacketSize = maxPacketSize + sizeof(unsigned int) + sizeof(unsigned int)+ k.size() * sizeof(unsigned int) +
            sizeof(unsigned long) + sizeof(unsigned long) + sizeof(unsigned int); // new package size will contain maxsize plus header size

    n.reserve(k.size()+ numExtraPackets);

    srand(this->seed);
//    ByteGF coeffMat[(k.size()+numExtraPackets) * maxPacketSize][maxPacketSize * k.size()];
    ByteGF **coeffMat = new ByteGF*[(k.size()+numExtraPackets) * maxPacketSize];//[maxPacketSize * k.size()];
    for (int k1 = 0; k1 < (k.size()+numExtraPackets) * maxPacketSize; ++k1) {
        coeffMat[k1] = new ByteGF[maxPacketSize * k.size()];
    }
    for (int i = 0; i < (k.size()+numExtraPackets) * maxPacketSize; ++i) {
        for (int j = 0; j < maxPacketSize * k.size(); ++j) {
            coeffMat[i][j] = rand() % 255 +1;
        }
    }
    //print coefficients matrix
    std::cout << "coefficients matrix before encode" << std::endl;
    for (int i=0;i<(k.size()+numExtraPackets) * maxPacketSize;i++){
        for ( int j=0;j<maxPacketSize * k.size();j++)
            std::cout <<coeffMat[i][j]<< " ";
        std::cout<<"\n";
    }
//    std::cout << "calculate each paquet breakpoint 1" << std::endl;
    //calculate each packet
    for (int pi = 0; pi < k.size()+ numExtraPackets; ++pi) {
//        std::cout << "breakpoint encode paquet " << pi << std::endl;
        ByteGF  data[newPacketSize];
        Packet e = Packet(data, newPacketSize );
        //create  and put encoded data for each entry
        for (int ei = 0; ei < maxPacketSize; ++ei) {
//            std::cout << "breakpoint calculate entry " << ei << std::endl;
            // for each entry of new packet we have to multiply different coefficient
            ByteGF entry;
            for (int subi = 0; subi < maxPacketSize; ++subi) {
                ByteGF subEntry;
                for (int i = 0; i < k.size(); ++i) {
                    ByteGF a;
                    if (subi < k.size() & k[subi].pilo_size_ > i){
                        a = k[subi].data_[i];
                    }
                    subEntry = subEntry + coeffMat[ei + (pi * maxPacketSize)][(subi * k.size()) + i] * a;
                }
                entry = entry + subEntry;
            }
            e.data_[ei] = entry;
        }
//        std::cout << "calculate each paquet breakpoint 3" << std::endl;
        //add header==========================
        union
        {
            unsigned long i;
            unsigned char byte[sizeof(unsigned long)];
        } temp;

        // add pilo_size_
        temp.i = k[pi].pilo_size_;
        if(k[pi].pilo_size_ > maxPacketSize)temp.i = maxPacketSize;
        for (int i = 0; i < sizeof(int); ++i) {
            ByteGF t = temp.byte[i];
            e.data_[maxPacketSize+i] = t;
        }
        e.setPilo_size_(maxPacketSize + sizeof(unsigned int) + sizeof(unsigned int) + (k.size() * sizeof(int)) +
                                sizeof(unsigned long) + sizeof(unsigned long) + sizeof(unsigned int));
        // add max_size_packet_
        temp.i = maxPacketSize;
        for (int i1 = 0; i1 < sizeof(int); ++i1) {
            ByteGF t = temp.byte[i1];
            e.data_[maxPacketSize + sizeof(int) + i1] = t;
        }
        e.setMax_size_packet_(maxPacketSize);
        //add original sizes
        unsigned int *orignal_sizes = new unsigned int[k.size()];
        for (int j1 = 0; j1 < k.size(); ++j1) {
            temp.i = k[j1].pilo_size_;
            orignal_sizes[j1] = temp.i;
            for (int i = 0; i < sizeof(int); ++i) {
                ByteGF t = temp.byte[i];
                e.data_[maxPacketSize + sizeof(int) + sizeof(int)+ (j1 * sizeof(int)) + i] = t;
            }
        }
        e.original_sizes_ = orignal_sizes;
        //add original number of packages
        temp.i = k.size();
        for (int j = 0; j < sizeof(long); ++j) {
            ByteGF t = temp.byte[j];
            e.data_[maxPacketSize + sizeof(int)+ sizeof(int)+ (k.size() * sizeof(int)) +j] = t;
        }
        e.setSequence_(pi);
        //add sequence
        temp.i = pi;
        for (int l = 0; l < sizeof(long); ++l) {
            ByteGF t = temp.byte[l];
            e.data_[maxPacketSize + sizeof(int)+ sizeof(int) + (k.size() * sizeof(int)) + sizeof(long) + l] = t;
        }
        e.setNum_original_packets(k.size());
        //add seed
        temp.i = this->seed;
        for (int m = 0; m < sizeof(int) ; ++m) {
            ByteGF t = temp.byte[m];
            e.data_[maxPacketSize + sizeof(int) + sizeof(int) + (k.size() * sizeof(int)) + sizeof(long) + sizeof(long) +
                    m] = t;
        }
        e.setSeed_batch(this->seed);
        // update encoded_
        e.setEncoded_(true);
        // finally add encoded packet
        n.push_back(e);
    }
    return n;
}

std::vector<Packet> Coder::decode(std::vector<Packet> n) {
    std::vector<Packet> k;
    int maxSeq;
    int maxPacketSize;
    int numPacksToDecode;

    if (n.size() == 0) return k;
    numPacksToDecode = n.at(0).getNum_original_pakets();
    //retrieve headers
    for (int i = 0; i < n.size(); ++i) {
        n.at(i).retrieveHeader();
        n.at(i).setEncoded_(true);
    }
    //if we don't have minimum packages we finish decode
    if (n.size() < n.at(0).getNum_original_pakets()){
        std::cout << std::endl << "Unable to decode due to not enough packets" << std::endl;
        return k;
    }

    //create coeffMat
    maxSeq = getMaxSeq(n);
    maxPacketSize = maxSizePackK(n);
    srand(n.at(0).getSeed_batch());
//    ByteGF coeffMat[maxSeq * n.size()][maxPacketSize * numPacksToDecode];
    std::cout << "breakpoint decode before create coeffMat" << std::endl;
    ByteGF **coeffMat  = new ByteGF*[maxSeq * maxPacketSize];
    for (int i1 = 0; i1 < maxSeq * maxPacketSize; ++i1) {
        coeffMat[i1] = new ByteGF[maxPacketSize * numPacksToDecode];
    }
    for (int i = 0; i < maxSeq * maxPacketSize; ++i) {
        for (int j = 0; j < maxPacketSize * numPacksToDecode; ++j) {
            coeffMat[i][j] = rand() % 255 +1;
        }
    }
    //print coefficients matrix
//    std::cout << "coefficients matrix before decode" << std::endl;
//    for (int i=0;i<maxSeq * maxPacketSize;i++){
//        for ( int j=0;j<maxPacketSize * numPacksToDecode;j++)
//            std::cout <<coeffMat[i][j]<< " ";
//        std::cout<<"\n";
//    }
    std::cout << "breakpoint coefficients matrix created" << std::endl;
    //populate extended matrix with coefficients and extended entry
    // the populated matrix would be created with first packages of n until numOriginalPackets
    int x = maxPacketSize * numPacksToDecode;
//    ByteGF extendedMat[x][x + 1];
    ByteGF **extendedMat = new ByteGF*[x];
    for (int j1 = 0; j1 < x; ++j1) {
        extendedMat[j1] = new ByteGF[x+1];
    }
    for (int l = 0; l < numPacksToDecode; ++l) {
        int seq = n.at(l).getSequence_();
        for (int i = 0; i < maxPacketSize; ++i) {
            for (int j = 0; j < x + 1 ; ++j) {
                //the extended entry would be an element of n
                if (j == x){
                    extendedMat[i + (l * maxPacketSize)][j] = n.at(l).data_[i];
                } else {
                    extendedMat[i + (l * maxPacketSize)][j] = coeffMat[i + (seq * maxPacketSize)][j];
                }

            }
        }
    }
    std::cout << "breakpoint extended matrix populated" << std::endl;
    std::cout<<"\nThe matrix populated\n";
    //print the new matrix
    for (int i=0;i<x;i++){
        for ( int j=0;j<=x;j++)
            std::cout <<extendedMat[i][j]<< " ";
        std::cout<<"\n";
    }
    ByteGF result[x];
    //Pivotisation
    for (int i=0;i<x;i++)
        for (int k=i+1;k<x;k++)
            if (extendedMat[i][i]<extendedMat[k][i])
                for (int j=0;j<=x;j++){
                    ByteGF temp=extendedMat[i][j];
                    extendedMat[i][j]=extendedMat[k][j];
                    extendedMat[k][j]=temp;
                }
    std::cout<<"breakpoint after pivotisation\n";
    //print the new matrix
//    for (int i=0;i<x;i++){
//        for ( int j=0;j<=x;j++)
//            std::cout <<extendedMat[i][j]<< " ";
//        std::cout<<"\n";
//    }
    //loop to perform the gauss elimination
    for (int i=0;i<x-1;i++)
        for (int k=i+1;k<x;k++){
            ByteGF t=extendedMat[k][i]/extendedMat[i][i];
            for (int j=0;j<=x;j++)
                extendedMat[k][j]=extendedMat[k][j]-t*extendedMat[i][j];    //make the elements below the pivot elements equal to zero or elimnate the variables
        }
    std::cout<<"breakpoint after gauss elimination\n";
//    std::cout<<"\n\nThe matrix after gauss-elimination is as follows:\n";
//    //print the new matrix
//    for (int i=0;i<x;i++){
//        for (int j=0;j<=x;j++)
//            std::cout<<extendedMat[i][j]<<std::setw(16);
//        std::cout<<"\n";
//    }
    //back-substitution
    for (int i=x-1;i>=0;i--){
        //x is an array whose values correspond to the values of x,y,z..
        result[i]=extendedMat[i][x];                //make the variable to be calculated equal to the rhs of the last equation
        for (int j=i+1;j<x;j++)
            if (j!=i)            //then subtract all the lhs values except the coefficient of the variable whose value                                   is being calculated
                result[i]=result[i]-extendedMat[i][j]*result[j];
        result[i]=result[i]/extendedMat[i][i];            //now finally divide the rhs by the coefficient of the variable to be calculated
    }
    //Prepare k
    k.reserve(numPacksToDecode);
//    //retrieve original sizes
//    unsigned int original_sizes[numPacksToDecode];
//    union
//    {
//        unsigned long i;
//        unsigned char byte[sizeof(unsigned long)];
//    } temp;
//    for (int j = 0; j < numPacksToDecode ; ++j) {
//        temp.i=0;
//        for (int i = 0; i < sizeof(int); ++i) {
//            ByteGF t = n.at(0).data_[n.at(0).getPilo_size_()- sizeof(int) - sizeof(long) - sizeof(long) -
//                                   ((numPacksToDecode*sizeof(int))) + (i + (j * sizeof(int)))];
//            temp.byte[i] = t.poly();
//        }
//        original_sizes[j] = temp.i;
//    }
    for (int m = 0; m < numPacksToDecode; ++m) {
        ByteGF temp[n.at(0).original_sizes_[m]];
        for (int i = 0; i < n.at(0).original_sizes_[m]; ++i) {
            temp[i] = result[i + (m * numPacksToDecode)];
        }
        Packet t(temp, n.at(0).original_sizes_[m]);
        t.setNum_original_packets(numPacksToDecode);
        t.setSequence_(m);
        t.setMax_size_packet_(maxPacketSize);
        t.setEncoded_(false);
        k.push_back(t);
    }
//    std::cout<<"\nThe values of the variables are as follows:\n";
//    for (int i=0;i<x;i++)
//        std::cout<<result[i]<<std::endl;


    return k;
}

int Coder::maxSizePackK(std::vector<Packet> packets) {
    int result = 0;
    for (int i = 0; i < packets.size() ; ++i) {
        if (packets.at(i).getPilo_size_() > result) result = packets.at(i).getPilo_size_();
    }
    return result;
}

int Coder::getMaxSeq(std::vector<Packet> n) {
    int maxSeq = 0;
    for (int i = 0; i < n.size(); ++i) {
        if (n.at(i).getSequence_() > maxSeq) maxSeq = n.at(i).getSequence_();
    }
    return maxSeq;
}
