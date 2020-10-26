#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

class CSuffixArray {
public:
    explicit CSuffixArray(const std::string&);
    ~CSuffixArray() = default;
    
    std::vector<int> GetSuffixArray() const;
    std::vector<int> BuildLcp() const;
    
    int CountUniqueSubstrings(const std::string& text);
    
private:
    void Build(int);
    int SeparateToClasses();
    
    std::vector<int> suffixArray;
    std::vector<int> classes;
    std::string text;
    static const int alphabetLen;
};

const int CSuffixArray::alphabetLen = 128;

CSuffixArray::CSuffixArray(const std::string& text) : text(text) {
    auto size = this->text.size();
    for (auto i = 0; i < size; ++i) {
        suffixArray.emplace_back(i);
    }
    classes.resize(size);
    
    int classesNum = SeparateToClasses();
    Build(classesNum);
    
}
int CSuffixArray::SeparateToClasses() {
    std::vector<int> cnt(alphabetLen);
    auto size = text.size();
    for (int i = 0; i < size; ++i) {
        ++cnt[static_cast<int>(text[i])];
    }
    
    for (int i = 1; i < alphabetLen; ++i) {
        cnt[i] += cnt[i - 1];
    }
    
    for (int i = 0; i < size; ++i) {
        suffixArray[--cnt[static_cast<int>(text[i])]] = i;
    }
    
    classes[suffixArray[0]] = 0;
    int classesNum = 1;
    
    for (int i = 1; i < size; ++i) {
        if (text[suffixArray[i]] != text[suffixArray[i - 1]]) {
            ++classesNum;
        }
        classes[suffixArray[i]] = classesNum - 1;
    }
    
    return classesNum;
}

void CSuffixArray::Build(int classesNum) {
    auto size = text.size();
    std::vector<int> currentPermutation(size);
    std::vector<int> newClasses(size);
    
    for (int height = 0; (1 << height) < size; ++height) {
        for (int i = 0; i < size; ++i) {
            currentPermutation[i] = (size + suffixArray[i] - (1 << height)) % size;
        }
        std::vector<int> cnt(classesNum);
        for (int i = 0; i < size; ++i) {
            ++cnt[classes[currentPermutation[i]]];
        }
        
        for (int i = 1; i < classesNum; ++i) {
            cnt[i] += cnt[i - 1];
        }
        
        for (int i = size - 1; i >= 0; --i) {
            suffixArray[--cnt[classes[currentPermutation[i]]]] = currentPermutation[i];
        }
        
        newClasses[suffixArray[0]] = 0;
        classesNum = 1;
        for (int i = 1; i < size; ++i) {
            int mid1 = (suffixArray[i] + (1 << height)) % size,  mid2 = (suffixArray[i - 1] + (1 << height)) % size;
            if (classes[suffixArray[i]] != classes[suffixArray[i - 1]] || classes[mid1] != classes[mid2])
                ++classesNum;
            newClasses[suffixArray[i]] = classesNum - 1;
        }
        classes = newClasses;
    }
    
}

std::vector<int> CSuffixArray::BuildLcp() const{
    auto size = text.size();
    std::vector<int> lcp(size);
    std::vector<int> reverseSuffixArray(size);
    
    for (int i = 0; i < size; ++i) {
        reverseSuffixArray[suffixArray[i]] = i;
    }
    int k = 0;
    for (int i = 0; i < size; ++i) {
        if (k > 0) {
            --k;
        }
        if (reverseSuffixArray[i] == size - 1) {
            lcp[size - 1] = -1;
            k = 0;
            continue;
        } else {
            int j = suffixArray[reverseSuffixArray[i] + 1];
            while (std::max(i + k, j + k) < size && text[i + k] == text[j + k]) {
                ++k;
            }
            lcp[reverseSuffixArray[i]] = k;
        }
    }

    return lcp;
}

std::vector<int> CSuffixArray::GetSuffixArray() const{
    return suffixArray;
}

int CSuffixArray::CountUniqueSubstrings(const std::string& text) {
    CSuffixArray suffArr(text);
    std::vector<int> lcp = suffArr.BuildLcp();
    auto size = text.size();
    int answer = 0;
    for (auto element : suffArr.GetSuffixArray()) {
        answer += size - element;
    }
    for (int i = 0; i < size - 1; ++i) {
        answer -= lcp[i];
    }
    return answer;
}

int main() {
    std::string text;
    std::getline(std::cin, text);
    CSuffixArray suffixArray(text);
    std::cout << suffixArray.CountUniqueSubstrings(text) << std::endl;
    return 0;
}
