/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Program.h"
#include "E_Context.h"
#include "E_Device.h"

#include <E_Util/E_FileName.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Program::getTypeObject() {
	// E_Program ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Program::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Program::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Program new Program()
	ES_CTOR(typeObject,1,2,new E_Program(new Program(parameter[0].to<Context*>(rt), {parameter[1].toString()})))

	// bool build(const std::vector<DeviceRef>& devices, const std::string& options = "")
	//! [ESMF] RESULT Program.build(p0[,p1])
	ES_MFUN(typeObject,Program,"build",1,2,
				EScript::create(thisObj->build({parameter[0].to<Device*>(rt)},parameter[1].toString(""))))

//	// std::vector<char*> getBinaries() const
//	//! [ESMF] RESULT Program.getBinaries()
//	ES_MFUN(typeObject,const E_Program,"getBinaries",0,0,
//				EScript::create(thisObj->getBinaries()))
//
//	// std::vector<size_t> getBinarySizes() const
//	//! [ESMF] RESULT Program.getBinarySizes()
//	ES_MFUN(typeObject,const E_Program,"getBinarySizes",0,0,
//				EScript::create(thisObj->getBinarySizes()))

	// std::string getBuildLog(Device* device) const
	//! [ESMF] RESULT Program.getBuildLog(p0)
	ES_MFUN(typeObject,const Program,"getBuildLog",1,1,
				EScript::create(thisObj->getBuildLog(parameter[0].to<Device*>(rt))))

	// std::string getBuildOptions(Device* device) const
	//! [ESMF] RESULT Program.getBuildOptions(p0)
	ES_MFUN(typeObject,const Program,"getBuildOptions",1,1,
				EScript::create(thisObj->getBuildOptions(parameter[0].to<Device*>(rt))))

	// BuildStatus_t getBuildStatus(Device* device) const
	//! [ESMF] RESULT Program.getBuildStatus(p0)
	ES_MFUN(typeObject,const Program,"getBuildStatus",1,1,
				EScript::create(static_cast<uint32_t>(thisObj->getBuildStatus(parameter[0].to<Device*>(rt)))))

	// Context* getContext() const
	//! [ESMF] RESULT Program.getContext()
	ES_MFUN(typeObject,const Program,"getContext",0,0,
				EScript::create(thisObj->getContext()))

	// std::vector<DeviceRef> getDevices() const
	//! [ESMF] RESULT Program.getDevices()
	ES_MFUNCTION(typeObject,const Program,"getDevices",0,0, {
		auto devices = thisObj->getDevices();
		auto arr = EScript::Array::create();
		for(auto dev : devices)
			arr->pushBack(EScript::create(dev.get()));
		return arr;
	})

	// std::string getKernelNames() const
	//! [ESMF] RESULT Program.getKernelNames()
	ES_MFUN(typeObject,const Program,"getKernelNames",0,0,
				EScript::create(thisObj->getKernelNames()))

	// uint32_t getNumDevices() const
	//! [ESMF] RESULT Program.getNumDevices()
	ES_MFUN(typeObject,const Program,"getNumDevices",0,0,
				EScript::create(thisObj->getNumDevices()))

	// uint32_t getNumKernels() const
	//! [ESMF] RESULT Program.getNumKernels()
	ES_MFUN(typeObject,const Program,"getNumKernels",0,0,
				EScript::create(thisObj->getNumKernels()))

	// std::string getSource() const
	//! [ESMF] RESULT Program.getSource()
	ES_MFUN(typeObject,const Program,"getSource",0,0,
				EScript::create(thisObj->getSource()))

	// bool attachSource(FileName)
	//! [ESMF] RESULT Program.attachFile(FileName)
	ES_MFUNCTION(typeObject,Program,"attachFile",1,1, {
		const Util::FileName file(parameter[0].toString());
		thisObj->attachSource(file);
		return thisEObj;
	})

	// bool attachSource(string)
	//! [ESMF] RESULT Program.attachSource(string)
	ES_MFUNCTION(typeObject,Program,"attachSource",1,1, {
		thisObj->attachSource(parameter[0].toString());
		return thisEObj;
	})

	// bool addInclude(FileName)
	//! [ESMF] RESULT Program.addInclude(FileName)
	ES_MFUNCTION(typeObject,Program,"addInclude",1,1, {
		const Util::FileName file(parameter[0].toString());
		thisObj->addInclude(file);
		return thisEObj;
	})

	// bool addDefine(string, string)
	//! [ESMF] RESULT Program.addDefine(string, [string])
	ES_MFUNCTION(typeObject,Program,"addDefine",1,2, {
		thisObj->addDefine(parameter[0].toString(), parameter[1]->isA(EScript::String::getTypeObject()) ? "\"" + parameter[1].toString() + "\"" : parameter[1].toString(""));
		return thisEObj;
	})
}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

