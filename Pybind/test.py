import TestPybind11
import numpy as np
import ctypes

# print(TestPybind11.add(1,2))
# print(TestPybind11.multy(2.34,4.5453))
# print(TestPybind11.isNumber("fds21d"))
# p = TestPybind11.Pet('Max', 6)
# print(p)
# print(p.name)
# print(p.age)
# p.name = 'Molly'
# p.age = 8
# print(p)

a = np.array([1, 2, 3], float)
print("<float> a:", memoryview(a))
# b = np.array([1, 2, 3], int)
# print("<int> b:", memoryview(b))
# aa = TestPybind11.FloatVector()
# aa.push_back(1)
# aa.push_back(2)
# aa.push_back(3)
# print(aa)
# print(TestPybind11.array_sum(a))
#
# def square(i, j):
#     return i * j
#
# s = TestPybind11.func_ret(square)
# print(s(3,5))

# print("NPyToArrTSum(a): ", TestPybind11.NPyToArrTSum(a))
# print("NPyToVectISum(b): ", TestPybind11.NPyToVectISum(b))
# print("NPyToVectFSum(aa): ", TestPybind11.NPyToVectFSum(aa))

print("Eigen: ", TestPybind11.norm_vector(a))