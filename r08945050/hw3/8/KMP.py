
def shortestPalindrome(s):
    temp = s + "#" + s[::-1]
    return temp
def getTable(s):
    table = [0 for _ in range(len(s))]
    index = 0

    for i in range(1,len(s)):
        if(s[index] == s[i]):
            index+=1
            table[i] = table[i-1]+1
        else:
            index = table[i - 1]
            while(index > 0 and s[index] != s[i]):
                index = table[index-1]

            if(s[index] == s[i]):
                index+=1
            table[i] = index
    return table
s = "abccc"
s1 = "catacb"
s2 = "abcaa"
s4 = "aacbaaacc"
s5 = "efcaabxxxefcaac"
temp_s = shortestPalindrome(s5)
print(temp_s)
table = getTable(s5)
print(table)
