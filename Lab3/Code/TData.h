#pragma once

struct TData {
	int key;
	double value;

	TData() : key(0), value(0) {}

	TData(TData& data) {
		this->key = data.key;
		this->value = data.value;
	}

	TData(int _key, double _value) : key(_key), value(_value) {}

	TData& operator = (TData& obj) {
		this->key = obj.key;
		this->value = obj.value;

		return *this;
	}
};