/*
	This file is part of the Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Event.h"
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Event::getTypeObject() {
	// E_Event ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Event::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Event::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Event new Event()
	ES_CTOR(typeObject,0,0,new E_Event(new Event()))

	// uint64_t getProfilingCommandEnd() const
	//! [ESMF] Number Event.getProfilingCommandEnd()
	ES_MFUN(typeObject,const Event,"getProfilingCommandEnd",0,0,
			EScript::Number::create(thisObj->getProfilingCommandEnd()))

	// uint64_t getProfilingCommandQueued() const
	//! [ESMF] Number Event.getProfilingCommandQueued()
	ES_MFUN(typeObject,const Event,"getProfilingCommandQueued",0,0,
			EScript::Number::create(thisObj->getProfilingCommandQueued()))

	// uint64_t getProfilingCommandStart() const
	//! [ESMF] Number Event.getProfilingCommandStart()
	ES_MFUN(typeObject,const Event,"getProfilingCommandStart",0,0,
			EScript::Number::create(thisObj->getProfilingCommandStart()))

	// uint64_t getProfilingCommandSubmit() const
	//! [ESMF] Number Event.getProfilingCommandSubmit()
	ES_MFUN(typeObject,const Event,"getProfilingCommandSubmit",0,0,
			EScript::Number::create(thisObj->getProfilingCommandSubmit()))

	// uint64_t getProfilingNanoseconds() const
	//! [ESMF] Number Event.getProfilingNanoseconds()
	ES_MFUN(typeObject,const Event,"getProfilingNanoseconds",0,0,
			EScript::Number::create(thisObj->getProfilingNanoseconds()))

	// double getProfilingMicroseconds() const
	//! [ESMF] Number Event.getProfilingMicroseconds()
	ES_MFUN(typeObject,const Event,"getProfilingMicroseconds",0,0,
			EScript::Number::create(thisObj->getProfilingMicroseconds()))

	// double getProfilingNanoseconds() const
	//! [ESMF] Number Event.getProfilingMilliseconds()
	ES_MFUN(typeObject,const Event,"getProfilingMilliseconds",0,0,
			EScript::Number::create(thisObj->getProfilingMilliseconds()))

	// double getProfilingSeconds() const
	//! [ESMF] Number Event.getProfilingSeconds()
	ES_MFUN(typeObject,const Event,"getProfilingSeconds",0,0,
			EScript::Number::create(thisObj->getProfilingSeconds()))

	// void setCallback(CallbackFn_t fun)
	//! [ESMF] self Event.setCallback(p0)
//	ES_MFUN(typeObject,Event,"setCallback",1,1,
//				(thisObj->setCallback(parameter[0].to<XXX>(rt)),thisEObj))

	// void wait()
	//! [ESMF] self Event.wait()
	ES_MFUN(typeObject,Event,"wait",0,0,
				(thisObj->wait(),thisEObj))

	// static void waitForEvents(const std::vector<Event*>& events)
	//! [ESF] Void Event.waitForEvents(p0)
	ES_FUNCTION(typeObject,"waitForEvents",1,1, {
		const EScript::Array * a = parameter[0].to<EScript::Array*>(rt);
		std::vector<Event*> events;
		for(auto e : *a)
			events.push_back(e.to<Event*>(rt));
		Event::waitForEvents(events);
	})
}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

