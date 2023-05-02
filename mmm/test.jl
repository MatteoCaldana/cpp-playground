using LinearAlgebra

function timeit(f, N)
    t = 0
    for i = 1:N 
        t += @elapsed f()
    end
    return t / Float64(N)
end

println(BLAS.get_config())

for e = 0:3
    N = 2^(10 + e)
    A = rand(Float64, N, N)
    B = rand(Float64, N, N)
    t = timeit(()->A*B, 2 ^ (2 * (3 - e)))
    println(N, " ", t)
end

using MKL

println(BLAS.get_config())

for e = 0:3
    N = 2^(10 + e)
    A = rand(Float64, N, N)
    B = rand(Float64, N, N)
    t = timeit(()->A*B, 2 ^ (2 * (3 - e)))
    println(N, " ", t)
end