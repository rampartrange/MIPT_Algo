#include <iostream>
#include <vector>
#include <string>

std::vector<int> FindEachSampleOccurrence(const std::string& sample, const std::string& text) {
    std::string textWithSample = sample + "#" + text;
    int size = textWithSample.size();
    
    std::vector<int> zFuction(size, 0);
    
    int l = 0, r = 0;
    
    for (int i = 1; i < size; ++i) {
        if (i < r) {
            zFuction[i] = std::min(zFuction[i-l], r - i);
        }
            while (textWithSample[i + zFuction[i]] == textWithSample[zFuction[i]]) {
                ++zFuction[i];
            }
            if (i + zFuction[i] > r) {
                l = i;
                r = i + zFuction[i];
            }
    }
    
    std::vector<int> occurrences;
    int sampleSize = sample.size();
    
    for (int i = 0; i < size; ++i) {
        if (zFuction[i] == sampleSize) {
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
