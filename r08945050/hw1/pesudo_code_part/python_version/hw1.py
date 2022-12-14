




# p3-2
def findMissingPositive(arr, size):
    # Mark arr[i] as visited by
    # making arr[arr[i] - 1] negative.
    # Note that 1 is subtracted
    # because index start
    # from 0 and positive numbers start from 1
    for i in range(size):
        if (abs(arr[i]) - 1 < size and arr[abs(arr[i]) - 1] > 0):
            arr[abs(arr[i]) - 1] = -arr[abs(arr[i]) - 1]

    # Return the first index value at which is positive
    for i in range(size):
        if (arr[i] > 0):
            # 1 is added because indexes start from 0
            return i + 1
    return size + 1


# p3-3
def seesaw(arr)->int:
    length = len(arr)
    left_torque = 0
    # O(N)
    right_total_weight = sum(arr)
    left_total_weight = 0
    right_torque = 0
    # O(N)
    for i in range(length):
        right_torque += arr[i]*i
    # O(N)
    for i in range(length):
        if (i == 0):
            right_torque = right_torque
            left_torque = 0
            if (right_torque == left_torque):
                return 0
        else:
            left_total_weight+=arr[i-1]
            left_torque+=left_total_weight
            right_total_weight-= arr[i-1]
            right_torque = right_torque - (right_total_weight)
            right_torque = right_torque

        if (left_torque == right_torque):
            return i
    return -1
# ans = seesaw([9,5,2,2,1])
# ans = seesaw([1,2,3,4,3,2,1])
# ans = seesaw([1,1,2,1])
# print(ans)


