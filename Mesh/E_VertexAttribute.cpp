/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_VertexAttribute.h"
#include <Rendering/Mesh/VertexAttribute.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>
#include <cstddef>
namespace E_Rendering {

EScript::Type * E_VertexAttribute::getTypeObject() {
	// E_VertexAttribute ---|> ReferenceObject ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

void E_VertexAttribute::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib, getClassName(), typeObject);
	using namespace Rendering;

	//! Bool VertexAttribute.empty()
	ES_MFUN(typeObject,const VertexAttribute, "empty", 0, 0, thisObj->empty())

	//! Bool VertexAttribute.getNormalize()
	ES_MFUN(typeObject,const VertexAttribute, "getNormalize", 0, 0, thisObj->getNormalize())

	//! Number VertexAttribute.getOffset()
	ES_MFUN(typeObject,const VertexAttribute, "getOffset", 0, 0, EScript::Number::create(thisObj->getOffset()))

	//! Number VertexAttribute.getDataSize()
	ES_MFUN(typeObject,const VertexAttribute, "getDataSize", 0, 0, EScript::Number::create(thisObj->getDataSize()))

	//! Number VertexAttribute.getDataType()
	ES_MFUN(typeObject,const VertexAttribute, "getDataType", 0, 0, EScript::Number::create(thisObj->getDataType()))

	//! Number VertexAttribute.getNumValues()
	ES_MFUN(typeObject,const VertexAttribute, "getNumValues", 0, 0, EScript::Number::create(thisObj->getNumValues()))

	//! String VertexAttribute.getName()
	ES_MFUN(typeObject,const VertexAttribute, "getName", 0, 0, thisObj->getName())
}

}
