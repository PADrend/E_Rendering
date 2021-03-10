/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_MeshBuilder.h"

#include "../Mesh/E_Mesh.h"
#include "../Mesh/E_VertexDescription.h"
#include "../Texture/E_Texture.h"

#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Matrix4x4.h>
#include <E_Geometry/E_SRT.h>
#include <E_Geometry/E_Sphere.h>
#include <E_Geometry/E_Box.h>
#include <E_Util/Graphics/E_Color.h>
#include <E_Util/Graphics/E_Bitmap.h>
#include <E_Util/Graphics/E_PixelAccessor.h>
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Rendering/MeshUtils/MeshUtils.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <cmath>

using namespace Rendering;
using namespace Rendering::MeshUtils;

namespace E_Rendering {
	
//! (static)
EScript::Type * E_MeshBuilder::getTypeObject() {
	// E_MeshBuilder ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_MeshBuilder::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	//!	[ESMF] new Rendering.MeshBuilder( [VertexDescription] )
	ES_CONSTRUCTOR(typeObject,0,1, {
		if(parameter.count()==0)
			return EScript::create(new MeshBuilder);
		else
			return EScript::create(new MeshBuilder(parameter[0].to<const VertexDescription&>(rt)));
	})

	//!	[ESMF] thisEObj MeshBuilder.addIndex( idx )
	ES_MFUN(typeObject,MeshBuilder,"addIndex",1,1,
				(thisObj->addIndex( parameter[0].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.addQuad( idx,idx,idx,idx )
	ES_MFUN(typeObject,MeshBuilder,"addQuad",4,4,
				(thisObj->addQuad( parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt),parameter[3].to<uint32_t>(rt)),thisEObj))
				
	//!	[ESMF] thisEObj MeshBuilder.addSphere( Sphere,inclinationSegments,azimuthSegments )
	//ES_MFUN(typeObject,MeshBuilder,"addSphere",3,3,
	//			(MeshBuilder::addSphere(*thisObj,parameter[0].to<Geometry::Sphere_f>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.addBox( Box )
	//ES_MFUN(typeObject,MeshBuilder,"addBox",1,1,
	//			(MeshBuilder::addBox(*thisObj,parameter[0].to<Geometry::Box>(rt)),thisEObj))


	//!	[ESMF] thisEObj MeshBuilder.addTriangle( idx,idx,idx )
	ES_MFUN(typeObject,MeshBuilder,"addTriangle",3,3,
				(thisObj->addTriangle( parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] Number MeshBuilder.addVertex()
	ES_MFUN(typeObject,MeshBuilder,"addVertex",0,0, thisObj->addVertex())
	
	//!	[ESMF] thisEObj MeshBuilder.addMesh( Mesh )
	ES_MFUN(typeObject,MeshBuilder,"addMesh",1,1,
				(thisObj->addMesh(parameter[0].to<Rendering::Mesh*>(rt)),thisEObj))

	//!	[ESMF] E_Mesh MeshBuilder.buildMesh()
	ES_MFUN(typeObject,MeshBuilder, "buildMesh", 0, 0, thisObj->buildMesh())

	//! int MeshBuilder.getNextIndex()
	ES_MFUN(typeObject,MeshBuilder,"getNextIndex",0,0,thisObj->getNextIndex())
	
	//! Matrix4x4 MeshBuilder.getTransformation()
	ES_MFUN(typeObject,MeshBuilder,"getTransformation",0,0,thisObj->getTransformation())

	//! bool MeshBuilder.isEmpty()
	ES_MFUN(typeObject,MeshBuilder,"isEmpty",0,0,thisObj->isEmpty())
	
	//!	[ESMF] thisEObj MeshBuilder.position( Vec3, [String attribute] )
	ES_MFUN(typeObject,MeshBuilder,"position",1,2,
				( thisObj->position( parameter[0].to<Geometry::Vec3>(rt), parameter[1].toString(VertexAttributeIds::POSITION.toString())),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.position2D(Vec2, [String attribute])
	ES_MFUN(typeObject,MeshBuilder, "position2D", 1, 2,
				 (thisObj->position(parameter[0].to<Geometry::Vec2>(rt), parameter[1].toString(VertexAttributeIds::POSITION.toString())),thisEObj))
				 
 	//!	[ESMF] thisEObj MeshBuilder.position4D(Vec4, [String attribute])
 	ES_MFUN(typeObject,MeshBuilder, "position4D", 1, 2,
 				 (thisObj->position(parameter[0].to<Geometry::Vec4>(rt), parameter[1].toString(VertexAttributeIds::POSITION.toString())),thisEObj))

 	//!	[ESMF] thisEObj MeshBuilder.normal( Vec3, [String attribute] )
 	ES_MFUN(typeObject,MeshBuilder,"normal",1,2,
 				( thisObj->normal( parameter[0].to<Geometry::Vec3>(rt), parameter[1].toString(VertexAttributeIds::NORMAL.toString())),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.color( Color4f | Color4ub, [String attribute] )
	ES_MFUN(typeObject,MeshBuilder,"color",1,2,
				( thisObj->color(parameter[0].to<Util::Color4f>(rt), parameter[1].toString(VertexAttributeIds::COLOR.toString())),thisEObj))
				 
	//!	[ESMF] thisEObj MeshBuilder.texCoord0( Vec2, [String attribute] )
	ES_MFUN(typeObject,MeshBuilder,"texCoord0",1,2,
				( thisObj->texCoord0( parameter[0].to<Geometry::Vec2>(rt), parameter[1].toString(VertexAttributeIds::TEXCOORD0.toString())),thisEObj))
			
	//! [ESMF] thisEObj MeshBuilder.value(Number value, String attribute)
	ES_MFUN(typeObject,MeshBuilder,"value",2,2,(thisObj->value(parameter[0].toFloat(), parameter[1].toString()),thisEObj))

	//! [ESMF] thisEObj MeshBuilder.values(Array values, String attribute)
	ES_MFUNCTION(typeObject,MeshBuilder,"values",2,2,{
		EScript::Array * a=parameter[0].to<EScript::Array*>(rt);
		std::vector<float> values;
		for(auto v : *a)
			values.push_back(v.toFloat());
		thisObj->values(values, parameter[1].toString());
		return thisEObj;
	})
		
	//! [ESMF] thisEObj MeshBuilder.valueUInt(Number value, String attribute)
	ES_MFUN(typeObject,MeshBuilder,"valueUInt",2,2,(thisObj->value(parameter[0].toUInt(), parameter[1].toString()),thisEObj))

	//! [ESMF] thisEObj MeshBuilder.valuesUInt(Array values, String attribute)
	ES_MFUNCTION(typeObject,MeshBuilder,"valuesUInt",2,2,{
		EScript::Array * a=parameter[0].to<EScript::Array*>(rt);
		std::vector<uint32_t> values;
		for(auto v : *a)
			values.push_back(v.toUInt());
		thisObj->values(values, parameter[1].toString());
		return thisEObj;
	})

	//!	[ESMF] thisEObj MeshBuilder.setTransformation( Matrix4x4 | SRT )
	ES_MFUNCTION(typeObject,MeshBuilder,"setTransformation",1,1,{
		E_Geometry::E_Matrix4x4 * em = parameter[0].toType<E_Geometry::E_Matrix4x4>();
		if(em){
			thisObj->setTransformation(em->ref());
		}else{
			thisObj->setTransformation(parameter[0].to<const Geometry::SRT&>(rt));
		}
		return thisEObj;
	})
				

}

}
