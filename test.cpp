//Never yield.
#include <bits/stdc++.h>
using namespace std;

#define llint long long
int mod = 1e9+7;
#define cinLoop(n, arr) for(int i = 0; i < n; i++) cin >> arr[i];
#define coutLoop(n, arr)    \
    for(int i=0; i<n; i++)  cout << arr[i] << " ";\
    cout << endl;
    
bool areFilesIdentical(const std::string& p1, const std::string& p2) {
    // Open both files in binary mode to ensure exact byte-for-byte comparison
    std::ifstream f1(p1, std::ifstream::binary | std::ifstream::ate);
    std::ifstream f2(p2, std::ifstream::binary | std::ifstream::ate);

    // 1. Check if files exist
    if (f1.fail() || f2.fail()) {
        std::cerr << "Error: Could not open one of the files." << std::endl;
        return false; 
    }

    // 2. Check file sizes first (Fastest check)
    if (f1.tellg() != f2.tellg()) {
        std::cout << "Mismatch: File sizes are different." << std::endl;
        return false; 
    }

    // Reset file pointers to the beginning
    f1.seekg(0, std::ifstream::beg);
    f2.seekg(0, std::ifstream::beg);

    // 3. Compare content byte-by-byte (using an iterator for cleaner code)
    std::istreambuf_iterator<char> begin1(f1);
    std::istreambuf_iterator<char> begin2(f2);
    std::istreambuf_iterator<char> end;

    // range-based comparison
    if (std::equal(begin1, end, begin2)) {
        return true;
    } else {
        std::cout << "Mismatch: Content bytes do not match." << std::endl;
        return false;
    }
}

int main() {
    std::string inputFile = "input.txt";
    std::string outputFile = "output.txt";

    std::cout << "Comparing " << inputFile << " and " << outputFile << "..." << std::endl;

    if (areFilesIdentical(inputFile, outputFile)) {
        std::cout << "SUCCESS: Files are exactly identical." << std::endl;
    } else {
        std::cout << "FAILURE: Files are NOT identical." << std::endl;
    }

    return 0;
}