//Never yield.
#include <bits/stdc++.h>
using namespace std;

#define llint long long
int mod = 1e9+7;
#define cinLoop(n, arr) for(int i = 0; i < n; i++) cin >> arr[i];
#define coutLoop(n, arr)    \
    for(int i=0; i<n; i++)  cout << arr[i] << " ";\
    cout << endl;

struct Node {
    char val;
    Node* left;
    Node* right;

    Node(char v = '\0', Node* l = nullptr, Node* r = nullptr) {
        val = v;
        left = l;
        right = r;
    }
};

void insertPath(Node* root, uint32_t code, int length, char c) {
    Node* curr = root;
    for (int i = length - 1; i >= 0; i--) {
        int bit = (code >> i) & 1;
        if (bit == 0) {
            if (!curr->left) curr->left = new Node();
            curr = curr->left;
        } else {
            if (!curr->right) curr->right = new Node();
            curr = curr->right;
        }
    }
    curr->val = c;
}

void decodeFile(const string& inputFile, const string& outputFile) {
    ifstream inFile(inputFile, ios::in | ios::binary);
    if (!inFile) return;

    char alphabet_size_byte;
    if (!inFile.get(alphabet_size_byte)) return;
    int alphabet_size = (unsigned char)alphabet_size_byte;

    vector<pair<char, int>> treeLeaves; 
    for (int i = 0; i < alphabet_size; i++) {
        char c; char len;
        inFile.get(c); inFile.get(len);
        treeLeaves.push_back({c, (int)len});
    }

    uint32_t textLength = 0;
    for (int i = 0; i < 4; i++) {
        char b; inFile.get(b);
        textLength = (textLength << 8) | (unsigned char)b;
    }
    
    Node* root = new Node();
    uint32_t curr_code = 0;
    int prev_len = 0;
    bool first = true;

    for (auto p : treeLeaves) {
        char c = p.first; int len = p.second;
        if (first) { curr_code = 0; first = false; } 
        else {
            curr_code++; 
            if (len > prev_len) curr_code <<= (len - prev_len); 
            else if (len < prev_len) curr_code >>= (prev_len - len); 
        }
        insertPath(root, curr_code, len, c);
        prev_len = len;
    }
    
    string decodedString = "";
    Node* curr = root;
    char byte;
    while (inFile.get(byte) && decodedString.length() < textLength) {
        unsigned char b = (unsigned char)byte;
        for (int i = 7; i >= 0; i--) {
            int bit = (b >> i) & 1;
            if (bit == 0) curr = curr->left; else curr = curr->right;
            if (!curr->left && !curr->right) {
                decodedString += curr->val;
                curr = root; 
                if (decodedString.length() == textLength) break;
            }
        }
    }
    inFile.close();

    ofstream outFile(outputFile, ios::out | ios::binary);
    outFile << decodedString;
    outFile.close();
}

int main(int argc, char* argv[]) {
    if(argc < 3) return 1;
    decodeFile(argv[1], argv[2]);
    return 0;
}