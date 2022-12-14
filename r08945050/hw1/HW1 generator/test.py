



import random

# for j in range(1,9):
#     err_counter = 0
#     for i in range(0,60000):
#         with open(f"./sample{j}/sample{i}.txt","r") as file:
#             f = file.readlines()
#             e_counter = 0
#             c_counter = 0
#             counter = 0
#
#             for line in f:
#                 if line[0] == 'e':
#                     e_counter+=1
#                 elif line[0] == 'c':
#                     c_counter+=1
#                 elif line[0] == 'g' or line[0] == 'l':
#                     counter+=1
#             if c_counter == 15:
#                 print(f"sample {i}")
#                 if e_counter-counter > 0:
#                     err_counter+=1
#     print(f"=====sample {j}=====")
#     print(f"err_counter is {err_counter}")


err_counter = 0
with open(f"./sample54819.txt", "r") as file:
    f = file.readlines()
    e_counter = 0
    c_counter = 0
    counter = 0

    for line in f:
        if line[0] == 'e':
            e_counter += 1
        elif line[0] == 'c':
            c_counter += 1
        elif line[0] == 'g' or line[0] == 'l':
            counter += 1
    if c_counter == 15:
        if e_counter - counter > 0:
            err_counter += 1
print(e_counter)
print(c_counter)
print(counter)
print(f"err_counter is {err_counter}")