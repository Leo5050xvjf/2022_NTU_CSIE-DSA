
# 2022-NTU-CSIE-DSA

## HW0: Big Integer Problem
[Reference](https://www.geeksforgeeks.org/bigint-big-integers-in-c-with-example/)

### Description
Develop a calculator that can compute numbers beyond the limitations of built-in data types.

## HW1: Simple Integer Calculations

### Description
Implement a program to calculate expressions. It should include:
- Arithmetic operators: +, -, *, /, % (Note: '/' denotes integer division, i.e., a/b =  $\lfloor\frac{a}{b}\rfloor$)
- Parentheses: (, )
- Example: For an input like `1+2=*5+(1+7+11)=%5-11=`, the program should output the correct result.

## HW2: Purple Cow

### Description
Utilize Doubly Linked List and XOR linked list to address the given problem.
- [XOR Linked List](https://www.youtube.com/watch?v=hMcHVfu3E8U&ab_channel=CppNuts)

### Solution
Implement data structures to support reverse operation of the linked list in O(1) time complexity. Additionally, ensure that traversal order adheres to the problem requirements.
- Initial approach: Use a special pointer (super_link) to meet the problem's demands, ensuring efficient and elegant code.

## HW3: Magic Certification

### Description
Give a string and add the minimum number of characters to convert it into a palindrome.

### Solution
1. Find the longest palindrome on the left and right sides.
   Example: Input: "aabc", Left: "aa" -> 2, Right: "c" -> 1
   - Use a method similar to the KMP string matching algorithm. Build a KMP table bidirectionally.
   - Determine the longest palindrome side, then reflect the string on that side without duplicating the palindrome.

## HW4: Purple Cow Revenge and COOL Server

### Description
1. [Disjoint Set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure)
2. [Offline algorithm](https://en.wikipedia.org/wiki/Online_and_offline)
3. Array operations and challenges (Detailed in HW4.pdf).

### Solution
1. Record data offline to backtrack to a specific day.
2. Implement a segment tree and treap (min heap + binary tree).
3. Apply a lazy tag for efficient updates.(吉如一線段樹)
   - Reference: [Implicit Treap Implementation](https://github.com/ShahjalalShohag/code-library/blob/master/Data%20Structures/Implicit%20Treap.cpp)
   - Note: The uploaded code contains some bugs.
