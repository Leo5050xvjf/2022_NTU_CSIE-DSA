# 2022-NTU-CSIE-DSA
# HW0
## 1.Big Integer problem 
[Ref](https://www.geeksforgeeks.org/bigint-big-integers-in-c-with-example/)

## Description
Build a calculator which can calculate number and number range doesn't limited by build-in type.

# HW1
## 1. Calculate Simple Integers Easily
## Description
Calculate a expression.
* arithmetic operators: +, -, *, /, %, where / means integer division. That is, a/b =  $\lfloor\frac{a}{b}\rfloor$
* parentheses: (, )
- For example, assume that Lisa inputs
  - 1+2=*5+(1+7+11)=%5-11=
  
## 2. Purple Cow
## Description
Use Doubly Linked List and **XOR linked list** to solve problem.
- [XOR Linked List](https://www.youtube.com/watch?v=hMcHVfu3E8U&ab_channel=CppNuts)
## Solution
You have to use some data structures to support the linked list reverse operation in O(1). Futhermore, the traverse order should be follow the problem request.

1. My first solution is use a special pointer(super_link) to achieve the problem request, but the code is not elegant and makes a lots of bugs.That is why I have to write a test data generator to debug.
2. Use XOR Linked List is a good idea.

# HW2
## 1. Teacher Lightning Bear’s Kingdom
## Description
## Solution
- Shortest Path
- DFS

## 2. Everybody Loves Brian
## Description
## Solution
- Min Heap
- Min Heap of Min Heap


# HW3
## 1. Magic Certification
## Description
Give a string, and add the mininum nums of character to make it into a palindrome.
## Solution
1. Find the longest palindrome on the left side and right side.
  Ex: input: "aabc", left side: "aa" -> 2, right side: "c" -> 1
  - To accomplish it, we use the similar method from [KMP](https://en.wikipedia.org/wiki/Knuth%E2%80%93Morris%E2%80%93Pratt_algorithm) string matching algorithm---We build up a KMP table in bidirectional way.
  - Determined longest palindrome on which side, then we reflect the string on that side but doesn't copy that palindrome.
## 2. Magic CertificationII
## Description
Give **k** string with length **l**,if flag == 0, then return whether the set of string exist the similar string or not.(The Def of similar string is: if the two string only have one char is different in each position, and we say it is similar string. ex: "abc" & "abb" is similar, "aaa" & "aaa" is similar.But "abc" & "bac" is not similar.)
## Solution
1. Build a hash table of all string.By similar Def, we will calculate hash value of string which skip pos_i char,where i is: 0< = i < l.
2.The efficient way is calculate the have value by the following property: 
    **part of code**: 
    for (int i = 0; i < l; ++i) {
        hash_val = (((hash_val*base)%q)+dict[str[i]])%q;
    }
    - q: a big prime number to avoid collision.
    - base: in lower case, the base will be 26, because the number of lower case is 26.
    - To avoid overflow, we do a % operation after every * and + operation.
    - We can get the hash value of the string which skip calculate the pos_i char in constant time only if we have get the hash value of string(without skip any char).
3. Use sort algorithm to sort the hash value table.
4. Build up a record table to check visited string.
5. Use Binary Search to find the same hash value string.For example, first, you get a unvisited string's hash value H1.Second, you heve to check the same value in the    Hash value table(which is 2D array), so we have to search every row to find out the H1.



# HW4
## 1.Purple Cow Revenge
## Description
1. [Disjoint Set](https://en.wikipedia.org/wiki/Disjoint-set_data_structure)
2. [Offline algorithm](https://en.wikipedia.org/wiki/Online_and_offline)
## Solution
1. The key idea is that the data is **offline**, which means if you want to back to certain day, you have to record some information from it.

## 1.COOL Server
## Description
1. You have an array, and you have to accomplish some operation.See the detail explanation in HW4.pdf.
## Solution
1. 吉如一segment tree
2. treap = min heap + binary tree
3. lazy tag
[Ref](https://github.com/ShahjalalShohag/code-library/blob/master/Data%20Structures/Implicit%20Treap.cpp)
Note: The code I upload have some bug.






