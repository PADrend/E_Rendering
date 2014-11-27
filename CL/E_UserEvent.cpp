#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_UserEvent.h"
#include "E_Context.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_UserEvent::getTypeObject() {
	// E_UserEvent ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_Event::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_UserEvent::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_UserEvent::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] UserEvent new UserEvent(Context)
	ES_CTOR(typeObject,1,1,new E_UserEvent(new UserEvent(parameter[0].to<Context*>(rt))))

	// void setStatus(int32_t status)
	//! [ESMF] self UserEvent.setStatus(p0)
	ES_MFUN(typeObject,UserEvent,"setStatus",1,1,
				(thisObj->setStatus(parameter[0].toInt()),thisEObj))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL
