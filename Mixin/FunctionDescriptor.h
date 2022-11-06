#pragma once

#include <vector>
#include <typeindex>
#include <string>

class FunctionDescriptor {
public:
	class Unknown {};

	using UnknownFunctionRef = Unknown(&)(Unknown);
	using UnknownFunctionPtr = Unknown(*)(Unknown);

	std::string getReturnTypeName();
	std::string getArgumentTypeNames();

	template <class Return, class... Arguments>
	FunctionDescriptor(Return(&function)(Arguments...));
private:
	static const std::type_index unknownType;
	const std::type_index returnType;
	const std::vector<std::type_index> argumentTypes;

	template <class First, class... Others>
	std::vector<std::type_index> getDescriptors();

	template <>
	std::vector<std::type_index> getDescriptors<void>();
};

template <class Return, class... Arguments>
inline FunctionDescriptor::FunctionDescriptor(Return(&function)(Arguments...)) :
	returnType(typeid(Return)),
	argumentTypes(getDescriptors<Arguments..., void>())
{}

template <class First, class... Others>
inline std::vector<std::type_index> FunctionDescriptor::getDescriptors() {
	std::vector<std::type_index> types = getDescriptors<Others...>();
	types.insert(types.begin(), typeid(First));
	return types;
}

template <>
inline std::vector<std::type_index> FunctionDescriptor::getDescriptors<void>() {
	std::vector<std::type_index> vec;
	return vec;
}