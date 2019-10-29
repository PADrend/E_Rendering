/*
	This file is part of the E_Rendering library.
	Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_VertexAccessor.h"
#include "E_Mesh.h"
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Util/StringIdentifier.h>
#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Vec4.h>
#include <E_Util/Graphics/E_Color.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
using namespace Rendering;

// ----------------------------------------
// E_VertexAccessor

//! (static)
EScript::Type * E_VertexAccessor::getTypeObject() {
	// E_VertexAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members
void E_VertexAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_VertexAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	//!	[ESMF] new Rendering.VertexAccessor( Mesh )
	ES_CTOR(typeObject, 1, 1, VertexAccessor::create(parameter[0].to<Mesh*>(rt)))
	
	//! [ESMF] Number VertexAccessor.getAttributeLocation(String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getAttributeLocation",1,1,thisObj->getAttributeLocation(parameter[0].toString()))
	
	//! [ESMF] Geometry.Vec3 VertexAccessor.getPosition(Number index, [String attribute | Number location])
	ES_MFUN(typeObject,const VertexAccessor,"getPosition",1,2,
		parameter[1].isNull() || parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->getPosition(parameter[0].toUInt(), parameter[1].toString(VertexAttributeIds::POSITION.toString())) :
		thisObj->getPosition(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)

	//! [ESMF] thisEObj VertexAccessor.setPosition(Number index, Geometry.Vec3, [String attribute | Number location])
	ES_MFUN(typeObject,VertexAccessor,"setPosition",2,3,(
		parameter[2].isNull() || parameter[2]->isA(EScript::String::getTypeObject()) ? 
		thisObj->setPosition(parameter[0].toUInt(), parameter[1].to<Geometry::Vec3>(rt), parameter[2].toString(VertexAttributeIds::POSITION.toString())) :		
		thisObj->setPosition(parameter[0].toUInt(), parameter[1].to<Geometry::Vec3>(rt), parameter[2].to<uint16_t>(rt))
	,thisEObj))
	
	//! [ESMF] Geometry.Vec3 VertexAccessor.getNormal(Number index, [String attribute | Number location])
	ES_MFUN(typeObject,const VertexAccessor,"getNormal",1,2,
		parameter[1].isNull() || parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->getNormal(parameter[0].toUInt(), parameter[1].toString(VertexAttributeIds::NORMAL.toString())) :
		thisObj->getNormal(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)

	//! [ESMF] thisEObj VertexAccessor.setNormal(Number index, Geometry.Vec3, [String attribute | Number location])
	ES_MFUN(typeObject,VertexAccessor,"setNormal",2,3,(
		parameter[2].isNull() || parameter[2]->isA(EScript::String::getTypeObject()) ? 
		thisObj->setNormal(parameter[0].toUInt(), parameter[1].to<Geometry::Vec3>(rt), parameter[2].toString(VertexAttributeIds::NORMAL.toString())) :		
		thisObj->setNormal(parameter[0].toUInt(), parameter[1].to<Geometry::Vec3>(rt), parameter[2].to<uint16_t>(rt))
	,thisEObj))
	
	//! [ESMF] Util.Color4f VertexAccessor.getColor4f(Number index, [String attribute | Number location])
	ES_MFUN(typeObject,const VertexAccessor,"getColor4f",1,2,
		parameter[1].isNull() || parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->getColor4f(parameter[0].toUInt(), parameter[1].toString(VertexAttributeIds::COLOR.toString())) :
		thisObj->getColor4f(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)

	//! [ESMF] thisEObj VertexAccessor.setColor(Number index, Util.Color4f, [String attribute | Number location])
	ES_MFUN(typeObject,VertexAccessor,"setColor",2,3,(
		parameter[2].isNull() || parameter[2]->isA(EScript::String::getTypeObject()) ? 
		thisObj->setColor(parameter[0].toUInt(), parameter[1].to<Util::Color4f>(rt), parameter[2].toString(VertexAttributeIds::COLOR.toString())) :		
		thisObj->setColor(parameter[0].toUInt(), parameter[1].to<Util::Color4f>(rt), parameter[2].to<uint16_t>(rt))
	,thisEObj))
	
	//! [ESMF] Geometry.Vec2 VertexAccessor.getTexCoord(Number index, [String attribute | Number location])
	ES_MFUN(typeObject,const VertexAccessor,"getTexCoord",1,2,
		parameter[1].isNull() || parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->getTexCoord(parameter[0].toUInt(), parameter[1].toString(VertexAttributeIds::TEXCOORD0.toString())) :
		thisObj->getTexCoord(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)

	//! [ESMF] thisEObj VertexAccessor.setTexCoord(Number index, Geometry.Vec2, [String attribute | Number location])
	ES_MFUN(typeObject,VertexAccessor,"setTexCoord",2,3,(
		parameter[2].isNull() || parameter[2]->isA(EScript::String::getTypeObject()) ? 
		thisObj->setTexCoord(parameter[0].toUInt(), parameter[1].to<Geometry::Vec2>(rt), parameter[2].toString(VertexAttributeIds::TEXCOORD0.toString())) :		
		thisObj->setTexCoord(parameter[0].toUInt(), parameter[1].to<Geometry::Vec2>(rt), parameter[2].to<uint16_t>(rt))
	,thisEObj))
	
	//! [ESMF] Geometry.Vec4 VertexAccessor.getVec4(Number index, String attribute | Number location)
	ES_MFUN(typeObject,const VertexAccessor,"getVec4",2,2,
		parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->getVec4(parameter[0].toUInt(), parameter[1].toString()) :
		thisObj->getVec4(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)

	//! [ESMF] thisEObj VertexAccessor.setVec4(Number index, Geometry.Vec4, String attribute | Number location)
	ES_MFUN(typeObject,VertexAccessor,"setVec4",3,3,(
		parameter[2]->isA(EScript::String::getTypeObject()) ? 
		thisObj->setVec4(parameter[0].toUInt(), parameter[1].to<Geometry::Vec4>(rt), parameter[2].toString()) :		
		thisObj->setVec4(parameter[0].toUInt(), parameter[1].to<Geometry::Vec4>(rt), parameter[2].to<uint16_t>(rt))
	,thisEObj))
		
	//! [ESMF] Number VertexAccessor.getFloat(Number index, String attribute | Number location)
	ES_MFUN(typeObject,const VertexAccessor,"getFloat",2,2,
		parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->readValue<float>(parameter[0].toUInt(), parameter[1].toString()) :
		thisObj->readValue<float>(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)
	
	//! [ESMF] Number VertexAccessor.getFloats(Number index, String attribute | Number location, Number count)
	ES_MFUN(typeObject,const VertexAccessor,"getFloats",3,3,
		EScript::Array::create(parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->readValues<float>(parameter[0].toUInt(), parameter[1].toString(), parameter[2].toUInt()) :
		thisObj->readValues<float>(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), parameter[2].toUInt()))
	)

	//! [ESMF] thisEObj VertexAccessor.setFloat(Number index, String attribute | Number location, Number value)
	ES_MFUN(typeObject,VertexAccessor,"setFloat",3,3,(
		parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->writeValue(parameter[0].toUInt(), parameter[1].toString(), parameter[2].toFloat()) :		
		thisObj->writeValue(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), parameter[2].toFloat())
	,thisEObj))

	//! [ESMF] thisEObj VertexAccessor.setFloats(Number index, String attribute, Array values)
	ES_MFUNCTION(typeObject,VertexAccessor,"setFloats",3,3,{
		EScript::Array * a=parameter[2].to<EScript::Array*>(rt);
		std::vector<float> values;
		for(auto v : *a)
			values.push_back(v.toFloat());
		if(parameter[1]->isA(EScript::String::getTypeObject()))
			thisObj->writeValues(parameter[0].toUInt(), parameter[1].toString(), values);
		else
			thisObj->writeValues(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), values);
		return thisEObj;
	})
		
	//! [ESMF] Number VertexAccessor.getUInt(Number index, String attribute | Number location)
	ES_MFUN(typeObject,const VertexAccessor,"getUInt",2,2,
		parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->readValue<uint32_t>(parameter[0].toUInt(), parameter[1].toString()) :
		thisObj->readValue<uint32_t>(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt))
	)
	
	//! [ESMF] Number VertexAccessor.getUInts(Number index, String attribute | Number location, Number count)
	ES_MFUN(typeObject,const VertexAccessor,"getUInts",3,3,
		EScript::Array::create(parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->readValues<uint32_t>(parameter[0].toUInt(), parameter[1].toString(), parameter[2].toUInt()) :
		thisObj->readValues<uint32_t>(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), parameter[2].toUInt()))
	)

	//! [ESMF] thisEObj VertexAccessor.setUInt(Number index, String attribute | Number location, Number value)
	ES_MFUN(typeObject,VertexAccessor,"setUInt",3,3,(
		parameter[1]->isA(EScript::String::getTypeObject()) ? 
		thisObj->writeValue(parameter[0].toUInt(), parameter[1].toString(), parameter[2].toUInt()) :		
		thisObj->writeValue(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), parameter[2].toUInt())
	,thisEObj))

	//! [ESMF] thisEObj VertexAccessor.setUInts(Number index, String attribute, Array values)
	ES_MFUNCTION(typeObject,VertexAccessor,"setUInts",3,3,{
		EScript::Array * a=parameter[2].to<EScript::Array*>(rt);
		std::vector<uint32_t> values;
		for(auto v : *a)
			values.push_back(v.toUInt());
		if(parameter[1]->isA(EScript::String::getTypeObject()))
			thisObj->writeValues(parameter[0].toUInt(), parameter[1].toString(), values);
		else
			thisObj->writeValues(parameter[0].toUInt(), parameter[1].to<uint16_t>(rt), values);
		return thisEObj;
	})
}

}
