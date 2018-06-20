/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_VertexDescription.h"
#include "E_VertexAttribute.h"
#include <Rendering/Mesh/VertexAttribute.h>
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Rendering/Mesh/VertexDescription.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>
#include <cstddef>
#include <deque>

namespace EScript {
class Namespace;
class Runtime;
}
namespace E_Rendering {

EScript::Type * E_VertexDescription::getTypeObject() {
	// E_VertexDescription ---|> ReferenceObject ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

/*!	initMembers	*/
void E_VertexDescription::init(EScript::Namespace & lib) {
	using namespace Rendering;

	{ // vertex attribute ids

		EScript::Namespace * E_VertexAttributeIds = new EScript::Namespace;
		declareConstant(&lib,"VertexAttributeIds",E_VertexAttributeIds);

		declareConstant(E_VertexAttributeIds, "POSITION",	VertexAttributeIds::POSITION.toString());
		declareConstant(E_VertexAttributeIds, "NORMAL",		VertexAttributeIds::NORMAL.toString());
		declareConstant(E_VertexAttributeIds, "COLOR",		VertexAttributeIds::COLOR.toString());
		declareConstant(E_VertexAttributeIds, "TANGENT",	VertexAttributeIds::TANGENT.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD0",	VertexAttributeIds::TEXCOORD0.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD1",	VertexAttributeIds::TEXCOORD1.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD2",	VertexAttributeIds::TEXCOORD2.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD3",	VertexAttributeIds::TEXCOORD3.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD4",	VertexAttributeIds::TEXCOORD4.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD5",	VertexAttributeIds::TEXCOORD5.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD6",	VertexAttributeIds::TEXCOORD6.toString());
		declareConstant(E_VertexAttributeIds, "TEXCOORD7",	VertexAttributeIds::TEXCOORD7.toString());

	}

	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib, getClassName(), typeObject);

	//! VertexDescription new VertexDescription()
	ES_CTOR(typeObject,0, 0, Rendering::VertexDescription())

	//! Attribute VertexDescription.appendAttribute(String name, Number numValues, Number type, Bool normalize)
	//! \note only string constants can be used.
	ES_MFUN(typeObject,VertexDescription, "appendAttribute", 4, 4,
				new E_VertexAttribute(thisObj->appendAttribute(parameter[0].toString(), parameter[1].to<uint32_t>(rt), parameter[2].to<uint32_t>(rt), parameter[3].toBool())))

	//! Number VertexDescription.getVertexSize()
	ES_MFUN(typeObject,const VertexDescription, "getVertexSize", 0, 0, EScript::Number::create(thisObj->getVertexSize()))

	//! Number VertexDescription.getNumAttributes()
	ES_MFUN(typeObject,const VertexDescription, "getNumAttributes", 0, 0, EScript::Number::create(thisObj->getNumAttributes()))

	//! Attribute | Void VertexDescription.getAttribute(String name)
	//! \note only string constants can be used.
	//! \note the result type may be different to the c++ version (void)
	ES_MFUNCTION(typeObject, const VertexDescription, "getAttribute", 1, 1, {
		const Rendering::VertexAttribute & attr = thisObj->getAttribute(parameter[0].toString());
		if(attr.empty()) {
			return EScript::create(nullptr);
		} else {
			return EScript::create(attr);
		}
	})

	//! Array VertexDescription.getAttributes()
	ES_MFUN(typeObject, const VertexDescription, "getAttributes", 0, 0, EScript::Array::create(thisObj->getAttributes()))

	//! Bool VertexDescription.hasAttribute(String name)
	ES_MFUN(typeObject,VertexDescription, "hasAttribute", 1, 1, EScript::Bool::create(
					thisObj->hasAttribute(parameter[0].toString())))
					

	//! Attribute VertexDescription.appendFloatAttribute(String name, Number numValues)
	//! \note only string constants can be used.
	ES_MFUN(typeObject,VertexDescription, "appendFloatAttribute", 1, 2,
				(thisObj->appendFloatAttribute(parameter[0].toString(), parameter[1].toUInt(1)),thisEObj))

	//! Attribute VertexDescription.appendUnsignedIntAttribute(String name, Number numValues)
	//! \note only string constants can be used.
	ES_MFUN(typeObject,VertexDescription, "appendUnsignedIntAttribute", 1, 3,
				(thisObj->appendUnsignedIntAttribute(parameter[0].toString(), parameter[1].toUInt(1), parameter[2].toBool(true)),thisEObj))

	//! thisEObj VertexDescription.appendColorRGBAByte()
	ES_MFUN(typeObject,VertexDescription, "appendColorRGBAByte", 0, 0, (thisObj->appendColorRGBAByte(),thisEObj))

	//! thisEObj VertexDescription.appendColorRGBFloat()
	ES_MFUN(typeObject,VertexDescription, "appendColorRGBFloat", 0, 0, (thisObj->appendColorRGBFloat(),thisEObj))

	//! thisEObj VertexDescription.appendColorRGBAFloat()
	ES_MFUN(typeObject,VertexDescription, "appendColorRGBAFloat", 0, 0, (thisObj->appendColorRGBAFloat(),thisEObj))

	//! thisEObj VertexDescription.appendNormalByte()
	ES_MFUN(typeObject,VertexDescription, "appendNormalByte", 0, 0, (thisObj->appendNormalByte(),thisEObj))

	//! thisEObj VertexDescription.appendNormalFloat()
	ES_MFUN(typeObject,VertexDescription, "appendNormalFloat", 0, 0, (thisObj->appendNormalFloat(),thisEObj))

	//! thisEObj VertexDescription.appendPosition2D()
	ES_MFUN(typeObject,VertexDescription, "appendPosition2D", 0, 0, (thisObj->appendPosition2D(),thisEObj))

	//! thisEObj VertexDescription.appendPosition3D()
	ES_MFUN(typeObject,VertexDescription, "appendPosition3D", 0, 0, (thisObj->appendPosition3D(),thisEObj))

	//! thisEObj VertexDescription.appendPosition4D()
	ES_MFUN(typeObject,VertexDescription, "appendPosition4D", 0, 0, (thisObj->appendPosition4D(),thisEObj))

	//! thisEObj VertexDescription.appendPosition4DHalf()
	ES_MFUN(typeObject,VertexDescription, "appendPosition4DHalf", 0, 0, (thisObj->appendPosition4DHalf(),thisEObj))

	//! thisEObj VertexDescription.appendTexCoord()
	ES_MFUN(typeObject,VertexDescription, "appendTexCoord", 0, 1, (thisObj->appendTexCoord(parameter[0].toUInt(0)),thisEObj))
}

}
