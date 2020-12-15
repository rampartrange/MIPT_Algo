//
//  WAVparser.cpp
//  FFTWavProcessing
//
//  Created by Артем Андрианов on 14.12.2020.
//

#include "WAVCompressor.hpp"

WAVFile::WAVFile(const std::string& filename) {
    FILE *file = fopen(filename.c_str(), "rb");
    if (!file)
    {
      std::cout << "Failed open file";
      exit(1);
    }
    
    fread(&header, sizeof(WAVHEADER), 1, file);
    
    data = new char[header.subchunk2Size];
    fread(data, header.subchunk2Size, 1, file);
    
    fclose(file);
}

WAVFile::~WAVFile() {
    delete[] data;
}

void WAVFile::Write(const std::string &filename) {
    FILE *file = fopen(filename.c_str(), "wb");
    fwrite(&header, sizeof(WAVHEADER), 1, file);
    fwrite(data, header.subchunk2Size, 1, file);
    fclose(file);
}

WAVHEADER WAVFile::GetHeader() const {
    return header;
}

char* WAVFile::GetData() const {
    return data;
}

void WAVFile::PrintHeader() const{
    std::cout << header.chunkId[0] << header.chunkId[1] << header.chunkId[2] << header.chunkId[3] << std::endl;
    std::cout << "Chunk size: " << header.chunkSize << std::endl;
    std::cout << header.format[0] << header.format[1] << header.format[2] << header.format[3] << std::endl;
    std::cout << header.subchunk1Id[0] << header.subchunk1Id[1] << header.subchunk1Id[2] << header.subchunk1Id[3] << std::endl;
    std::cout << "SubChunkId1: " << header.subchunk1Size << std::endl;
    std::cout << "Audio format: " << header.audioFormat << std::endl;
    std::cout << "Channels: " <<  header.numChannels << std::endl;
    std::cout << "Sample rate: " << header.sampleRate << std::endl;
    std::cout << "Bits per sample: " << header.bitsPerSample << std::endl;
    std::cout << header.subchunk2Id[0] << header.subchunk2Id[1] << header.subchunk2Id[2] << header.subchunk2Id[3] << std::endl;
}

void WAVFile::TransformData(const std::vector<int>& dataVector) {
        for (int i = 0; i < header.subchunk2Size; ++ i) {
        data[i] = static_cast<char>(dataVector[i]);
    }
    
}

void WAVFile::CompressData(double ratio) {
    std::vector<int> dataVector(header.subchunk2Size);

    for (int i = 0; i < header.subchunk2Size; ++i) {
        dataVector[i] = static_cast<int>(data[i]);
    }
    
    size_t n = FindUpperDegreeOfTwo(header.subchunk2Size);

    dataVector.resize(n);
  
    std::vector<cld> complexVector = MakeComplexVector(dataVector, n);
    MakeFFT(complexVector);


    for (int i = static_cast<int>(n * ratio); i < n; ++i) {
        complexVector[i] = 0;
    }

    MakeInverseFFT(complexVector);
    dataVector = MakeIntVector(complexVector);
    TransformData(dataVector);
} 