
#include "E_Context.h"
#include "E_Device.h"
#include "E_Platform.h"
#include <vector>

#include <Rendering/CL/CLUtils.h>
#include <Rendering/CL/Platform.h>
#include <Rendering/CL/Device.h>

#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Context.h"
#include "E_Device.h"
#include "E_Platform.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Context::getTypeObject() {
	// E_Context ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Context::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Context::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Context new Context()
	ES_CONSTRUCTOR(typeObject,2,3,{
		Platform* platform = parameter[0].to<Platform*>(rt);
		EScript::Array* arr = parameter[1].toType<EScript::Array>();
		std::vector<DeviceRef> devices;
		if(arr) {
			for(uint32_t i=0; i<arr->size(); ++i)
				devices.push_back(arr->at(i).to<Device*>(rt));
			return new E_Context(new Context(platform, devices, parameter[2].toBool()));
		}
		auto device = parameter[1].toType<E_Device>();
		if(device) {
			return new E_Context(new Context(platform, (**device).get(), parameter[2].toBool()));
		}
		return new E_Context(new Context(platform, parameter[1].to<uint32_t>(rt), parameter[2].toBool()));
	});

	// std::vector<DeviceRef> getDevices()
	//! [ESMF] RESULT Context.getDevices()
	ES_MFUNCTION(typeObject,Context,"getDevices",0,0, {
		auto devices = thisObj->getDevices();
		auto arr = EScript::Array::create();
		for(auto dev : devices)
			arr->pushBack(EScript::create(dev.get()));
		return arr;
	})

	// Platform* getPlatform() const
	//! [ESMF] RESULT Context.getPlatform()
	ES_MFUN(typeObject,const Context,"getPlatform",0,0,
				EScript::create(thisObj->getPlatform()))

	// std::vector<intptr_t> getProperties() const
	//! [ESMF] RESULT Context.getProperties()
//	ES_MFUN(typeObject,const Context,"getProperties",0,0,
//				EScript::create(thisObj->getProperties()))

	// bool isUsingGLInterop() const
	//! [ESMF] RESULT Context.isUsingGLInterop()
	ES_MFUN(typeObject,const Context,"isUsingGLInterop",0,0,
				EScript::create(thisObj->isUsingGLInterop()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

