/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_BufferObject.h"

#include <EScript/Basics.h>

namespace E_Rendering {

//! (static)
EScript::Type * E_BufferObject::getTypeObject() {
	// E_BufferObject ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_BufferObject::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	using namespace Rendering;

	//! [ESMF] BufferObject new BufferObject()
	ES_CTOR(typeObject,0,0,new CountedBufferObject(BufferObject()));

	//! [ESMF] Bool BufferObject.isValid()
	ES_MFUN(typeObject,BufferObject,"isValid",0,0,thisObj->isValid())

	//! [ESMF] thisEObj BufferObject.swap(E_BufferObject)
	ES_MFUN(typeObject,BufferObject,"swap",0,0,(thisObj->swap(*parameter[1].to<BufferObject*>(rt)),thisEObj))

	//! [ESMF] thisEObj BufferObject.prepare()
	ES_MFUN(typeObject,BufferObject,"prepare",0,0,(thisObj->prepare(), thisEObj))

	//! [ESMF] thisEObj BufferObject.destroy()
	ES_MFUN(typeObject,BufferObject,"destroy",0,0,(thisObj->destroy(), thisEObj))

	//! [ESMF] thisEObj BufferObject.allocateData(bufferTarget, numBytes, usageHint)
	ES_MFUN(typeObject,BufferObject,"allocateData",3,3,(thisObj->allocateData<uint8_t>(parameter[0].toUInt(), parameter[1].toUInt(), parameter[2].toUInt()), thisEObj))

}

}
