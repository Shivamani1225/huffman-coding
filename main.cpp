#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <memory>

class HuffmanNode {
public:
    char symbol;
    int frequency;
    std::unique_ptr<HuffmanNode> left, right;

    HuffmanNode(char sym, int freq) : symbol(sym), frequency(freq) {}
};

struct CompareNodes {
    bool operator()(const std::unique_ptr<HuffmanNode>& a, const std::unique_ptr<HuffmanNode>& b) {
        return a->frequency > b->frequency;
    }
};

class HuffmanEncoder {
private:
    std::unordered_map<char, int> frequencyMap;
    std::unique_ptr<HuffmanNode> root;

    void generateCodes(HuffmanNode* node, const std::string& code, std::vector<std::string>& codes) const {
        if (!node) return;
        
        if (node->symbol != '\0') {
            codes[node->symbol] = code;
        }

        generateCodes(node->left.get(), code + "0", codes);
        generateCodes(node->right.get(), code + "1", codes);
    }

public:
    void buildTree() {
        std::priority_queue<std::unique_ptr<HuffmanNode>, std::vector<std::unique_ptr<HuffmanNode>>, CompareNodes> minHeap;

        for (const auto& pair : frequencyMap) {
            minHeap.push(std::make_unique<HuffmanNode>(pair.first, pair.second));
        }

        while (minHeap.size() > 1) {
            auto left = std::move(minHeap.top()); minHeap.pop();
            auto right = std::move(minHeap.top()); minHeap.pop();

            auto newNode = std::make_unique<HuffmanNode>('\0', left->frequency + right->frequency);
            newNode->left = std::move(left);
            newNode->right = std::move(right);

            minHeap.push(std::move(newNode));
        }

        root = std::move(minHeap.top());
    }

    void addSymbol(char symbol, int frequency) {
        frequencyMap[symbol] = frequency;
    }

    std::vector<std::string> encodedSymbols() const {
        std::vector<std::string> codes(256);  // Assuming ASCII
        generateCodes(root.get(), "", codes);
        return codes;
    }
};

int main() {
    HuffmanEncoder encoder;
    int symbolCount;

    std::cout << "Enter the number of symbols: ";
    std::cin >> symbolCount;

    for (int i = 0; i < symbolCount; ++i) {
        char symbol;
        int frequency;
        std::cout << "Enter symbol and its frequency: ";
        std::cin >> symbol >> frequency;
        encoder.addSymbol(symbol, frequency);
    }

    encoder.buildTree();

    std::cout << "Huffman Codes:\n";
    auto codes = encoder.encodedSymbols();
    for (char c = 0; c < 256; ++c) {
        if (!codes[c].empty()) {
            std::cout << c << ": " << codes[c] << '\n';
        }
    }

    return 0;
}
