#include <iostream>
#include <string>
#include <vector>
std::vector<long long> CountOddOrEvenPals(std::vector<long long>& palindromsNum, const std::string& text, bool isOdd) {
    int size = text.size();
    
    long long evenIndex = isOdd ? 0 : 1;
    long long left = 0;
    long long right = -1;
    
    for (long long i = 0; i < size; ++i) {
        long long lengthOfPalWithCenterInI = 0;
        
        if (i <= right) {
            lengthOfPalWithCenterInI = std::min(right - i + evenIndex, palindromsNum[right - i + left + evenIndex]);
        }
        
        while ((i + lengthOfPalWithCenterInI + 1 - evenIndex < size) && (i - lengthOfPalWithCenterInI - 1 >= 0) &&
            (text[i + lengthOfPalWithCenterInI + 1 - evenIndex] == text[i - lengthOfPalWithCenterInI - 1])) {
                ++lengthOfPalWithCenterInI;
            }
        
        palindromsNum[i] = lengthOfPalWithCenterInI;
        
        if (i + lengthOfPalWithCenterInI - evenIndex > right) {
            left = i - lengthOfPalWithCenterInI;
            right = i + lengthOfPalWithCenterInI - evenIndex;
        }
    }
    return palindromsNum;
}

long long CountPalindroms(const std::string& text) {
    int size = text.size();
    
    std::vector<long long> oddPalindromsNum(size, 0);
    std::vector<long long> evenPalindromsNum(size, 0);
    
    CountOddOrEvenPals(oddPalindromsNum, text, true);
    CountOddOrEvenPals(evenPalindromsNum, text, false);
    
    long long numberOfPalindroms = 0;
    for (long long i = 0; i < size; ++i) {
        numberOfPalindroms += oddPalindromsNum[i] + evenPalindromsNum[i];
    }
    
    return numberOfPalindroms;
}

int main() {
    std::string text;
    std::getline(std::cin, text);
    std::cout << CountPalindroms(text) << std::endl;
    return 0;
}
