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
	ES_CTOR(typeObject, 1, 1, new VertexAccessor(parameter[0].to<Mesh*>(rt)->openVertexData()))
	
	//! [ESMF] Geometry.Vec3 VertexAccessor.getPosition(Number index, [String attribute])
	ES_MFUN(typeObject,const VertexAccessor,"getPosition",1,2,thisObj->getPosition(parameter[0].to<uint32_t>(rt), parameter[1].toString(VertexAttributeIds::POSITION.toString())))

	//! [ESMF] thisEObj VertexAccessor.setPosition(Number index, Geometry.Vec3, [String attribute])
	ES_MFUN(typeObject,VertexAccessor,"setPosition",2,3,(thisObj->setPosition(parameter[0].to<uint32_t>(rt), parameter[1].to<Geometry::Vec3>(rt), parameter[2].toString(VertexAttributeIds::POSITION.toString())),thisEObj))
	
	//! [ESMF] Geometry.Vec3 VertexAccessor.getNormal(Number index, [String attribute])
	ES_MFUN(typeObject,const VertexAccessor,"getNormal",1,2,thisObj->getNormal(parameter[0].to<uint32_t>(rt), parameter[1].toString(VertexAttributeIds::NORMAL.toString())))

	//! [ESMF] thisEObj VertexAccessor.setNormal(Number index, Geometry.Vec3, [String attribute])
	ES_MFUN(typeObject,VertexAccessor,"setNormal",2,3,(thisObj->setPosition(parameter[0].to<uint32_t>(rt), parameter[1].to<Geometry::Vec3>(rt), parameter[2].toString(VertexAttributeIds::NORMAL.toString())),thisEObj))
	
	//! [ESMF] Util.Color4f VertexAccessor.getColor4f(Number index, [String attribute])
	ES_MFUN(typeObject,const VertexAccessor,"getColor4f",1,2,thisObj->getColor4f(parameter[0].to<uint32_t>(rt), parameter[1].toString(VertexAttributeIds::COLOR.toString())))

	//! [ESMF] thisEObj VertexAccessor.setColor(Number index, Util.Color, [String attribute])
	ES_MFUN(typeObject,VertexAccessor,"setColor",2,3,(thisObj->setColor(parameter[0].to<uint32_t>(rt), parameter[1].to<Util::Color4f>(rt), parameter[2].toString(VertexAttributeIds::COLOR.toString())),thisEObj))
	
	//! [ESMF] Geometry.Vec2 VertexAccessor.getTexCoord(Number index, [String attribute])
	ES_MFUN(typeObject,const VertexAccessor,"getTexCoord",1,2,thisObj->getTexCoord(parameter[0].to<uint32_t>(rt), parameter[1].toString(VertexAttributeIds::TEXCOORD0.toString())))

	//! [ESMF] thisEObj VertexAccessor.setTexCoord(Number index, Geometry.Vec2, [String attribute])
	ES_MFUN(typeObject,VertexAccessor,"setTexCoord",2,3,(thisObj->setTexCoord(parameter[0].to<uint32_t>(rt), parameter[1].to<Geometry::Vec2>(rt), parameter[2].toString(VertexAttributeIds::TEXCOORD0.toString())),thisEObj))
	
	//! [ESMF] Geometry.Vec4 VertexAccessor.getVec4(Number index, String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getVec4",2,2,thisObj->getVec4(parameter[0].to<uint32_t>(rt), parameter[1].toString()))

	//! [ESMF] thisEObj VertexAccessor.setVec4(Number index, Geometry.Vec4, String attribute)
	ES_MFUN(typeObject,VertexAccessor,"setVec4",3,3,(thisObj->setVec4(parameter[0].to<uint32_t>(rt), parameter[1].to<Geometry::Vec4>(rt), parameter[2].toString()),thisEObj))

	//! [ESMF] Number VertexAccessor.getFloat(Number index, String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getFloat",2,2,thisObj->getFloat(parameter[0].to<uint32_t>(rt), parameter[1].toString()))

	//! [ESMF] Array VertexAccessor.getFloats(Number index, String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getFloats",2,2,EScript::Array::create(thisObj->getFloats(parameter[0].to<uint32_t>(rt), parameter[1].toString())))
	
	//! [ESMF] thisEObj VertexAccessor.setFloat(Number index, Number value, String attribute)
	ES_MFUN(typeObject,VertexAccessor,"setFloat",3,3,(thisObj->setFloat(parameter[0].to<uint32_t>(rt), parameter[1].toFloat(), parameter[2].toString()),thisEObj))
	
	//! [ESMF] thisEObj VertexAccessor.setFloats(Number index, Array values, String attribute)
	ES_MFUNCTION(typeObject,VertexAccessor,"setFloats",3,3,{
		EScript::Array * a=parameter[1].to<EScript::Array*>(rt);
		std::vector<float> values;
		for(auto v : *a)
			values.push_back(v.toFloat());
		thisObj->setFloats(parameter[0].to<uint32_t>(rt), values, parameter[2].toString());
		return thisEObj;
	})
	
	//! [ESMF] Number VertexAccessor.getUInt(Number index, String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getUInt",2,2,thisObj->getUInt(parameter[0].to<uint32_t>(rt), parameter[1].toString()))

	//! [ESMF] Array VertexAccessor.getUInts(Number index, String attribute)
	ES_MFUN(typeObject,const VertexAccessor,"getUInts",2,2,EScript::Array::create(thisObj->getUInts(parameter[0].to<uint32_t>(rt), parameter[1].toString())))
	
	//! [ESMF] thisEObj VertexAccessor.setUInt(Number index, Number value, String attribute)
	ES_MFUN(typeObject,VertexAccessor,"setUInt",3,3,(thisObj->setUInt(parameter[0].to<uint32_t>(rt), parameter[1].toUInt(), parameter[2].toString()),thisEObj))
	
	//! [ESMF] thisEObj VertexAccessor.setUInts(Number index, Array values, String attribute)
	ES_MFUNCTION(typeObject,VertexAccessor,"setUInts",3,3,{
		EScript::Array * a=parameter[1].to<EScript::Array*>(rt);
		std::vector<uint32_t> values;
		for(auto v : *a)
			values.push_back(v.toUInt());
		thisObj->setUInts(parameter[0].to<uint32_t>(rt), values, parameter[2].toString());
		return thisEObj;
	})
}

}
