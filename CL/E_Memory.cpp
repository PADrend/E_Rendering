#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Memory.h"
#include "E_Context.h"
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Memory::getTypeObject() {
	// E_Memory ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Memory::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Memory::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;

	declareConstant(&lib, "NO_ACCESS", static_cast<uint32_t>(ReadWrite_t::NoAccess));
	declareConstant(&lib, "READ_ONLY", static_cast<uint32_t>(ReadWrite_t::ReadOnly));
	declareConstant(&lib, "READ_WRITE", static_cast<uint32_t>(ReadWrite_t::ReadWrite));
	declareConstant(&lib, "WRITE_ONLY", static_cast<uint32_t>(ReadWrite_t::WriteOnly));
	
	// Context* getContext() const
	//! [ESMF] RESULT Memory.getContext()
	ES_MFUN(typeObject,const Memory,"getContext",0,0,
				EScript::create(thisObj->getContext()))

	// uint32_t getFlags() const
	//! [ESMF] RESULT Memory.getFlags()
	ES_MFUN(typeObject,const Memory,"getFlags",0,0,
				EScript::create(thisObj->getFlags()))

	// void* getHostPtr() const
	//! [ESMF] self Memory.getHostPtr()
//	ES_MFUN(typeObject,const Memory,"getHostPtr",0,0,
//				(thisObj->getHostPtr(),thisEObj))

	// size_t getOffset() const
	//! [ESMF] RESULT Memory.getOffset()
	ES_MFUN(typeObject,const Memory,"getOffset",0,0,
				EScript::Number::create(thisObj->getOffset()))

	// size_t getSize() const
	//! [ESMF] RESULT Memory.getSize()
	ES_MFUN(typeObject,const Memory,"getSize",0,0,
				EScript::Number::create(thisObj->getSize()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

