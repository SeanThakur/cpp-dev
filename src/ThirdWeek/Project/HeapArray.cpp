#include <iostream>
#include <utility>

class HeapArray {
private: 
	size_t size;
	int *data;
public:
	HeapArray(int value) {
		std::cout << "Constructor:: called\n";
		size = value;
		data = new int[size];
		// default value for data
		for(size_t i = 0; i < size; i++) {
			data[i] = i;
		}
	}

	HeapArray(const HeapArray &source) {
		std::cout << "Copy Constructor:: called\n";
		size = source.size;
		data = new int[size];
		for(size_t i = 0; i < size; i++) {
			data[i] = source.data[i];
		}
	}

	HeapArray &operator=(const HeapArray &source) {
		std::cout << "Copy assignment Constructor:: called\n";
		if (this == &source) {
			return *this;
		}
		if(data != nullptr) {
			delete[] data;
		}

		size = source.size;
		data = new int[size];
		for(size_t i = 0; i < size; i++) {
			data[i] = source.data[i];
		}

		return *this;
	}

	HeapArray &operator=(HeapArray &&source) noexcept {
		std::cout << "Move assignment Constructor:: called\n";
		if(this == &source) {
			return *this;
		}

		if(this->data != nullptr) {
			delete[] data;
		}
		this->size = source.size;
		this->data = source.data;

		source.data = nullptr;
		source.size = 0;
		return *this;
	}

	HeapArray(HeapArray &&source) noexcept {
		std::cout << "Move Constructor:: called\n";
		this->size = source.size;
		this->data = source.data;

		source.data = nullptr;
		source.size = 0;
	}

	~HeapArray() {
		std::cout << "Desctructor:: called\n";
		if(data != nullptr) {
			delete[] data;
		}
	}
};

int main() {
	std::cout << "HeapArray main program \n";
	HeapArray target(100000);
	HeapArray source(100000);
	target = source;
	target = std::move(HeapArray(100000));
	return 0;
}

