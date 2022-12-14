

import random
import argparse
import os
class People(object):
    def __init__(self,group,label):
        self.label = label
        self.group = group
    def __index__(self):
        return self.group
class Bathroom(object):
    def __init__(self,label):
        self.label = label
        self.isClose = -1
        self.peopleList = []
    def __index__(self):
        return self.label

def fildLastPeople(peopleList, groupLabel):
    for id,ob in enumerate(reversed(peopleList)):
        if ob.group == groupLabel:
            return len(peopleList) - id - 1
    return None
def fildPreBathroom(start,BathroomList):
    if(BathroomList[start].isClose != -1):
        raise ValueError("wrong start index!\n")
    init = start
    while True:
        start -=1
        if start == -1:
            start = len(BathroomList)-1
        # if init == start:
        #     break
        if (BathroomList[start].isClose == -1):
            return start
def enter(group,label,peopleList):
    id = fildLastPeople(peopleList,group)
    if id != None:
        peopleList.insert(id+1,People(group,label))
    else:
        peopleList.append(People(group,label))
def go(peopleList):
    if len(peopleList) != 0:
       peopleList.pop(0)
def leave(peopleList):
    if (len(peopleList) != 0):
        return peopleList.pop(-1)
def close(source_peopleList,dest_peopleList):
    while len(source_peopleList) != 0:
        p = leave(source_peopleList)
        enter(p.group,p.label,dest_peopleList)
def generatorSample(numOfBathroom,numOfCommand,numOfGroup,operator,filepath):

    numOfPeopleALL = 0
    group_candidate = [g for g in range(numOfGroup)]
    # 不能 go or leave or close or enter 一間關閉的廁所
    bathroom_candidate = [B for B in range(numOfBathroom)]

    # 不能 go or leave 一間空的廁所
    bathroom_people_record = [0 for _ in range(numOfBathroom)]
    # people的label 是不影響程式的，所以給一個有序的序列沒關係
    peopleLabel_counter = 1
    counter = 0
    with open(filepath,"w") as f:
        print(f"{numOfBathroom} {numOfCommand} {numOfGroup}",file=f)
        while counter < numOfCommand:
            op = random.choice(operator)
            if op[0] =='e':
                g = random.choice(group_candidate)
                label = peopleLabel_counter
                b = random.choice(bathroom_candidate)
                completeOP = f"enter {g} {label} {b}"

                bathroom_people_record[b] +=1
                peopleLabel_counter+=1
                numOfPeopleALL+=1
            elif op[0] =='g':
                b = random.choice(bathroom_candidate)
                # 非法操作
                if (bathroom_people_record[b] == 0):
                    continue
                else:
                    bathroom_people_record[b]-=1
                    numOfPeopleALL -= 1
                    completeOP = f"go {b}"
            elif op[0] =='l':
                b = random.choice(bathroom_candidate)
                # 非法操作
                if (bathroom_people_record[b] == 0):
                    continue
                else:
                    bathroom_people_record[b]-=1
                    numOfPeopleALL -= 1
                    completeOP = f"leave {b}"
            else:
                # 如果只剩一間廁所，且還有人的情況則是非法操作 2.如果剩餘command 的數量 > 1 則會出現已經沒廁所可用，但還需要command的情況
                if ((numOfBathroom == 1 and numOfPeopleALL != 0) or (numOfBathroom == 1 and (numOfCommand-counter >1))):
                    continue
                else:
                    b = random.choice(bathroom_candidate)
                    b_id = bathroom_candidate.index(b)
                    if (len(bathroom_candidate) > 1):
                        if (b_id == 0):
                            pre_b_id = len(bathroom_candidate)-1
                            pre_b = bathroom_candidate[pre_b_id]
                        else:
                            pre_b_id = b_id-1
                            pre_b = bathroom_candidate[pre_b_id]

                        numOfBathroom -= 1
                        # 叭叭叭亂寫一通，close不會減總數好嗎
                        # numOfPeopleALL -= bathroom_people_record[b]
                        bathroom_people_record[pre_b] += bathroom_people_record[b]
                        bathroom_people_record[b] = 0
                        bathroom_candidate.pop(b_id)
                    else:
                        numOfBathroom -= 1
                        # 叭叭叭亂寫一通，close不會減總數好嗎
                        # numOfPeopleALL -= bathroom_people_record[b]
                        bathroom_people_record[b] = 0
                        bathroom_candidate.pop(b_id)

                    completeOP = f"close {b}"
            if counter != numOfCommand-1:
                print(completeOP,file=f)
            else:print(completeOP,end="",file=f)
            counter +=1
def generate_unit_ans(sample_path,ans_path):
    Bathroom_List = []
    openBathroomCounter = 0
    group_candidate = []
    openBathroomList = []
    closeBathroom = []
    with open(f"{sample_path}", "r") as file:
        init = file.readline()
        numOfBathroom, numOfCommand, numOfGroup = map(int, init.split(" "))
        for x in range(numOfBathroom):
            Bathroom_List.append(Bathroom(x))
            openBathroomList.append(x)
        openBathroomCounter = len(openBathroomList)

        for x in range(numOfGroup):
            group_candidate.append(x)
        f = file.readlines()

    num1, num2, num3 = [0, 0, 0]
    for line in f:
        if line[0] == 'e':
            num1, num2, num3 = map(int, line.split(" ")[1:])
            enter(num1, num2, Bathroom_List[num3].peopleList)
        elif line[0] == 'g':
            num1 = list(map(int, line.split(" ")[1:]))[0]
            go(Bathroom_List[num1].peopleList)
        elif line[0] == 'l':
            num1 = list(map(int, line.split(" ")[1:]))[0]
            leave(Bathroom_List[num1].peopleList)
        else:
            num1 = list(map(int, line.split(" ")[1:]))[0]
            PreBathroom_id = fildPreBathroom(num1, Bathroom_List)
            close(Bathroom_List[num1].peopleList, Bathroom_List[PreBathroom_id].peopleList)
            Bathroom_List[num1].isClose = 1


    with open(f'{ans_path}', "w") as ans_f:
        for n in range(numOfBathroom):
            if (Bathroom_List[n].isClose == 1 or len(Bathroom_List[n].peopleList) == 0):
                if (n == numOfBathroom - 1):
                    print(' ', end="", file=ans_f)
                else:
                    print(' ', file=ans_f)
            else:
                peoplelist = Bathroom_List[n].peopleList
                label_list = []
                for m in range(len(peoplelist)):
                    label_list.append(str(peoplelist[m].label))
                ans = " ".join(label_list)
                if (n == numOfBathroom - 1):
                    print(ans, end="", file=ans_f)
                else:
                    print(ans, file=ans_f)
def get_parser():
    parser = argparse.ArgumentParser(description="generate data ans data_ans")
    parser.add_argument("--d",help="generate data number",default=10,type = int)
    parser.add_argument("--c",help="num Of command", default=200, type=int)
    parser.add_argument("--b", help="num Of bathroom",default=15, type=int)
    parser.add_argument("--g", help="num Of group",default=6, type=int)
    # parser.add_argument("--sample", help="",default=6, type=int)
    # parser.add_argument("--ans", help="num Of group",default=6, type=int)
    return parser


def generate_sample(numOfBathroom,numOfCommand,numOfGroup,dataLen,e_prob=32,l_prob=8,g_prob=8,c_prob=2):


    for j in range(1, 10):
        print(f"=====================generating sample {j}=====================")

        # 可以自行調整
        op1 = ["enter" for _ in range(e_prob)]
        op2 = ["leave" for _ in range(l_prob)]
        op3 = ["go" for _ in range(g_prob)]
        op4 = ["close" for _ in range(c_prob)]
        operator = op1 + op2 + op3 + op4

        try:
            os.makedirs(f"./sample/sample{j}")
        except:
            pass
        for i in range(dataLen):
            generatorSample(numOfBathroom, numOfCommand, numOfGroup, operator, f"./sample/sample{j}/sample{i}.txt")
def generate_ans(dataLen):

    for _ in range(1, 10):
        print(f"=====================generating Ans {_}=====================")
        for i in range(dataLen):
            Bathroom_List = []
            group_candidate = []
            openBathroomList = []
            with open(f"./sample/sample{_}/sample{i}.txt","r") as file:
                init = file.readline()
                numOfBathroom,numOfCommand,numOfGroup = map(int,init.split(" "))
                for x in range(numOfBathroom):
                    Bathroom_List.append(Bathroom(x))
                    openBathroomList.append(x)

                for x in range(numOfGroup):
                    group_candidate.append(x)
                f = file.readlines()

            for line in f:
                if line[0] == 'e':
                    num1, num2, num3 = map(int, line.split(" ")[1:])
                    enter(num1,num2,Bathroom_List[num3].peopleList)
                elif line[0] == 'g':
                    num1 = list(map(int, line.split(" ")[1:]))[0]
                    go(Bathroom_List[num1].peopleList)
                elif line[0] == 'l':
                    num1 = list(map(int, line.split(" ")[1:]))[0]
                    leave(Bathroom_List[num1].peopleList)
                else:
                    num1 = list(map(int, line.split(" ")[1:]))[0]
                    PreBathroom_id = fildPreBathroom(num1,Bathroom_List)
                    close(Bathroom_List[num1].peopleList,Bathroom_List[PreBathroom_id].peopleList)
                    Bathroom_List[num1].isClose = 1

            try:
                os.makedirs(f"./Ans/Ans{_}")
            except:
                pass

            with open(f'./Ans/Ans{_}/ans{i}.txt',"w") as ans_f:
                for n in range(numOfBathroom):
                    if(Bathroom_List[n].isClose == 1 or len(Bathroom_List[n].peopleList)==0):
                        if(n == numOfBathroom-1):
                            print('', end="",file=ans_f)
                        else:
                            print('',file=ans_f)
                    else:
                        peoplelist = Bathroom_List[n].peopleList
                        label_list = []
                        for m in range(len(peoplelist)):
                            label_list.append(str(peoplelist[m].label))
                        ans = " ".join(label_list)
                        if(n == numOfBathroom-1):
                            print(ans,end="",file=ans_f)
                        else:
                            print(ans,file=ans_f)

if __name__ == '__main__':
    parser = get_parser()
    args = parser.parse_args()
    numOfBathroom, numOfCommand, numOfGroup,dataLen = [args.b, args.c, args.g,args.d]
    generate_sample(numOfBathroom, numOfCommand, numOfGroup,dataLen)
    generate_ans(dataLen)




# 1.產生合法sample0.txt
# 2.讀取sample.txt並使用創建的函數去生成答案


    # generate_unit_ans("./sample54820.txt","./ans54820.txt")
