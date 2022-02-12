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
                start_value = 0
                flag        = True
                if i == k - 1:
                    partial_sol.append(copy.deepcopy(arr))
                    flag = False
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

ans = get_partial_sol_list(size=8, depth=4)
print(ans)
print(len(ans))
