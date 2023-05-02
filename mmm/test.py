import numpy as np
import time

np.show_config()

for N in 2**np.array([10, 11, 12, 13]):
    A = np.random.rand(N, N)
    B = np.random.rand(N, N)
    t0 = time.time()
    C = A @ B
    t1 = time.time()
    print(N, t1 - t0)


import torch as th 

print("Torch: ", th.__version__)
for N in 2**np.array([10, 11, 12, 13]):
    A = th.rand(N, N, dtype=th.float64)
    B = th.rand(N, N, dtype=th.float64)
    print(A.dtype)
    t0 = time.time()
    C = A @ B
    t1 = time.time()
    print(N, t1 - t0)