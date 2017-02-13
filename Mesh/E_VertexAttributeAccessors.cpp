/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_VertexAttributeAccessors.h"
#include "E_Mesh.h"
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Util/StringIdentifier.h>
#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Util/Graphics/E_Color.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
using namespace Rendering;

// ----------------------------------------
// E_VertexAttributeAccessor

//! (static)
EScript::Type * E_VertexAttributeAccessor::getTypeObject() {
	// E_VertexAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_VertexAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_VertexAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESMF] Bool vertexAttributeAccessor.checkRange(uint)
	ES_MFUN(typeObject,const VertexAttributeAccessor,"checkRange",1,1,
				thisObj->checkRange(parameter[0].to<uint32_t>(rt)))

	E_ColorAttributeAccessor::init(lib);
	E_NormalAttributeAccessor::init(lib);
	E_PositionAttributeAccessor::init(lib);
	E_TexCoordAttributeAccessor::init(lib);
	E_FloatAttributeAccessor::init(lib);
	E_UIntAttributeAccessor::init(lib);
}


// ---------------------------------------------------------
// E_ColorAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_ColorAttributeAccessor::getTypeObject() {
	// E_ColorAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_ColorAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_ColorAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);


	//! [ESF] ColorAttributeAccessor Rendering.ColorAttributeAccessor.create(Mesh, [name])
	ES_FUN(typeObject,"create",1,2,Rendering::ColorAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),
					parameter[1].toString(VertexAttributeIds::COLOR.toString())))

	//! [ESMF] Util.Color4f colorAttributeAccessor.getColor4f(index)
	ES_MFUN(typeObject,const ColorAttributeAccessor,"getColor4f",1,1,
				thisObj->getColor4f(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] thisEObj colorAttributeAccessor.setColor(index,Util.Color4f | Util.Color4ub)
	ES_MFUN(typeObject,ColorAttributeAccessor,"setColor",2,2,
				(thisObj->setColor(parameter[0].to<uint32_t>(rt),parameter[1].to<Util::Color4f>(rt)),thisEObj))
}

// ---------------------------------------------------------
// E_NormalAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_NormalAttributeAccessor::getTypeObject() {
	// E_NormalAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_NormalAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_NormalAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESF] NormalAttributeAccessor Rendering.NormalAttributeAccessor.create(Mesh,[name])
	ES_FUN(typeObject,"create",1,2,Rendering::NormalAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),parameter[1].toString(VertexAttributeIds::NORMAL.toString())))

	//! [ESMF] Geometry.Vec3 colorAttributeAccessor.getNormal(index)
	ES_MFUN(typeObject,const NormalAttributeAccessor,"getNormal",1,1,thisObj->getNormal(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] thisEObj colorAttributeAccessor.setNormal(Geometry.Vec3)
	ES_MFUN(typeObject,NormalAttributeAccessor,"setNormal",2,2,(
				thisObj->setNormal(parameter[0].to<uint32_t>(rt),
										parameter[1].to<Geometry::Vec3>(rt)),thisEObj))
}

// ---------------------------------------------------------
// E_PositionAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_PositionAttributeAccessor::getTypeObject() {
	// E_PositionAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_PositionAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_PositionAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESF] PositionAttributeAccessor Rendering.PositionAttributeAccessor.create(Mesh,[name])
	ES_FUN(typeObject,"create",1,2,Rendering::PositionAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),parameter[1].toString(VertexAttributeIds::POSITION.toString())))

	//! [ESMF] Geometry.Vec3 colorAttributeAccessor.getPosition(index)
	ES_MFUN(typeObject,const PositionAttributeAccessor,"getPosition",1,1,thisObj->getPosition(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] thisEObj colorAttributeAccessor.setPosition(Geometry.Vec3)
	ES_MFUN(typeObject,PositionAttributeAccessor,"setPosition",2,2,(
				thisObj->setPosition(parameter[0].to<uint32_t>(rt),
											parameter[1].to<Geometry::Vec3>(rt)),thisEObj))
}

// ---------------------------------------------------------
// E_TexCoordAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_TexCoordAttributeAccessor::getTypeObject() {
	// E_TexCoordAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_TexCoordAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_TexCoordAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESF] TexCoordAttributeAccessor Rendering.TexCoordAttributeAccessor.create(Mesh,[name])
	ES_FUN(typeObject,"create",1,2,Rendering::TexCoordAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),parameter[1].toString(VertexAttributeIds::TEXCOORD0.toString())))

	//! [ESMF] Geometry.Vec2 colorAttributeAccessor.getCoordinate(index)
	ES_MFUN(typeObject,const TexCoordAttributeAccessor,"getCoordinate",1,1,thisObj->getCoordinate(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] thisEObj colorAttributeAccessor.setCoordinate(Geometry.Vec2)
	ES_MFUN(typeObject,TexCoordAttributeAccessor,"setCoordinate",2,2,(
				thisObj->setCoordinate(parameter[0].to<uint32_t>(rt),
											parameter[1].to<const Geometry::Vec2&>(rt)),thisEObj))
}

// ---------------------------------------------------------
// E_FloatAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_FloatAttributeAccessor::getTypeObject() {
	// E_FloatAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_FloatAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_FloatAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESF] FloatAttributeAccessor Rendering.FloatAttributeAccessor.create(Mesh,name)
	ES_FUN(typeObject,"create",2,2,Rendering::FloatAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),parameter[1].toString()))

	//! [ESMF] Number FloatAttributeAccessor.getValue(index)
	ES_MFUN(typeObject,const FloatAttributeAccessor,"getValue",1,1,thisObj->getValue(parameter[0].to<uint32_t>(rt)))
	
	//! [ESMF] Number FloatAttributeAccessor.getValues(index)
	ES_MFUNCTION(typeObject,const FloatAttributeAccessor,"getValues",1,1,{
		const auto values = thisObj->getValues(parameter[0].to<uint32_t>(rt));
		return EScript::Array::create(values); 
	})
	
	//! [ESMF] thisEObj FloatAttributeAccessor.setValue(Number, Number)
	ES_MFUN(typeObject,FloatAttributeAccessor,"setValue",2,2,(
				thisObj->setValue(parameter[0].to<uint32_t>(rt),
											parameter[1].toFloat()),thisEObj))
											
	//! [ESMF] thisEObj FloatAttributeAccessor.setValue(Number, Number)
	ES_MFUNCTION(typeObject,FloatAttributeAccessor,"setValues",2,2,{		
			EScript::Array * a=parameter[1].toType<EScript::Array>();
			if(a) {
				std::vector<float> values;
				for(auto v : *a)
					values.push_back(v.toFloat());
				thisObj->setValues(parameter[0].to<uint32_t>(rt), values);
			} else {
				thisObj->setValue(parameter[0].to<uint32_t>(rt),parameter[1].toFloat());
			}
			return thisEObj;
	})
}


// ---------------------------------------------------------
// E_UIntAttributeAccessor ---|> E_VertexAttributeAccessor

//! (static)
EScript::Type * E_UIntAttributeAccessor::getTypeObject() {
	// E_FloatAttributeAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_VertexAttributeAccessor::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_UIntAttributeAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_UIntAttributeAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESF] UIntAttributeAccessor Rendering.UIntAttributeAccessor.create(Mesh,name)
	ES_FUN(typeObject,"create",2,2,Rendering::UIntAttributeAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)->openVertexData(),parameter[1].toString()))

	//! [ESMF] Number UIntAttributeAccessor.getValue(index)
	ES_MFUN(typeObject,const UIntAttributeAccessor,"getValue",1,1,thisObj->getValue(parameter[0].to<uint32_t>(rt)))
	
	//! [ESMF] Number UIntAttributeAccessor.getValues(index)
	ES_MFUNCTION(typeObject,const UIntAttributeAccessor,"getValues",1,1,{
		const auto values = thisObj->getValues(parameter[0].to<uint32_t>(rt));
		return EScript::Array::create(values); 
	})
	
	//! [ESMF] thisEObj UIntAttributeAccessor.setValue(Number, Number)
	ES_MFUN(typeObject,UIntAttributeAccessor,"setValue",2,2,(
				thisObj->setValue(parameter[0].to<uint32_t>(rt),
											parameter[1].to<uint32_t>(rt)),thisEObj))
											
	//! [ESMF] thisEObj UIntAttributeAccessor.setValue(Number, Number)
	ES_MFUNCTION(typeObject,UIntAttributeAccessor,"setValues",2,2,{		
		EScript::Array * a=parameter[1].toType<EScript::Array>();
		if(a) {
			std::vector<uint32_t> values;
			for(auto v : *a)
				values.push_back(v.toUInt());
			thisObj->setValues(parameter[0].to<uint32_t>(rt), values);
		} else {
			thisObj->setValue(parameter[0].to<uint32_t>(rt),parameter[1].toUInt());
		}
		return thisEObj;
	})
}

}
