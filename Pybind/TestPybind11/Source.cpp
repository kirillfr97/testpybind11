#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include <pybind11/eigen.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <chrono>
#include <string>
#include <vector>

PYBIND11_MAKE_OPAQUE(std::vector<float>);

namespace py = pybind11;

int add(int i, int j) {
	return i + j;
}

double multy(double i, double j)
{
	return i*j;
}

double func_arg(const std::function<int(int)> &f)
{
	return f(5);
}

std::function<int(int, int)> func_ret(const std::function<int(int,int)> &f) {
	return [f](int i, int j) {
		return f(i,j) + add(i,j) + 21;
	};
}

std::string isNumber(std::string s) {
	if (s.size() == 0) return "Empty string";
	for (int i = 0; i < s.size(); i++)
	{
		if ((s[i] >= '0' && s[i] <= '9')) return "Digit";
	}
	return "No digits here";
}

class Pet 
{
public:
	Pet(const std::string &name, int age) : name(name), age(age) { }
	void setName(const std::string &name_) { name = name_; }
	const std::string &getName() const { return name; }
	void setAge(int age_) { age = age_; }
	int getAge() const { return age; }
private:
	std::string name;
	int age;
};

float NPyToVectISum(const std::vector<int> &x) {
	float sum = 0;
	auto begin = std::chrono::steady_clock::now();
	for (int i = 0; i < 1e7; i++)
		for (size_t i = 0; i < x.size(); i++)
			sum += x[i];
	auto end = std::chrono::steady_clock::now();
	auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
	std::cout << "The time for std::vector (not qpaque): " << elapsed_ms.count() << " ms\n";
	std::cout << "Data std::vector (not qpaque): " << x.data() << "\n\n";
	return sum;
}
PYBIND11_MODULE(TestPybind11, m) {
	m.doc() = "pybind11 example plugin"; // optional module docstring

	py::class_<Pet> pet(m, "Pet");
	pet
		.def(py::init<const std::string &, int>())
		.def_property("name", &Pet::getName, &Pet::setName, "Getter and setter on a name")
		.def_property("age", &Pet::getAge, &Pet::setAge, "Getter and setter on an age")
		.def("__repr__",
			[](const Pet &a) { return "<Pet named '" + a.getName() 
			+ "' : age '" + std::to_string(a.getAge()) + "'>"; }
		);

	py::class_<std::vector<float>>(m, "FloatVector")
		.def(py::init<>())
		.def("clear", &std::vector<float>::clear)
		.def("pop_back", &std::vector<float>::pop_back)
		.def("push_back", (void (std::vector<float>::*)(const float &)) &std::vector<float>::push_back)
		.def("print", [](std::vector<float> &v) {
		std::cout << "vector: " << "\n";
		for (size_t i = 0; i < v.size(); i++)
			std::cout << "  " << v[i] << "\n";	})
		.def("__len__", [](const std::vector<float> &v) { return v.size(); })
		.def("__iter__", [](std::vector<float> &v) {
		return py::make_iterator(v.begin(), v.end()); 
	}, py::keep_alive<0, 1>());/* Keep vector alive while iterator is used */
	
	// callbacks
	m.def("func_arg", &func_arg);
	m.def("func_ret", &func_ret);
	
	m.def("array_sum", [](py::array_t<float> x) {
		auto r = x.mutable_unchecked<1>();
		float sum = 0;
		for (ssize_t i = 0; i < r.shape(0); i++)
			sum += r(i);
		return sum;
	}, "A function which returns sum of vector's elements");

	m.def("NPyToArrTSum", [](py::array_t<float> x) {
		auto r = x.mutable_unchecked<1>();
		float sum = 0;
		auto begin = std::chrono::steady_clock::now();
		for (int i = 0; i < 1e7; i++)
			for (ssize_t i = 0; i < r.shape(0); i++)
				sum += r(i);
		auto end = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		std::cout << "The time for array_t type: " << elapsed_ms.count() << " ms\n";
		std::cout << "Data array_t type: " << x.data() << "\n\n";
		return sum;
	});

	m.def("NPyToVectFSum", [](const std::vector<float> &x) {
		float sum = 0;
		auto begin = std::chrono::steady_clock::now();
		for (int i = 0; i < 1e7; i++)
			for (size_t i = 0; i < x.size(); i++)
				sum += x[i];
		auto end = std::chrono::steady_clock::now();
		auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin);
		std::cout << "The time for std::vector (qpaque): " << elapsed_ms.count() << " ms\n";
		std::cout << "Data std::vector (qpaque): " << x.data() << "\n\n";
		return sum;
	});

	m.def("NPyToVectISum", &NPyToVectISum, "A function which returns sums 1e7 times sum of vector's elements");
		
	m.def("add", &add, "A function which adds two numbers");

	m.def("multy", &multy, "A function which multy two numbers");

	m.def("isNumber", &isNumber, "A function which checks whether a string contains digits");

	m.def("norm_vector", [](py::EigenDRef<Eigen::Vector3d> vec) {
		std::cout << "Data EigenDRef: " << vec.data() << "\n\n";
		return vec.norm();
	}, py::return_value_policy::reference_internal);
}

