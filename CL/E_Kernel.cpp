#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Kernel.h"
#include "E_Program.h"
#include "E_Memory.h"
#include "E_Device.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Kernel::getTypeObject() {
	// E_Kernel ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Kernel::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Kernel::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Kernel new Kernel()
	ES_CTOR(typeObject,2,2,new E_Kernel(new Kernel(parameter[0].to<Program*>(rt), parameter[1].toString())))

	// bool setArg(index, value)
	//! [ESMF] RESULT Kernel.setArg(p0,p1)
	ES_MFUNCTION(typeObject,Kernel,"setArg",2,2, {
		E_Memory* mem = parameter[1].toType<E_Memory>();
		if(mem)
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),**mem));
		return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].toFloat()));
	})

	// bool setArgs(Array)
	//! [ESMF] RESULT Kernel.setArg(p0,p1)
	ES_MFUNCTION(typeObject,Kernel,"setArgs",1,1, {
		const EScript::Array * a = parameter[0].to<EScript::Array*>(rt);
		bool result = true;
		uint32_t i = 0;
		for(auto val : *a) {
			E_Memory* mem = val.toType<E_Memory>();
			if(mem)
				result &= thisObj->setArg(i++,**mem);
			result &= thisObj->setArg(i++,val.toFloat());
		}
		return result;
	})

	// std::string getArgName(uint32_t index) const
	//! [ESMF] RESULT Kernel.getArgName(p0)
	ES_MFUN(typeObject,const Kernel,"getArgName",1,1,
				EScript::create(thisObj->getArgName(parameter[0].to<uint32_t>(rt))))

	// std::string getArgTypeName(uint32_t index) const
	//! [ESMF] RESULT Kernel.getArgTypeName(p0)
	ES_MFUN(typeObject,const Kernel,"getArgTypeName",1,1,
				EScript::create(thisObj->getArgTypeName(parameter[0].to<uint32_t>(rt))))

	// std::string getAttributes() const
	//! [ESMF] RESULT Kernel.getAttributes()
	ES_MFUN(typeObject,const Kernel,"getAttributes",0,0,
				EScript::create(thisObj->getAttributes()))

	// std::array<size_t, 3> getCompileWorkGroupSize(const Device& device) const
	//! [ESMF] RESULT Kernel.getCompileWorkGroupSize(p0)
	ES_MFUNCTION(typeObject,const Kernel,"getCompileWorkGroupSize",1,1, {
		auto sizes = thisObj->getCompileWorkGroupSize(parameter[0].to<Device*>(rt));
		auto arr = EScript::Array::create();
		for(auto s : sizes)
			arr->pushBack(EScript::Number::create(s));
	})

	// std::string getFunctionName() const
	//! [ESMF] RESULT Kernel.getFunctionName()
	ES_MFUN(typeObject,const Kernel,"getFunctionName",0,0,
				EScript::create(thisObj->getFunctionName()))

	// //	std::array<size_t, 3> getGlobalWorkSize(const Device& device) const
	//! [ESMF] RESULT Kernel.getGlobalWorkSize(p0)
//	ES_MFUN(typeObject,const Kernel,"getGlobalWorkSize",1,1,
//				EScript::create(thisObj->getGlobalWorkSize(parameter[0].to<Device*>(rt))))

	// uint64_t getLocalMemSize(const Device& device) const
	//! [ESMF] RESULT Kernel.getLocalMemSize(p0)
	ES_MFUN(typeObject,const Kernel,"getLocalMemSize",1,1,
			EScript::Number::create(thisObj->getLocalMemSize(parameter[0].to<Device*>(rt))))

	// uint32_t getNumArgs() const
	//! [ESMF] RESULT Kernel.getNumArgs()
	ES_MFUN(typeObject,const Kernel,"getNumArgs",0,0,
				EScript::create(thisObj->getNumArgs()))

	// size_t getPreferredWorkGroupSizeMultiple(const Device& device) const
	//! [ESMF] RESULT Kernel.getPreferredWorkGroupSizeMultiple(p0)
	ES_MFUN(typeObject,const Kernel,"getPreferredWorkGroupSizeMultiple",1,1,
			EScript::Number::create(thisObj->getPreferredWorkGroupSizeMultiple(parameter[0].to<Device*>(rt))))

	// uint64_t getPrivateMemSize(const Device& device) const
	//! [ESMF] RESULT Kernel.getPrivateMemSize(p0)
	ES_MFUN(typeObject,const Kernel,"getPrivateMemSize",1,1,
			EScript::Number::create(thisObj->getPrivateMemSize(parameter[0].to<Device*>(rt))))

	// Program* getProgram() const
	//! [ESMF] RESULT Kernel.getProgram()
	ES_MFUN(typeObject,const Kernel,"getProgram",0,0,
				EScript::create(thisObj->getProgram()))

	// size_t getWorkGroupSize(const Device& device) const
	//! [ESMF] RESULT Kernel.getWorkGroupSize(p0)
	ES_MFUN(typeObject,const Kernel,"getWorkGroupSize",1,1,
			EScript::Number::create(thisObj->getWorkGroupSize(parameter[0].to<Device*>(rt))))


}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

