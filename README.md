# Huffman Compression & Decompression

A full-stack web application that implements **Huffman Coding** for lossless data compression and decompression. The project features a modern React frontend with a high-performance C++ backend engine for efficient file processing.

## 🚀 Live Demo

- **Frontend**: Hosted on Netlify
- **Backend**: Deployed on Render

## 📋 Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Compression Approach](#compression-approach)
- [Decompression Approach](#decompression-approach)
- [Technical Stack](#technical-stack)
- [File Format](#file-format)
- [Installation & Setup](#installation--setup)
- [Usage](#usage)
- [Project Structure](#project-structure)

## 🎯 Overview

This project implements the classic **Huffman Coding** algorithm for data compression. Huffman Coding is a lossless compression technique that assigns variable-length codes to characters based on their frequency of occurrence - more frequent characters get shorter codes, resulting in overall size reduction.

## ✨ Features

- **Lossless Compression**: Original data is perfectly reconstructed after decompression
- **Variable-Length Encoding**: Efficient bit-level encoding using Huffman trees
- **Web Interface**: User-friendly React frontend with drag-and-drop support
- **High Performance**: C++ backend for fast compression/decompression
- **Real-time Statistics**: View compression ratios and file size comparisons
- **Cross-Platform**: Works on any modern browser

## 🏗 Architecture

```
┌─────────────┐          ┌──────────────┐          ┌─────────────┐
│   React     │  HTTP    │    Flask     │  Spawns  │     C++     │
│  Frontend   │ ────────>│   Backend    │ ────────>│   Engine    │
│  (Netlify)  │          │  (Render)    │          │  (Binary)   │
└─────────────┘          └──────────────┘          └─────────────┘
```

- **Frontend**: React + TypeScript + Vite + TailwindCSS + shadcn/ui
- **Backend**: Flask (Python) serving as API layer
- **Engine**: C++ compiled binaries for compression/decompression logic

## 🔧 Compression Approach

### Algorithm Overview

The compression process follows these key steps:

### 1. **Frequency Analysis & Priority Assignment**

To understand the deterministic nature of this engine, let's walk through the process with an example string: **"abdacadabda"** (Length: 11).

**Step 1: Frequency Count**
First, the code counts character occurrences:
- `a`: 5
- `b`: 2
- `d`: 3
- `c`: 1

**Step 2: Sorting & Initialization**
The code creates a `vector<char>` alphabet and sorts it alphabetically: `['a', 'b', 'c', 'd']`.
It then creates Leaf Nodes and assigns them a **Priority Index** based on this sorted order.

| Character | Frequency | Priority Index | Height |
|-----------|-----------|----------------|--------|
| 'a'       | 5         | 0              | 0      |
| 'b'       | 2         | 1              | 0      |
| 'c'       | 1         | 2              | 0      |
| 'd'       | 3         | 3              | 0      |

**Step 3: The Priority Queue (Min-Heap)**
The `Comp` struct defines the sorting rules for the queue. This is the most critical part of the logic.

**The Hierarchy of "Who is Smaller?" (Who floats to the top?):**
1. **Lowest Frequency** (Standard Huffman rule).
2. **Lowest Height** (Tie-breaker 1: Prefer shallower sub-trees).
3. **Lowest Priority Index** (Tie-breaker 2: Prefer characters that appeared earlier in the alphabet).
4. **Lowest ASCII Value** (Final tie-breaker).

**Initial Queue State** (Sorted by rules above):
1. `c` (Freq: 1, H: 0, Pri: 2) **[TOP]**
2. `b` (Freq: 2, H: 0, Pri: 1)
3. `d` (Freq: 3, H: 0, Pri: 3)
4. `a` (Freq: 5, H: 0, Pri: 0)

### 2. **Building the Tree (Iteration by Iteration)**

The loop runs until only one node remains.

**Iteration 1**
- **Pop A (Top)**: Node `c` (Freq 1)
- **Pop B (Next)**: Node `b` (Freq 2)
- **Create Parent (P1)**:
  - Frequency: $1 + 2 = 3$
  - Height: $\max(0, 0) + 1 = 1$
  - Priority: $\min(\text{Pri}_c, \text{Pri}_b) = \min(2, 1) = 1$
- **Left/Right Logic**:
  - The code checks: `if (a->priority < b->priority)`
  - `c` (Pri 2) < `b` (Pri 1) is **FALSE**.
  - **Result**: `b` becomes **Left**, `c` becomes **Right**.
  - *(Note: This keeps the node with the 'earlier' alphabet index on the left).*
- **Push P1**: `P1` (Freq 3, H 1, Pri 1) enters the queue.

**Iteration 2: The Conflict (Freq 3 vs Freq 3)**
The queue now contains:
1. `P1` (Freq: 3, Height: 1, Pri: 1)
2. `d` (Freq: 3, Height: 0, Pri: 3)
3. `a` (Freq: 5, Height: 0, Pri: 0)

**CONFLICT RESOLUTION**:
Both `P1` and `d` have Frequency 3.
The comparator `Comp` checks: `if(l->height != r->height) return l->height > r->height;`
This logic favors **Lower Height**.
- `d` has Height 0.
- `P1` has Height 1.
- **Winner**: `d` is considered "smaller/better" and moves to the top.

**Queue Order before processing**:
1. `d` (Freq 3) **[TOP]**
2. `P1` (Freq 3)
3. `a` (Freq 5)

**Processing**:
- **Pop A**: Node `d` (Freq 3)
- **Pop B**: Node `P1` (Freq 3)
- **Create Parent (P2)**:
  - Frequency: $3 + 3 = 6$
  - Height: $\max(0, 1) + 1 = 2$
  - Priority: $\min(3, 1) = 1$
- **Left/Right Logic**:
  - `d` (Pri 3) < `P1` (Pri 1) is **FALSE**.
  - **Result**: `P1` becomes **Left**, `d` becomes **Right**.

**Iteration 3 (Final Merge)**
The queue now contains:
1. `a` (Freq: 5, H: 0, Pri: 0)
2. `P2` (Freq: 6, H: 2, Pri: 1)

**Processing**:
- **Pop A**: Node `a` (Freq 5)
- **Pop B**: Node `P2` (Freq 6)
- **Create Root**:
  - Frequency: $5 + 6 = 11$
  - Height: $\max(0, 2) + 1 = 3$
  - Priority: 0
- **Left/Right Logic**:
  - `a` (Pri 0) < `P2` (Pri 1) is **TRUE**.
  - **Result**: `a` becomes **Left**, `P2` becomes **Right**.

### 3. **Final Tree Structure & Conflict Logic Summary**

**Tree Hierarchy**:
```
       [ROOT (11)]
       /        \
     'a'      [P2 (6)]
     (0)      /      \
          [P1 (3)]    'd'
          /      \    (1)
        'b'      'c'
        (0)      (1)
```

**Bit Assignment** (Left = 0, Right = 1):
- `a`: Left → **0** (Length: 1 bit)
- `b`: Right → Left → Left → **100** (Length: 3 bits)
- `c`: Right → Left → Right → **101** (Length: 3 bits)
- `d`: Right → Right → **11** (Length: 2 bits)

**Why This Logic Matters?**
The specific lines in the code that handle the "Freq 3 vs Freq 3" conflict are:

```cpp
// Inside Comp struct
if(l->height != r->height) return l->height > r->height;
if(l->priority != r->priority) return l->priority > r->priority;
```

1.  **Why Height?** By picking the shorter node (`d`) over the taller node (`P1`) when frequencies match, we prevent the tree from becoming too deep (unbalanced). This optimizes the worst-case bit length.
2.  **Why Priority?** By using the priority index (derived from alphabetical order), we ensure that if everything else is equal, `a` will always be processed before `b`. This makes the compression **Deterministic**—running the program 100 times on the same file will produce the exact same binary output every time.

### 4. **Code Generation via Tree Traversal**
```cpp
void traverse(Node* node, uint32_t code_bits, int code_length, 
              unordered_map<char, pair<uint32_t, int>> &rep) {
    if(!node) return;
    if((!node->left) && (!node->right)) {
        rep[node->val] = make_pair(code_bits, code_length);
        return;
    }
    traverse(node->left, (code_bits<<1), code_length+1, rep);
    traverse(node->right, ((code_bits<<1)|1), code_length+1, rep);
}
```
- Traverse tree to generate Huffman codes
- Left child = append '0' bit (`code_bits << 1`)
- Right child = append '1' bit (`(code_bits << 1) | 1`)
- Leaf nodes store the final character codes

### 5. **Binary Encoding & File Structure**

The compressed file format:
```
[1 byte]  - Number of unique characters (alphabet size)
[n pairs] - Character-Length pairs:
            * [1 byte] ASCII value of character
            * [1 byte] Code length for this character
[4 bytes] - Original text length (uint32_t)
[remaining] - Bit-packed encoded data
```

**Bit-Level Packing**:
```cpp
for(char ch: text) {
    uint32_t bin = rep[ch].first;
    int len = rep[ch].second;
    
    while(len) {
        int fill_length = min(len, 8-pos);
        curr = ((curr << fill_length) | (bin >> (len - fill_length)));
        bin = (bin ^ ((bin >> (len-fill_length)) << (len-fill_length)));
        len -= fill_length;
        pos += fill_length;
        
        if(pos == 8) {
            encoded.push_back(curr);
            curr = 0;
            pos = 0;
        }
    }
}
```
- Pack variable-length codes into bytes efficiently
- No byte boundaries wasted
- Maximum space utilization

## 🔓 Decompression Approach

Here is a comprehensive step-by-step explanation of the **Decompression Process** using the exact same example: **"abdacadabda"**.

We will trace how your `decompressor.cpp` takes the raw binary file and turns it back into text, specifically focusing on the mathematical logic used to rebuild the tree.

---

### Phase 1: The Input File (What the Decompressor Sees)

The decompressor does not see "letters"; it sees **bytes**. Based on our compression of "abdacadabda", the file content looks roughly like this in hex/binary:

**File Content:**

1. **Header:**
   * `Alphabet Size`: `4`
   * `Dictionary`: `('a', 1)`, `('b', 3)`, `('c', 3)`, `('d', 2)` (Stored as bytes)
   * `Text Length`: `11`

2. **Body (Compressed Bits):**
   * The compressed bitstream was: `0100110101011100110...`
   * Packed into bytes: `[01001101]` `[01011100]` `[11000000]`

---

### Phase 2: Reconstructing the Codes (The "Canonical" Logic)

This is the most complex part of your code. The file contains **Lengths**, not Codes. The decompressor must mathematically guess the codes.

**The Loop Logic:**

* Start `curr_code = 0`.
* Iterate through the dictionary.
* Update `curr_code` based on the change in length (`len` vs `prev_len`).

**Trace:**

| Step | Char | Length | Math Logic (`curr_code` operations) | Binary Code |
| --- | --- | --- | --- | --- |
| **1** | **'a'** | 1 | `first` is true. Start at 0. | **0** |
| **2** | **'b'** | 3 | 1. Increment `curr_code`: <br><br>2. `len` (3) > `prev` (1).<br><br>3. Shift Left: `1 << (3-1)`  `1 << 2` = **4** | **100** |
| **3** | **'c'** | 3 | 1. Increment `curr_code`: <br><br>2. `len` (3) == `prev` (3). No Shift. | **101** |
| **4** | **'d'** | 2 | 1. Increment `curr_code`:  (`110`)<br><br>2. `len` (2) < `prev` (3).<br><br>3. Shift Right: `6 >> (3-2)`  `6 >> 1` = **3** | **11** |

**Result:**
The decompressor has successfully recovered the exact codes used by the compressor:

* **a:** `0`
* **b:** `100`
* **c:** `101`
* **d:** `11`

---

### Phase 3: Building the Tree in Memory

Now, the function `insertPath` runs for each of these codes to build the tree in RAM.

1. **Insert 'a' (Code `0`)**
   * Start at Root.
   * Bit `0`: Go Left.
   * **Action:** Create Node. Mark it as 'a'.

2. **Insert 'b' (Code `100`)**
   * Start at Root.
   * Bit `1`: Go Right.
   * Bit `0`: Go Left.
   * Bit `0`: Go Left.
   * **Action:** Create Node. Mark it as 'b'.

3. **Insert 'c' (Code `101`)**
   * Start at Root.
   * Bit `1`: Go Right.
   * Bit `0`: Go Left.
   * Bit `1`: Go Right.
   * **Action:** Create Node. Mark it as 'c'.

4. **Insert 'd' (Code `11`)**
   * Start at Root.
   * Bit `1`: Go Right.
   * Bit `1`: Go Right.
   * **Action:** Create Node. Mark it as 'd'.

**Visualizing the Reconstructed Tree:**

```text
       [ROOT]
       /    \
     'a'   (Right)
            /   \
        (Left)  'd'
        /    \
      'b'    'c'
```

---

### Phase 4: The Decoding Loop (Reading the Stream)

Now the decompressor reads the **Body Bytes** one by one and walks the tree.

**Byte 1:** `01001101`

| Current Bit | Action | Current Node State | Output |
| --- | --- | --- | --- |
| **0** | Go Left | Leaf Found ('a')! Reset to Root. | **"a"** |
| **1** | Go Right | Internal Node. |  |
| **0** | Go Left | Internal Node. |  |
| **0** | Go Left | Leaf Found ('b')! Reset to Root. | **"b"** |
| **1** | Go Right | Internal Node. |  |
| **1** | Go Right | Leaf Found ('d')! Reset to Root. | **"d"** |
| **0** | Go Left | Leaf Found ('a')! Reset to Root. | **"a"** |
| **1** | Go Right | Internal Node. |  |

**Byte 2:** `01011100` (Continuing from previous state)

| Current Bit | Action | Current Node State | Output |
| --- | --- | --- | --- |
| **0** | Go Left | Internal Node. |  |
| **1** | Go Right | Leaf Found ('c')! Reset to Root. | **"c"** |
| **0** | Go Left | Leaf Found ('a')! Reset to Root. | **"a"** |
| **1** | Go Right | Internal Node. |  |
| **1** | Go Right | Leaf Found ('d')! Reset to Root. | **"d"** |
| **1** | Go Right | Internal Node. |  |
| **0** | Go Left | Internal Node. |  |
| **0** | Go Left | Leaf Found ('b')! Reset to Root. | **"b"** |

*(...and so on until `textLength` of 11 is reached)*

### Summary of the Logic

1. **Header Parsing:** Extracts lengths to understand the tree structure.
2. **Canonical Math:** `curr_code` shifts Left `<<` when codes get longer (deeper in tree) and Right `>>` when they get shorter (shallower).
3. **Tree Construction:** Physically allocates nodes in memory to create a lookup path.
4. **Stream Traversal:** Simply follows the bits (Left/Right) until it hits a leaf, then prints and resets.

## 🛠 Technical Stack

### Frontend
- **React 18** with TypeScript
- **Vite** for fast development and building
- **TailwindCSS** for styling
- **shadcn/ui** for UI components
- **Lucide React** for icons

### Backend
- **Flask** (Python) for REST API
- **Flask-CORS** for cross-origin requests
- **C++ (g++)** for compression engine
- Compiled binaries: `compressor` and `decompressor`

### Deployment
- **Frontend**: Netlify (static hosting)
- **Backend**: Render (containerized deployment)

## 📦 File Format

### Compressed File (.bin)
```
Byte 0:        Alphabet size (n)
Bytes 1-2n:    n pairs of (character, code_length)
Bytes 2n+1 to 2n+4: Original text length (4 bytes, big-endian)
Remaining:     Bit-packed encoded data
```

### Why This Format?
- **Minimal overhead**: Only stores alphabet and lengths
- **Canonical codes**: Tree can be reconstructed deterministically
- **Exact restoration**: Text length ensures no padding bits decoded
- **Space efficient**: No tree structure stored explicitly

## 🚀 Installation & Setup

### Prerequisites
- Node.js 18+
- Python 3.8+
- g++ compiler (for C++ backend)

### Backend Setup
```bash
cd backend

# Compile C++ programs
bash build.sh
# Or manually:
g++ compressor.cpp -o compressor -std=c++17
g++ decompressor.cpp -o decompressor -std=c++17

# Install Python dependencies
pip install -r requirements.txt

# Run Flask server
python app.py
```

### Frontend Setup
```bash
cd frontend

# Install dependencies
npm install

# Run development server
npm run dev

# Build for production
npm run build
```

## 💡 Usage

### Web Interface
1. Navigate to the web application
2. **Compress**: Upload a text file → Download `.bin` compressed file
3. **Decompress**: Upload a `.bin` file → Download restored text file

### Command Line (Direct C++ Usage)
```bash
# Compress
./compressor input.txt output.bin

# Decompress
./decompressor output.bin restored.txt
```

## 📂 Project Structure

```
.
├── backend/
│   ├── app.py              # Flask API server
│   ├── compressor.cpp      # Huffman compression engine
│   ├── decompressor.cpp    # Huffman decompression engine
│   ├── build.sh            # Build script for C++
│   └── requirements.txt    # Python dependencies
│
├── frontend/
│   ├── src/
│   │   ├── App.tsx         # Main React component
│   │   ├── pages/
│   │   │   └── Index.tsx   # Main page with tabs
│   │   └── components/
│   │       ├── CompressionPanel.tsx
│   │       └── DecompressionPanel.tsx
│   ├── package.json
│   └── vite.config.ts
│
└── README.md              # This file
```

## 🎓 Key Learning Points

### Algorithm Design
- **Priority queue** optimization with custom comparators
- **Canonical Huffman codes** for space-efficient tree storage
- **Bit-level manipulation** for maximum compression

### Engineering Practices
- **Separation of concerns**: React UI + Flask API + C++ engine
- **Cross-platform compatibility**: Binary handling across OS
- **Error handling**: Graceful failures and edge cases
- **Modern web stack**: TypeScript, Vite, TailwindCSS

### Performance Considerations
- **C++ backend**: Fast processing for large files
- **Streaming**: Byte-by-byte processing (low memory footprint)
- **Efficient encoding**: Bit-packing without byte boundaries

## 📈 Compression Efficiency

Typical compression ratios depend on:
- **Text redundancy**: Repetitive text compresses better
- **Alphabet size**: Fewer unique characters → shorter codes
- **Character distribution**: Skewed frequencies → better compression

Example: English text typically achieves **40-60% size reduction**.

## 🔮 Future Enhancements

- [ ] Support for binary files (images, executables)
- [ ] Adaptive Huffman coding for streaming data
- [ ] Multi-file batch compression
- [ ] Progressive compression for large files
- [ ] Compression statistics visualization

## 📝 License

This project is open-source and available for educational purposes.

## 👨‍💻 Author

Built with ❤️ using Huffman Coding by Krishna Satyam

---

**Note**: This implementation prioritizes **correctness** and **educational value** while maintaining production-ready performance through C++ optimization.
