# 🧮 Data Structure Lab Experiments

Welcome to our Data Structure course lab repository! This is where algorithms come to life through practical implementations. 🚀

## 📚 What's Inside?

This repository contains exciting experiments from our Data Structure course, showcasing the beauty of classic data structures and algorithms in action.

### 🔬 Experiment 1: Polynomial Calculator

**The Challenge**: Build a smart polynomial calculator that can handle all your mathematical needs!

**What it does**:
- ✨ Parse polynomial expressions like `3x^4 - 5x^2 + 7` with ease
- ➕ Add and subtract polynomials effortlessly  
- 📊 Evaluate polynomials at any given point
- 🎨 Beautiful Qt-based graphical interface for smooth user interaction

**The Tech Magic**:
- **Data Structure**: Sparse polynomial representation using linked lists (降幂单链表)
- **Core Algorithm**: Efficient term merging and fast exponentiation (O(log n) power calculation!)
- **Language**: C++17 with Qt5/Qt6 Widgets
- **Features**: 
  - Smart input parsing (handles brackets, negative exponents, implicit coefficients)
  - Robust error handling (no crashes, just helpful messages!)
  - Coefficient sorting and automatic term combination

**Key Files**:
- `polynomial.h/cpp` - The brain of our calculator (linked list operations, parsing, arithmetic)
- `mainwindow.h/cpp` - The friendly GUI that brings it all together
- `main.cpp` - Application entry point

### 🎯 Experiment 2: [To Be Added]

Stay tuned for more exciting data structure adventures! 🌟

## 🛠️ Getting Started

### Prerequisites
- Qt 5.15.2+ or Qt 6.x
- CMake 3.16+
- C++17 compatible compiler

### Build & Run

```bash
# Clone the repository
git clone https://github.com/submergepsc/project1_DS.git
cd project1_DS

# Build with CMake
cmake -B build -S .
cmake --build build

# Run the application
./build/SparsePolynomialCalculator
```

Or simply open `CMakeLists.txt` in Qt Creator and hit the run button! 🎯

## 🎮 How to Use

1. **Enter Your Polynomial**: Type expressions like `2x^3 - 4x + 1` into the input field
2. **Build It**: Click "Build A/B" to create your polynomial
3. **Perform Operations**: 
   - Add or subtract polynomials
   - Evaluate at specific x values
   - View results in both expression and sequence format

## 🌟 Highlights

- **Zero-tolerance for bugs**: Comprehensive error handling ensures smooth sailing
- **Smart parsing**: Understands various input formats (with/without spaces, brackets, signs)
- **Efficient storage**: Only stores non-zero terms (sparse representation saves memory!)
- **Fast computation**: Binary exponentiation makes evaluation lightning quick ⚡

## 📖 Learning Outcomes

Through these experiments, you'll master:
- Linked list manipulation and traversal
- String parsing and lexical analysis
- Algorithm design and complexity analysis
- GUI programming with Qt framework
- Object-oriented design patterns

## 🤝 Contributing

This is a course project, but suggestions and improvements are always welcome! Feel free to open issues or submit pull requests.

## 📝 License

Educational project for Data Structure course.

---

*Built with ❤️ and lots of ☕ by Data Structure enthusiasts!*
