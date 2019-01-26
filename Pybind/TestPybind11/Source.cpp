#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/functional.h>
#include <string>

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
		return f(i,j) + add(i,j);
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

class Dog : public Pet
{
public:
	Dog(const std::string &name, int age) : Pet(name, age) {}
	std::string call() { return "woof! woof! woof!"; }
};

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
	
	m.def("func_arg", &func_arg);
	m.def("func_ret", &func_ret);

	/*py::calss_<Dog>(m, "Dog", pet)
		.def(py::init<const std::string &, int>())
		.def("call", &Dog::call, "Who let the dog out??");*/

	m.def("array_sum", [](py::array_t<float> x) {
		auto r = x.mutable_unchecked<1>(); // x must have ndim = 3; can be non-writeable
		float sum = 0;
		for (ssize_t i = 0; i < r.shape(0); i++)
			sum += r(i);
		return sum;
	});
	
	m.def("add", &add, "A function which adds two numbers");

	m.def("multy", &multy, "A function which multy two numbers");

	m.def("isNumber", &isNumber, "A function which checks whether a string contains digits");
}

