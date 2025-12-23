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
    int freq;
    char val;
    Node* left;
    Node* right;
    int height;
    int priority;

    Node(int f,int p, int h=0, char v = '\0',  Node* l = nullptr, Node* r = nullptr) {
        freq = f;
        val = v;
        left = l;
        right = r;
        height = h;
        priority = p;
    }
};



struct Comp {
    bool operator()(Node* l, Node* r) {
        if (l->freq != r->freq)
            return l->freq > r->freq;

        if(l->height != r->height) return l->height > r->height;

        if(l->priority != r->priority) return l->priority > r->priority;

        return l->val > r->val;
    }
};


void traverse(Node* node, uint32_t code_bits, int code_length, unordered_map<char, pair<uint32_t, int>> &rep){
    if(!node)   return;  
    if((!node->left) && (!node->right)){
        rep[node->val] = make_pair(code_bits, code_length);
        // cout << node->val << " :- " << node->priority << " :- " << int(code_length)   << endl;
        return;
    }

    traverse(node->left, (code_bits<<1), code_length+1, rep);
    traverse(node->right, ((code_bits<<1)|1), code_length+1, rep);
}

bool Comp2(char a, char b, const unordered_map<char, int>& lengths) {
    if (lengths.at(a) != lengths.at(b)) {
        return lengths.at(a) < lengths.at(b); 
    }
    return a < b; 
}

void buildHuffmanTree(vector<uint8_t> &encoded, string text){
    unordered_map<char, int> freq;
    for(auto ch: text)  freq[ch]++;

    vector<char> alphabet;
    for(auto p: freq)   alphabet.push_back(p.first);
    sort(alphabet.begin(), alphabet.end());

    priority_queue<Node*, vector<Node*>, Comp> pq;
    int priority = 0;
    for(auto c: alphabet){
        // cout << c;
        Node* node = new Node(freq[c], 0, 0, c);
        pq.push(node);
    }
    // cout << endl;   
    
    unordered_map<char, int> mp; // ToBeDestroyed
    unordered_map<Node*, string> mp2;   //ToBeDestroyed

    queue<Node*> qp;
    while(pq.size()){
        alphabet[priority] = pq.top()->val;
        priority++;
        qp.push(pq.top());
        pq.top()->priority = priority;
        mp[pq.top()->val] = priority; //ToBeDestroyed
        mp2[pq.top()] = pq.top()->val;  //ToBeDestroyed
        // cout << mp2[pq.top()] << endl;      //ToBeDestroyed 
        pq.pop();
    }
    while(qp.size()){
        pq.push(qp.front());
        qp.pop();
    }

    while(pq.size()!=1){
        Node* a = pq.top();
        pq.pop();
        Node* b  = pq.top();
        pq.pop();
        priority = min(a->priority, b->priority); //check if to be changed
        
        if(a->priority < b->priority){
            Node* node = new Node((a->freq+b->freq), priority, max(a->height, b->height)+1, '\0',  a, b);
            mp2[node] = mp2[a]+mp2[b]; //ToBeDestroyed
            // cout << mp2[node] << " - left - " << mp2[node->left] << " - right - " << mp2[node->right] << endl;      //ToBeDestroyed
            pq.push(node);
        }   
        else{
            Node* node = new Node((a->freq+b->freq), priority, max(a->height, b->height)+1, '\0',  b, a);
            mp2[node] = mp2[b]+mp2[a]; //ToBeDestroyed
            // cout << mp2[node] << " - left - " << mp2[node->left] << " - right - " << mp2[node->right] << endl;      //ToBeDestroyed
            pq.push(node);
        }
        
        // mp2[node] = mp2[a]+mp2[b]; //ToBeDestroyed
        // cout << mp2[node] << " - left - " << mp2[node->left] << " - right - " << mp2[node->right] << endl;      //ToBeDestroyed
        // pq.push(node);
    }

    // cout << endl;
     
    Node* rootNode = pq.top();

    uint8_t pos = 0, curr = 0;

    unordered_map<char, pair<uint32_t, int>> rep;
    traverse(rootNode, 0, 0, rep);

    for(auto c: alphabet){
        // cout << c << " : freq(" << freq[c] << ")" << endl;
    }

    encoded.push_back(freq.size());
    for(auto c: mp2[pq.top()]){
        int ascii = c;
        encoded.push_back(ascii);
        encoded.push_back(rep[c].second);
    }

    int textLength = text.length();
    for(int i=0; i<4; i++){
        uint8_t num = textLength>>(8*(3-i));
        encoded.push_back(num);
        textLength = (textLength ^ (num << (8*(3-i))));
    }

    for(char ch: text){
        uint32_t bin = rep[ch].first;
        int len = rep[ch].second;
        
        while(len){
            int fill_length = min(len, 8-pos);
            curr = ((curr << fill_length)|(bin>>(len-fill_length)));
            bin = (bin ^ ((bin >> (len-fill_length))<<(len-fill_length)));
            len -= fill_length;
            pos += fill_length;

            if(pos == 8){
                encoded.push_back(curr);
                curr = 0;
                pos = 0;
            }
        }
    }

    if(pos){
        curr = (curr<<(8-pos));
        encoded.push_back(curr);
    }
}

void saveToFile(const string& filename, const vector<uint8_t>& encoded) {
    ofstream outFile(filename, ios::out | ios::binary);
    if (outFile.is_open()) {
        outFile.write(reinterpret_cast<const char*>(encoded.data()), encoded.size());
        outFile.close();
    } else {
        cerr << "Error: Could not open output file." << endl;
    }
}

int main(int argc, char* argv[]){
    if(argc < 3) return 1;

    ifstream inFile(argv[1], ios::binary); 
    if (!inFile) return 1;

    stringstream buffer;
    buffer << inFile.rdbuf();
    string str = buffer.str();

    if (str.empty()) return 0;

    vector<uint8_t> encoded;
    
    buildHuffmanTree(encoded, str);

    saveToFile(argv[2], encoded); 
    
    return 0;
}