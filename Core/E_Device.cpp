/*
	This file is part of the E_Rendering library.
	Copyright (C) 2020 Sascha Brandt <sascha@brandt.graphics>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Device.h"

#include <E_Util/UI/E_Window.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <vector>

using namespace EScript;
using namespace Rendering;

namespace E_Rendering {

std::vector<std::string> toStringArray(EScript::Array* array) {
	std::vector<std::string> result;
	if(!array)
		return result;
	for(const auto& obj : *array) {
		result.emplace_back(obj.toString());
	}
	return result;
}

//! (static)
EScript::Type * E_Device::getTypeObject() {
	// E_Device ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_Device::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESMF] new Rendering.Device(Window window)
	ES_CTOR(typeObject,1,2,Device::create(parameter[0].to<Util::UI::Window*>(rt), toStringArray(parameter[1].toType<EScript::Array>())))

}

}
