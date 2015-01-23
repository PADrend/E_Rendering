/*
	This file is part of the Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Platform.h"
#include "E_Device.h"

#include <Rendering/CL/Device.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <vector>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Platform::getTypeObject() {
	// E_Platform ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Platform::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Platform::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	// static std::vector<PlatformRef> get()
	//! [ESF] RESULT Platform.get()
	ES_FUNCTION(typeObject,"get",0,0, {
		auto platforms = Platform::get();
		auto arr = EScript::Array::create();
		for(auto platform : platforms)
			arr->pushBack(EScript::create(platform.get()));
		return arr;
	})

	// std::vector<DeviceRef> getDevices() const
	//! [ESMF] RESULT Platform.getDevices()
	ES_MFUNCTION(typeObject,Platform,"getDevices",0,0, {
		auto devices = thisObj->getDevices();
		auto arr = EScript::Array::create();
		for(auto dev : devices)
			arr->pushBack(EScript::create(dev.get()));
		return arr;
	})

	// std::string getExtensions() const
	//! [ESMF] RESULT Platform.getExtensions()
	ES_MFUN(typeObject,const Platform,"getExtensions",0,0,
				EScript::create(thisObj->getExtensions()))

	// std::string getName() const
	//! [ESMF] RESULT Platform.getName()
	ES_MFUN(typeObject,const Platform,"getName",0,0,
				EScript::create(thisObj->getName()))

	// std::string getProfile() const
	//! [ESMF] RESULT Platform.getProfile()
	ES_MFUN(typeObject,const Platform,"getProfile",0,0,
				EScript::create(thisObj->getProfile()))

	// std::string getVendor() const
	//! [ESMF] RESULT Platform.getVendor()
	ES_MFUN(typeObject,const Platform,"getVendor",0,0,
				EScript::create(thisObj->getVendor()))

	// std::string getVersion() const
	//! [ESMF] RESULT Platform.getVersion()
	ES_MFUN(typeObject,const Platform,"getVersion",0,0,
				EScript::create(thisObj->getVersion()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

