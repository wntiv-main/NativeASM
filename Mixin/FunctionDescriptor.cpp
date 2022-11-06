#include "FunctionDescriptor.h"
#include <string>

const std::type_index FunctionDescriptor::unknownType = typeid(FunctionDescriptor::Unknown);

std::string FunctionDescriptor::getReturnTypeName() {
	return returnType == unknownType ?
		"???" : returnType.name();
}

std::string FunctionDescriptor::getArgumentTypeNames() {
	if (argumentTypes.size() == 1 && argumentTypes.at(0) == unknownType)
		return "?...";
	std::string result = "";
	bool first = true;
	for (std::type_index type : argumentTypes) {
		if (!first) result += ", ";
		first = false;
		result += type.name();
	}
	return result;
}