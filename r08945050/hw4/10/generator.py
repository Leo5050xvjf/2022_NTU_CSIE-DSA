class DisjointSet:
    def __init__(self):
        self.numOfSet = 0
        self.every_day_status = {}
        self.numOfSetList = {}
    parent = {}

    # stores the depth of trees
    rank = {}

    # perform MakeSet operation
    def makeSet(self, universe):
        # create `n` disjoint sets (one for each item)
        counter = 0
        for i in universe:
            self.parent[i] = i
            self.rank[i] = 0
            counter+=1
        self.numOfSet = counter
    # Find the root of the set in which element `k` belongs
    def Find(self, k):
        # if `k` is not the root
        if self.parent[k] != k:
            # path compression
            self.parent[k] = self.Find(self.parent[k])
        return self.parent[k]

    # Perform Union of two subsets
    def Union(self, a, b):
        # find the root of the sets in which elements `x` and `y` belongs
        x = self.Find(a)
        y = self.Find(b)

        # if `x` and `y` are present in the same set
        if x == y:
            return
        self.numOfSet-=1

        # Always attach a smaller depth tree under the root of the deeper tree.
        if self.rank[x] > self.rank[y]:
            self.parent[y] = x
        elif self.rank[x] < self.rank[y]:
            self.parent[x] = y
        else:
            self.parent[x] = y
            self.rank[y] = self.rank[y] + 1


import random
def sample_generator(numOfCommand, output_path, set_min, set_max):
    ratio = ['q','m','m','m','b','b','b']
    with open(output_path,'w') as f:
        f.write(f"{set_max} {numOfCommand}\n")
        counter = 0
        for i in range(numOfCommand):
            c = random.choice(ratio)
            if(c == 'm'):
                a = random.randint(set_min,set_max)
                b = random.randint(set_min,set_max)
                f.write(f"merge {a} {b}\n")
            elif(c == 'q'):
                f.write(f"query\n")
            else:
                a = random.randint(0,counter)
                # print(a)
                f.write(f"boom {a}\n")
            counter+=1


def init_ds(ds:DisjointSet,N,M,parent,rank):
    for i in range(0,M+1):
        ds.numOfSetList[i] = -1
        ds.every_day_status[i] = -1
    ds.numOfSetList[0] = N
    ds.every_day_status[0] = [parent.copy(),rank.copy()]
def ans_generator(sample_path,output_path):
    with open (output_path,'w') as w_f:
        with open(sample_path, 'r') as f:
            numOfSet,numOfCommand = f.readline().split(' ')
            numOfSet = int(numOfSet)
            numOfCommand = int(numOfCommand)
            universe = [i for i in range(1, int(numOfSet)+1)]
            ds = DisjointSet()
            ds.makeSet(universe)
            init_ds(ds,numOfSet,numOfCommand,ds.parent,ds.rank)

            boom_list = [-1 for _ in range(numOfCommand+1)]
            boom_list[0] = 0
            # print(ds.numOfSetList)
            # print(ds.every_day_status)

            counter = 1
            for line in f:
                if(line[0] == 'q'):
                    boom_list[counter] = -1
                    w_f.write(f"{ds.numOfSet}\n")
                elif(line[0] == 'm'):
                    boom_list[counter] = -2
                    _,a,b = line.split(' ')
                    a= int(a)
                    b= int(b)
                    ds.Union(a, b)
                    ds.every_day_status[counter] = [ds.parent.copy(),ds.rank.copy()]
                    ds.numOfSetList[counter] = ds.numOfSet


                else:

                    _, k = line.split(' ')
                    k = int(k)
                    boom_list[counter] = k

                    while((boom_list[k] != -1 and boom_list[k] != -2) or boom_list[k] ==-1):
                        if(boom_list[k] ==-1):

                            while(boom_list[k] ==-1):
                                k = k-1
                        else:
                            k = boom_list[k]
                        if(k == boom_list[k]):
                            break


                    ds.numOfSet = ds.numOfSetList[k]

                    ds.parent,ds.rank = ds.every_day_status[k][0].copy(),ds.every_day_status[k][1].copy()
                # print(ds.numOfSet)
                counter+=1

if __name__ == '__main__':
    import os
    for _ in range(1):
        try:
            os.makedirs(f"./sample_dir_{_}/ans{_}")
            os.makedirs(f"./sample_dir_{_}/sample{_}")

        except:
            pass
        for i in range(10000):
            sample_generator(500, f"./sample_dir_{_}/sample{_}/sample{i}.txt", 1, 100)
            ans_generator(f"./sample_dir_{_}/sample{_}/sample{i}.txt",f"./sample_dir_{_}/ans{_}/ans{i}.txt")

    # unit test
    # ans_generator(f"./sample2.txt", f"./ans2.txt")





