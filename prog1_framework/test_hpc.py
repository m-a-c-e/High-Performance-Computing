import copy


def is_valid(arr):
    try:
        idx_minus_1 = arr.index(-1)
    except:
        idx_minus_1 = len(arr)

    if idx_minus_1 == 0:
        return True

    idx = idx_minus_1 - 1 

    
    for i in range(0, idx):
        if arr[i] == arr[idx] or (abs(arr[i] - arr[idx]) == abs(i - idx)):
            print("i = ", i, " arr[i] = ", arr[i], " idx = ", idx)
            return False
    return True


def get_partial_sol_list(size, depth):
    n = size        # size of board (rows)
    k = depth       # k > 1 --> other wise solution is trivial 
                    # k <= n --> solution not possible    

    arr         = [-1] * n
    start_value = 0
    partial_sol = []
    flag        = False
    i           = 0

    while i < k and i >= 0:
        flag = False
        while start_value < n: 
            arr[i] = start_value
            if is_valid(arr):
                if i == k - 1:
                    # a partial solution of size k is created here
                    # send it to any worker here
                    partial_sol.append(copy.deepcopy(arr))
                    start_value += 1
                    continue
                start_value = 0
                flag        = True
                break
            else:
                start_value += 1
        if flag:
            i += 1
            continue
        arr[i]      = -1
        i           = i - 1
        start_value = arr[i] + 1

    return partial_sol

ans = get_partial_sol_list(size=4, depth=2)
#ans = is_valid([1, 2, 3, -1])
print(ans)
print(len(ans))
