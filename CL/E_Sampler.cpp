/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Sampler.h"
#include "E_Context.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Sampler::getTypeObject() {
	// E_Sampler ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Sampler::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Sampler::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Sampler new Sampler()
	ES_CTOR(typeObject,4,4,new E_Sampler(new Sampler(parameter[0].to<Context*>(rt),
			parameter[1].toBool(),
			static_cast<AdressingMode_t>(parameter[2].to<uint32_t>(rt)),
			static_cast<FilterMode_t>(parameter[3].to<uint32_t>(rt)))))

	// AdressingMode_r getAdressingMode() const
	//! [ESMF] RESULT Sampler.getAdressingMode()
	ES_MFUN(typeObject,const Sampler,"getAdressingMode",0,0,
				EScript::create(static_cast<uint32_t>(thisObj->getAdressingMode())))

	// Context* getContext() const
	//! [ESMF] RESULT Sampler.getContext()
	ES_MFUN(typeObject,const Sampler,"getContext",0,0,
				EScript::create(thisObj->getContext()))

	// FilterMode_t getFilterMode() const
	//! [ESMF] RESULT Sampler.getFilterMode()
	ES_MFUN(typeObject,const Sampler,"getFilterMode",0,0,
				EScript::create(static_cast<uint32_t>(thisObj->getFilterMode())))

	// bool hasNormalizedCoords() const
	//! [ESMF] RESULT Sampler.hasNormalizedCoords()
	ES_MFUN(typeObject,const Sampler,"hasNormalizedCoords",0,0,
				EScript::create(thisObj->hasNormalizedCoords()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

