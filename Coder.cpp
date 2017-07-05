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
    int start_e1 = clock();
//    ByteGF **coeffMat = new ByteGF*[k.size() + numExtraPackets];//[maxPacketSize * k.size()];
    ByteGF coeffMat[k.size() + numExtraPackets][k.size()];
    for (int k1 = 0; k1 < k.size() + numExtraPackets; ++k1) {
//        int numCoeff = k.size();
//        if (maxPacketSize > k.size()){
//            numCoeff = maxPacketSize;
//        }
//        coeffMat[k1] = new ByteGF[k.size()];
        for (int j = 0; j < k.size(); ++j) {
            coeffMat[k1][j] = rand() % 255; // + 1;
        }
    }
//    std::cout << "coefficients matrix before decode" << std::endl;
//    for (int i=0;i<k.size() + numExtraPackets;i++){
//        for ( int j=0;j<k.size();j++)
//            std::cout <<coeffMat[i][j]<< " ";
//        std::cout<<"\n";
//    }
    int stop_e1 = clock();
//    std::cout << std:: endl << "Time execution creating coefficients before encode: " << (stop_e1-start_e1)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
    //print coefficients matrix
//    std::cout << "calculate each paquet breakpoint 1" << std::endl;
    //calculate each packet
    int start_e2 = clock();
    for (int pi = 0; pi < k.size()+ numExtraPackets; ++pi) {
//        std::cout << "breakpoint encode paquet " << pi << std::endl;
        ByteGF  data[newPacketSize];
        Packet e = Packet(data, newPacketSize );
        //create  and put encoded data for each entry
        for (int ei = 0; ei < maxPacketSize; ++ei) {
//            std::cout << "breakpoint calculate entry " << ei << std::endl;
            // for each entry of new packet we have to multiply different coefficient
            ByteGF entry;
            for (int subi = 0; subi < k.size(); ++subi) {
                ByteGF a,b;
                if(k[subi].pilo_size_ > ei){
                    a = k[subi].data_[ei];
                }
                b = coeffMat[pi][subi];
                entry = entry + b * a;
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
    int stop_e2 = clock();
//    std::cout << std:: endl << "Time execution after encode: " << (stop_e2-start_e2)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
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
    maxPacketSize = n.at(0).getMax_size_packet_();// maxSizePackK(n);
    srand(n.at(0).getSeed_batch());
//    std::cout << "breakpoint decode before create coeffMat maxsize " << maxPacketSize << std::endl;
    int start_d1 = clock();
    ByteGF **coeffMat  = new ByteGF*[(maxSeq +1)];
//    ByteGF coeffMat[maxSeq +1][numPacksToDecode];
    for (int i1 = 0; i1 < (maxSeq +1); ++i1) {
        coeffMat[i1] = new ByteGF[numPacksToDecode];
        for (int j = 0; j < numPacksToDecode; ++j) {
            coeffMat[i1][j] = rand() % 255; // + 1;
        }
    }
    ByteGF **coeffMat2 = new ByteGF*[numPacksToDecode];
//    ByteGF coeffMat2[numPacksToDecode][numPacksToDecode];
    for (int l = 0; l < numPacksToDecode; ++l) {
//        for (int i = 0; i < numPacksToDecode; ++i) {
//            coeffMat2[l][i] = coeffMat[n.at(l).getSequence_()][i];
//        }
        coeffMat2[l] = coeffMat[n.at(l).getSequence_()];
    }
    // decomposition of coeff2
    ByteGF **lu = new ByteGF*[numPacksToDecode];
    for (int j1 = 0; j1 < numPacksToDecode; ++j1) {
        lu[j1] = new ByteGF[numPacksToDecode];
    }
//    ByteGF lu[numPacksToDecode][numPacksToDecode];
    ByteGF sum;
    for (int i = 0; i < numPacksToDecode; i++)
    {
        for (int j = i; j < numPacksToDecode; j++)
        {
            sum = 0;
            for (int k = 0; k < i; k++)
                sum = sum + lu[i][k] * lu[k][j];
                lu[i][j] = coeffMat2[i][j] - sum;
        }
        for (int j = i + 1; j < numPacksToDecode; j++)
        {
            sum = 0;
            for (int k = 0; k < i; k++)
                sum = sum + lu[j][k] * lu[k][i];
                lu[j][i] = (1 / lu[i][i]) * (coeffMat2[j][i] - sum);
        }
    }
    int stop_d1 = clock();
//    std::cout << std:: endl << "Time execution for creating coefficients to decode : " << (stop_d1-start_d1)/double(CLOCKS_PER_SEC)*1000
//              << " miliseconds " <<  std::endl;
    //print coefficients matrix
//    std::cout << "coefficients matrix before decode" << std::endl;
//    for (int i=0;i<numPacksToDecode;i++){
//        for ( int j=0;j<numPacksToDecode;j++)
//            std::cout << coeffMat2[i][j] << " ";
//        std::cout << std:: endl;
//    }
//    std::cout << std:: endl;
//    std::cout << "breakpoint coefficients matrix created" << std::endl;
    //decode by each entry
    ByteGF **result= new ByteGF*[maxPacketSize];
    for (int l1 = 0; l1 < maxPacketSize; ++l1) {
        ByteGF *rightPart = new ByteGF[numPacksToDecode];
        for (int i = 0; i < numPacksToDecode; ++i) {
            rightPart[i] = n.at(i).data_[l1];
//            std::cout << rightPart[i] << " ";
        }
//        std::cout << std:: endl;
        // find solution of Ly = b
        ByteGF *y = new ByteGF[numPacksToDecode];
        for (int i = 0; i < numPacksToDecode; i++)
        {
            sum = 0;
            for (int k = 0; k < i; k++)
                sum = sum + lu[i][k] * y[k];
            y[i] = rightPart[i] - sum;
//            std::cout << y[i] << " ";
        }
//        std::cout << std:: endl;
        // find solution of Ux = y
        ByteGF *x = new ByteGF[numPacksToDecode];
        for (int i = numPacksToDecode - 1; i >= 0; i--)
        {
            sum = 0;
            for (int k = i + 1; k < numPacksToDecode; k++)
                sum = sum + lu[i][k] * x[k];
            x[i] = (1 / lu[i][i]) * (y[i] - sum);
//            std::cout << x[i] << "x[" << i << "]"<< " ";
        }
//        std::cout <<"     " << l1 <<  std::endl;
        result[l1] = x;
//        std::cout << "Iteration " << l1<< std:: endl;
//        for (int j = 0; j < numPacksToDecode; ++j) {
//            std::cout << result[l1][j] << " ";
//        }
//        std::cout << std:: endl;
//        delete y;
//        delete x;
//        delete rightPart;
    }
//    for (int k1 = 0; k1 < maxPacketSize; ++k1) {
//        std::cout << "Iteration " << k1<< std:: endl;
//        for (int i = 0; i < numPacksToDecode; ++i) {
//            std::cout << result[k1][i] << " ";
//        }
//        std::cout << std:: endl;
//    }
    int start_d2 = clock();
    k.reserve(numPacksToDecode);

    for (int m = 0; m < numPacksToDecode; ++m) {
        ByteGF temp[n.at(0).original_sizes_[m]];
        for (int i = 0; i < n.at(0).original_sizes_[m]; i++) {
            temp[i] = result[i][m];
//            std::cout << result[i][m] << " ";
        }
//        std::cout << std:: endl;
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

std::vector<Packet> Coder::losePackets(std::vector<Packet> n) {
    srand(time(NULL));
    int numPacketsToLose = rand() % n.size() - 1 ;
    if (numPacketsToLose > 0 ){
        for (int i = 0; i < numPacketsToLose ; ++i) {
            n.erase(n.begin() + (rand() % n.size()));

        }
    }
    return n;
}

std::vector<Packet> Coder::losePackets(std::vector<Packet> n, int numPacketsToLose) {
    for (int i = 0; i < numPacketsToLose ; ++i) {
        n.erase(n.begin() + (rand() % n.size()));
    }
    return n;
}
