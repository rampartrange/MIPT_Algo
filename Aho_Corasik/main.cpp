#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <string_view>


class CTrie {
public:
    CTrie(const std::vector<std::string_view>& samples, const std::vector<int>& startSamplePositions);
    std::vector<int> GetEachEntryInText(const std::string& sample, const std::string& text);
    ~CTrie();
private:
    struct SVertex;
    
    void AddString(const std::string_view& sample, int sampleNum);
    std::vector<int> startSamplePositions;
    std::vector<std::string_view> samples;
    SVertex* SuffLink(SVertex* v);
    SVertex* Transition(SVertex* v, char move);
    SVertex* GetUp(SVertex* v);
    
    SVertex* root;
    
};

struct CTrie::SVertex {
    SVertex(SVertex* parent, char parentChar);
    ~SVertex();
    
    SVertex* parent;
    char parentChar;

    SVertex* suffLink;
    SVertex* up;
    
    std::unordered_map<char, SVertex*> children;
    std::unordered_map<char, SVertex*> transitions;
    std::vector<int> sampleNums;
    
    bool isTerminal;
    
    static const std::string alphabet;
};

const std::string CTrie::SVertex::alphabet = "abcdefghijklmnopqrstuvwxyz";

CTrie::SVertex::SVertex(SVertex* parent, char parentChar) : parent(parent), parentChar(parentChar),
                                                            isTerminal(false), suffLink(NULL), up(NULL) {};

CTrie::SVertex::~SVertex() {

    for (const auto& [key, node] : children) {
        delete node;
    }
}

CTrie::CTrie(const std::vector<std::string_view>& samples, const std::vector<int>& startSamplePositions) :
             samples(samples), startSamplePositions(startSamplePositions) {
    root = new SVertex(NULL, '#');
    int sampleNum = 0;
    for (const auto& sample : samples) {
        AddString(sample, sampleNum);
        ++sampleNum;
    }
    if (sampleNum == 0) {
        root->isTerminal = true;
        root->sampleNums.push_back(0);
    }
}

CTrie::~CTrie() {
    delete root;
}

void CTrie::AddString(const std::string_view& sample, int sampleNum) {
    SVertex* currentVertex = root;
    auto size = sample.size();
    for (int i = 0; i < size; ++i) {
        if (currentVertex->children.find(sample[i]) == currentVertex->children.end()) {
            currentVertex->children[sample[i]] = new SVertex(currentVertex, sample[i]);
        }
        currentVertex = currentVertex->children[sample[i]];
    }
    currentVertex->sampleNums.push_back(sampleNum);
    currentVertex->isTerminal = true;
}

CTrie::SVertex* CTrie::SuffLink(SVertex* v) {
    if (v->suffLink == NULL) {
        if (v == root || v->parent == root) {
            v->suffLink = root;
        } else {
            v->suffLink = Transition(SuffLink(v->parent), v->parentChar);
        }
    }
    return v->suffLink;
}

CTrie::SVertex* CTrie::Transition(SVertex *v, char move) {
    if (v->transitions.find(move) == v->transitions.end()) {
        if (v->children.find(move) != v->children.end()) {
            v->transitions[move] = v->children[move];
        } else if (v == root) {
            v->transitions[move] = root;
        } else {
            v->transitions[move] = Transition(SuffLink(v), move);
        }
    }
    return v->transitions[move];
}

CTrie::SVertex* CTrie::GetUp(SVertex *v) {
    if (v->up == NULL) {
        if (SuffLink(v)->isTerminal) {
            v->up = SuffLink(v);
        } else if (SuffLink(v) == root) {
            v->up = root;
        } else {
            v->up = GetUp(SuffLink(v));
        }
    }
    return v->up;
}

std::vector<int> CTrie::GetEachEntryInText(const std::string& sample, const std::string& text) {
    auto sampleSize = sample.size();
    auto textSize = text.size();
    
    std::vector<int> answer;
    std::vector<int> entries (textSize, 0);
    
    SVertex* v = root;
    
    for (auto i = 0; i < textSize; ++i) {
        v = Transition(v, text[i]);
        if (v == root && v->isTerminal) {
            if (i + 1 >= sampleSize) {
                ++entries[i - sampleSize + 1];
                answer.push_back(i - sampleSize + 1);
            }
        }
        for (SVertex* u = v; u != root; u = GetUp(u)) {
            if (u->isTerminal) {
                if (samples.empty()) {
                    if (i + 1 >= sampleSize) {
                        ++entries[i - sampleSize + 1];
                        answer.push_back(i - sampleSize + 1);
                    }
                }
                else {
                    for (auto pos : u->sampleNums) {
                        if (i + 1 >= samples[pos].size() + startSamplePositions[pos]) {
                            ++entries[i - samples[pos].size() + 1 - startSamplePositions[pos]];
                        }
                    }
                }
            }
        } 
    }
    if (samples.empty()) {
        return answer;
    }
    
    for (auto i = 0; i < textSize; ++i) {
        if ((textSize - i + 1 > sampleSize) && (entries[i] == samples.size())) {
            answer.push_back(i);
        }
    }
    return answer;
}

std::vector<std::string_view> ParseSample(std::string& sample, std::vector<int>& startSamplePositions) {
    std::vector<std::string_view> parsedSample;
    std::string_view currentSubstring = sample;
    auto size = sample.size();
    size_t pos = 0;
    size_t count = 0;
    for (auto i = 0; i < size; ++i) {
        if (sample[i] != '?') {
            if (count == 0) {
                pos = i;
            }
            count += 1;
        } else if (count){
            parsedSample.emplace_back(currentSubstring.substr(pos, count));
            startSamplePositions.push_back(pos);
            count = 0;
        }
    }
    
    if (count) {
        parsedSample.emplace_back(currentSubstring.substr(pos, count));
        startSamplePositions.push_back(pos);
    }
    
    if (startSamplePositions.empty()) {
        startSamplePositions.push_back(0);
    }
    
    return parsedSample;
}

int main() {
    std::string sample;
    std::string text;
    std::getline(std::cin, sample);
    std::getline(std::cin, text);
    
    std::vector<int> startSamplePositions;
    std::vector<std::string_view> parsedSample = ParseSample(sample, startSamplePositions);
    CTrie trie(parsedSample, startSamplePositions);

    for (auto pos : trie.GetEachEntryInText(sample, text)) {
        std::cout << pos << " ";
    }
    
    return 0;
}
