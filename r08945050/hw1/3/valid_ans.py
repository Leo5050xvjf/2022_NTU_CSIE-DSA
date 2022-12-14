
#
import filecmp

def cmpAns(numOfData,GT_ansRoot,My_ansRoot):
    for j in range(numOfData):
            GT_path = f"{GT_ansRoot}/ans{j}.txt"
            My_path = f"{My_ansRoot}/ans{j}.txt"
            if not filecmp.cmp(GT_path,My_path):
                print(GT_path )
                exit(0)

# for i in range(1,10):
#     print(f"======================Dir {i}==============================")
#     for j in range(60000):
#         GT_path = f"./GT_Ans/Ans{i}/ans{j}.txt"
#         My_path = f"./My_Ans/Ans{i}/ans{j}.txt"
#         if not filecmp.cmp(GT_path,My_path):
#             print(GT_path )
#             exit(0)

# import os
# for i in range(1,10):
#     os.mkdir(f"C:/Users/User/Desktop/2022_DSA/P5_modify/Ans{i}")

# GT_path = f"./ans54820.txt"
# My_path = f"./Myans54820.txt"
# print(filecmp.cmp(GT_path, My_path))


if __name__ == '__main__':
    # cmpAns(60000,"","")
    pass