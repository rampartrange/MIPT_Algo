#include "WAVCompressor.hpp"
#include "FFT.hpp"

int main() {

    std::string input;
    std::cin >> input;

    WavFile file(input);

    file.CompressData(0.2);    

    std::string output;
    std::cin >> output;
    
    file.Write(output);

    return 0;
}