
class Node():
    def __init__(self,val):
        self.val = val
        self.right = None
        self.left = None

class Tree():
    def __init__(self):
        self.root = None
    def add(self,root,val):
        if (root == None):
            self.root = Node(val)
        elif(val > root.val):
            if (root.right == None):
                root.right = Node(val)
            else:
                self.add(root.right,val)
        else:
            if (root.left == None):
                root.left = Node(val)
            else:
                self.add(root.left,val)
    # 左,root,右
    def InOrder(self,root):
        if(root == None):
            return
        if(root.left!= None):
            self.InOrder(root.left)
        print(root.val,end=" ")
        if(root.right != None):
            self.InOrder(root.right)
    # 左,右,root
    def PostOrder(self,root):
        if(root == None):
            return
        if(root.left != None):
            self.PostOrder(root.left)
        if(root.right != None):
            self.PostOrder(root.right)
        print(root.val,end=" ")
    # root,左,右
    def PreOrder(self,root):
        if (root == None):
            return
        print(root.val, end=" ")
        if (root.left != None):
            self.PreOrder(root.left)
        if (root.right != None):
            self.PreOrder(root.right)

    def accumulation(self,root,sum):
        if(root == None):
            return 0
        if(root.right != None):
            sum =self.accumulation(root.right,sum)
        root.val += sum
        sum = root.val
        if(root.left != None):
            sum = self.accumulation(root.left,sum)
        return sum


if __name__ == '__main__':
    # T = Tree()
    # order_list = [3,2,5,1,4,6,8]
    # for val in order_list:
    #     T.add(T.root,val)
    # T.InOrder(T.root)
    # print("\n")
    # T.PostOrder(T.root)
    # print("\n")
    # T.PreOrder(T.root)
    # print(T.root.val)
    # T.InOrder(T.root)
    # print("\n")
    # total = T.accumulation(T.root,0)
    # T.InOrder(T.root)
    # -------------------------------------------------------------
    T = Tree()
    order_list = [5,2,1,4,3]
    for val in order_list:
        T.add(T.root,val)
    T.InOrder(T.root)


