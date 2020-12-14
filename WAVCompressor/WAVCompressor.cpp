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
    PrintHeader();
    
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
    printf("Chunk size: %d\n", header.chunkSize);
    std::cout << header.format[0] << header.format[1] << header.format[2] << header.format[3] << std::endl;
    std::cout << header.subchunk1Id[0] << header.subchunk1Id[1] << header.subchunk1Id[2] << header.subchunk1Id[3] << std::endl;
    printf("SubChunkId1: %d\n", header.subchunk1Size);
    printf("Audio format: %d\n", header.audioFormat);
    printf("Channels: %d\n", header.numChannels);
    printf("Sample rate: %d\n", header.sampleRate);
    printf("Bits per sample: %d\n", header.bitsPerSample);
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
    complexVector = MakeFFT(complexVector);


    for (int i = static_cast<int>(n * ratio); i < n; ++i) {
        complexVector[i] = 0;
    }

    std::vector<cld> reversedVector = MakeInverseFFT(complexVector);
    dataVector = MakeIntVector(reversedVector);
    TransformData(dataVector);
} 