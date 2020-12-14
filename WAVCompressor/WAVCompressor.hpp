//
//  WAVparser.hpp
//  FFTWavProcessing
//
//  Created by Артем Андрианов on 14.12.2020.
//

#ifndef WAVparser_hpp
#define WAVparser_hpp

#include <iostream>
#include <string>
#include <FFT.hpp>

struct WAVHEADER {
    char chunkId[4];
    unsigned int chunkSize;
 
    char format[4];
    char subchunk1Id[4];
 
    unsigned int subchunk1Size;
    unsigned short audioFormat;
    unsigned short numChannels;
 
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short blockAlign;
 
    unsigned short bitsPerSample;
    char subchunk2Id[4];
    unsigned int subchunk2Size;
};

class WAVFile {
public:
    WAVFile(const std::string& filename);
    ~WAVFile();
    
    void CompressData(double ratio);

    void Write(const std::string &filename);

    char* GetData() const;
    WAVHEADER GetHeader() const;
    void PrintHeader() const;

private:
    void TransformData(const std::vector<int>& dataVector);
    WAVHEADER header;
    char* data;
    FILE* file;
};

#endif /* WAVparser_hpp */
