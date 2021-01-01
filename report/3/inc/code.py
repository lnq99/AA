def bubbleSort(A):
    n = len(A) - 1
    for i in range(n):
        ni = n-i
        for j in range(ni):
            if A[j] > A[j+1]:
                A[j], A[j+1] = A[j+1], A[j]


def insertionSort(A):
    n = len(A)
    for i in range(1, n):
        for j in range(i, 0, -1):
            if A[j] < A[j-1]:
                A[j], A[j-1] = A[j-1], A[j]
            else:
                break


def mergeSort(A):
    n = len(A)
    B = [None] * n
    k = 1
    while k < n:
        k2 = k * 2
        left_end = k
        while left_end < n:
            left = left_end - k
            right_end = left_end + k
            
            if right_end > n: right_end = n
                
            m = i = left
            j = left_end
            
            while i < left_end and j < right_end:
                if A[i] <= A[j]:
                    B[m] = A[i]
                    i += 1
                else:
                    B[m] = A[j]
                    j += 1
                m += 1
                
            if i < left_end:
                B[m:right_end] = A[i:left_end]
            else:
                B[m:right_end] = A[j:right_end]

            A[left:right_end] = B[left:right_end]

            left_end += k2
            
        k *= 2