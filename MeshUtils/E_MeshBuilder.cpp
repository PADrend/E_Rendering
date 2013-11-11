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
#include <E_Util/Graphics/E_Color.h>
#include <E_Util/Graphics/E_Bitmap.h>
#include <Rendering/Mesh/VertexAttributeIds.h>
#include <Rendering/MeshUtils/MeshUtils.h>
#include <EScript/Utils/DeprecatedMacros.h>
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

	// (static)

	//!	[ESF] (static) Mesh Rendering.MeshBuilder.createArrow(Number radius, Number length)
	ES_FUN(typeObject,"createArrow", 2, 2, (MeshBuilder::createArrow(parameter[0].toFloat(), parameter[1].toFloat())))

	//! [ESF] (static) Mesh Rendering.MeshBuilder.createCone(Number radius, Number height, Number segments)
	ES_FUN(typeObject,"createCone", 3, 3, (MeshBuilder::createCone(parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].to<uint32_t>(rt))))

	//! [ESF] (static) Mesh Rendering.MeshBuilder.createConicalFrustum(Number radiusBottom, Number radiusTop, Number height, Number segments)
	ES_FUN(typeObject,"createConicalFrustum", 4, 4, (MeshBuilder::createConicalFrustum(parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].to<uint32_t>(rt))))

	//! [ESF] (static) Mesh Rendering.MeshBuilder.createDiscSector(Number radius, Number segments, Number angle)
	ES_FUNCTION2(typeObject,"createDiscSector",2,3,{
		if (parameter.count() == 2) {
			return EScript::create(MeshBuilder::createDiscSector(parameter[0].toFloat(), parameter[1].to<uint32_t>(rt)));
		} else {
			return EScript::create(MeshBuilder::createDiscSector(parameter[0].toFloat(), parameter[1].to<uint32_t>(rt), parameter[2].toFloat()));
		}
	})

	/*! [ESF] (static) Mesh Rendering.MeshBuilder.createDome(radius=100,
								int horiRes = 40, int vertRes = 40,
								double halfSphereFraction = 1.0,
								double imagePercentage = 1.0) */
	ES_FUN(typeObject,"createDome",0,5,(MeshBuilder::createDome(
				parameter[0].toFloat(100.0),
				parameter[1].toInt(40),
				parameter[2].toInt(40),
				parameter[3].toFloat(1.0),
				parameter[4].toFloat(1.0))))

	//!	[ESF] (static) Mesh Rendering.MeshBuilder.createRectangle([VertexDescription],width,height)
	ES_FUNCTION2(typeObject, "createRectangle", 2, 3, {
		if (parameter.count() == 3) {
			return EScript::create(MeshBuilder::createRectangle(parameter[0].to<const VertexDescription&>(rt),parameter[1].toFloat(), parameter[2].toFloat()));
		} else {
			Rendering::VertexDescription vertexDescription;
			vertexDescription.appendPosition3D();
			vertexDescription.appendNormalFloat();
			vertexDescription.appendColorRGBFloat();
			vertexDescription.appendTexCoord();
			return EScript::create(MeshBuilder::createRectangle(vertexDescription,parameter[0].toFloat(), parameter[1].toFloat()));
		}
	})

	//! [ESF] (static) Mesh Rendering.MeshBuilder.createRingSector(Number innerRadius, Number outerRadius, Number segments, Number angle)
	ES_FUNCTION2(typeObject,"createRingSector",3,4,{
		if (parameter.count() == 3) {
			return EScript::create(MeshBuilder::createRingSector(parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].to<uint32_t>(rt)));
		} else {
			return EScript::create(MeshBuilder::createRingSector(parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].to<uint32_t>(rt), parameter[3].toFloat()));
		}
	})

	//!	[ESF] (static) Mesh Rendering.MeshBuilder.createSphere(Number, Number)
	ES_FUNCTION2(typeObject, "createSphere", 2, 2, {
		Rendering::VertexDescription vertexDescription;
		vertexDescription.appendPosition3D();
		vertexDescription.appendNormalFloat();
		vertexDescription.appendColorRGBFloat();
		vertexDescription.appendTexCoord();
		return EScript::create(MeshBuilder::createSphere(vertexDescription, parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)));
	})

	// EXPERIMENTAL!!!!!!!!!!!!!!
	//!	[ESMF] new Rendering.MeshBuilder.createMeshFromBitmaps(Util.Bitmap depth[, Util.Bitmap color[, Util.Bitmap normals]])
	ES_FUNCTION2(typeObject,"createMeshFromBitmaps",1,3,{
		Util::Reference<Util::Bitmap> depth = parameter[0].to<Util::Reference<Util::Bitmap>>(rt);
		Util::Reference<Util::Bitmap> color;
		Util::Reference<Util::Bitmap> normal;

		Rendering::VertexDescription vertexDescription;
		vertexDescription.appendPosition3D();
		if(parameter.count()>1){
			color = parameter[1].to<Util::Reference<Util::Bitmap>>(rt);
			vertexDescription.appendColorRGBAByte();
		}
		if(parameter.count()>2){
			normal = parameter[2].to<Util::Reference<Util::Bitmap>>(rt);
			vertexDescription.appendNormalByte();
		}
		Mesh * m=MeshBuilder::createMeshFromBitmaps(vertexDescription,
													std::move(depth), 
													std::move(color), 
													std::move(normal));
		if(m)
			return EScript::create(m);
		else return nullptr;
	})

	// ----

	// --------------------------------------------------

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

	//!	[ESMF] thisEObj MeshBuilder.addTriangle( idx,idx,idx )
	ES_MFUN(typeObject,MeshBuilder,"addTriangle",3,3,
				(thisObj->addTriangle( parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] Number MeshBuilder.addVertex( [Vec3f pos ,Vec3 normal, float r, g, b, a, float u, v )
	ES_MFUNCTION(typeObject,MeshBuilder,"addVertex",0,8,{
		uint32_t index=0;
		if(parameter.count() == 8) {
			index=thisObj->addVertex(
					parameter[0].to<const Geometry::Vec3&>(rt), // pos
					parameter[1].to<const Geometry::Vec3&>(rt), // normal
					parameter[2].toFloat(),	parameter[3].toFloat(),	parameter[4].toFloat(), parameter[5].toFloat(),	 // color
					parameter[6].toFloat(),	parameter[7].toFloat()); // tex0
		}
		else {
			assertParamCount(rt,parameter.count(),0,0);
			index=thisObj->addVertex();
		}
		return index;
	})

	//!	[ESMF] E_Mesh MeshBuilder.buildMesh()
	ES_MFUN(typeObject,MeshBuilder, "buildMesh", 0, 0, thisObj->buildMesh())

	//!	[ESMF] thisEObj MeshBuilder.color( Color4f | Color4ub )
	ES_MFUNCTION(typeObject,MeshBuilder,"color",1,1,{
		thisObj->color(parameter[0].to<Util::Color4f>(rt));
		return thisEObj;
	})

	//! int MeshBuilder.getNextIndex()
	ES_MFUN(typeObject,MeshBuilder,"getNextIndex",0,0,thisObj->getNextIndex())
	
	//! Matrix4x4 MeshBuilder.getTransformation()
	ES_MFUN(typeObject,MeshBuilder,"getTransformation",0,0,thisObj->getTransformation())

	//! bool MeshBuilder.isEmpty()
	ES_MFUN(typeObject,MeshBuilder,"isEmpty",0,0,thisObj->isEmpty())

	//!	[ESMF] thisEObj MeshBuilder.normal( Vec3 )
	ES_MFUN(typeObject,MeshBuilder,"normal",1,1,
				( thisObj->normal( parameter[0].to<const Geometry::Vec3&>(rt)),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.position( Vec3 )
	ES_MFUN(typeObject,MeshBuilder,"position",1,1,
				( thisObj->position( parameter[0].to<const Geometry::Vec3&>(rt)),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.position2D(Vec2)
	ES_MFUN(typeObject,MeshBuilder, "position2D", 1, 1,
				 (thisObj->position(parameter[0].to<const Geometry::Vec2&>(rt)),thisEObj))

	//!	[ESMF] thisEObj MeshBuilder.texCoord0( Vec2 )
	ES_MFUN(typeObject,MeshBuilder,"texCoord0",1,1,
				( thisObj->texCoord0( parameter[0].to<const Geometry::Vec2&>(rt)),thisEObj))

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
