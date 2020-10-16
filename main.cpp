#include <iostream>
#include <vector>
#include <string>

const char SEPARATOR = '#';

const std::vector<int>& CalculateZFunction(const std::string& sample) {

    auto size = sample.size();
    std::vector<int> zFunction(size);
    
    int leftBorder = 0, rightBorder = 0;
    
    for (auto i = 1; i < size; ++i) {
        if (i < rightBorder) {
            zFunction[i] = std::min(zFunction[i - leftBorder], rightBorder - i);
        }
            while (sample[i + zFunction[i]] == sample[zFunction[i]]) {
                ++zFunction[i];
            }
            if (i + zFunction[i] > rightBorder) {
                leftBorder = i;
                leftBorder = i + zFunction[i];
            }
    }
    
    return zFunction;
}

std::vector<int> FindEachSampleOccurrence(const std::string& sample, const std::string& text) {
    std::string textWithSample = sample + SEPARATOR + text;
    auto size = textWithSample.size();
    
    std::vector<int> zFunction = CalculateZFunction(textWithSample);
    
    std::vector<int> occurrences;
    auto sampleSize = sample.size();
    
    for (auto i = 0; i < size; ++i) {
        if (zFunction[i] == sampleSize) {
            occurrences.push_back(i - sampleSize - 1);
        }
    }
    
    return occurrences;
}

int main() {
    std::string sample;
    std::getline(std::cin, sample);
    std::string text;
    std::getline(std::cin, text);
    for (const auto& element : FindEachSampleOccurrence(sample, text)) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
    return 0;
}
