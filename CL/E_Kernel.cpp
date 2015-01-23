#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Kernel.h"
#include "E_Program.h"
#include "E_Memory.h"
#include "E_Device.h"

#include <E_Geometry/E_Vec3.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <Util/TypeConstant.h>

namespace EScript {
template<> inline int8_t convertTo<int8_t>(Runtime& rt,ObjPtr src)	{	return static_cast<int8_t>(convertTo<double>(rt,src));	}
template<> inline uint8_t convertTo<uint8_t>(Runtime& rt,ObjPtr src)	{	return static_cast<uint8_t>(convertTo<double>(rt,src));	}
}

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
	ES_MFUNCTION(typeObject,Kernel,"setArg",2,3, {
		E_Memory* mem = parameter[1].toType<E_Memory>();
		if(mem)
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),**mem));

		E_Geometry::E_Vec3* vec3 = parameter[1].toType<E_Geometry::E_Vec3>();
		if(vec3) {
			std::vector<float> values({
				(**vec3).x(), (**vec3).y(), (**vec3).z(), 1
			});
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),values.size()*sizeof(float),values.data()));
		}

		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[2].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		switch(type) {
		case Util::TypeConstant::UINT8:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<uint8_t>(rt)));
		case Util::TypeConstant::INT8:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<int8_t>(rt)));
		case Util::TypeConstant::UINT16:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<uint16_t>(rt)));
		case Util::TypeConstant::INT16:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<int16_t>(rt)));
		case Util::TypeConstant::UINT32:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt)));
		case Util::TypeConstant::INT32:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<int32_t>(rt)));
		case Util::TypeConstant::UINT64:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<uint64_t>(rt)));
		case Util::TypeConstant::INT64:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<int64_t>(rt)));
		case Util::TypeConstant::DOUBLE:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].to<double>(rt)));
		default:
			return EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt),parameter[1].toFloat()));
		}
	})

	// bool setArg(index, size, nullptr)
	//! [ESMF] RESULT Kernel.setArgSize(index, size)
	ES_MFUN(typeObject,Kernel,"setArgSize",2,2,
				EScript::create(thisObj->setArg(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt), nullptr)))

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

