/*
 This file is part of the Rendering library.
 Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
 
 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_WireShapes.h"
#include "../Mesh/E_Mesh.h"
#include "../Mesh/E_VertexDescription.h"
#include "E_MeshBuilder.h"
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Box.h>
#include <E_Geometry/E_Sphere.h>
#include <E_Geometry/E_Rect.h>
#include <E_Geometry/E_Frustum.h>
#include <E_Util/Graphics/E_PixelAccessor.h>

#include <Rendering/MeshUtils/MeshBuilder.h>
#include <Rendering/MeshUtils/WireShapes.h>
#include <EScript/Basics.h>

namespace E_Rendering {
namespace E_WireShapes {

using namespace Geometry;
using namespace Rendering;
using namespace Rendering::MeshUtils;
using namespace Rendering::MeshUtils::WireShapes;

void init(EScript::Namespace * lib) {
	auto mbTypeObject = E_MeshBuilder::getTypeObject();
  
	//! @name MeshBuilder
	//! @{
  
	//! [ESF] (static) Mesh Rendering.createWireBox(VertexDescription, Box)
	ES_FUN(lib, "createWireBox", 2, 2, createWireBox(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Box>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addWireBox(Box)
	ES_MFUN(mbTypeObject, MeshBuilder, "addWireBox", 1, 1, (addWireBox(*thisObj, parameter[0].to<Box>(rt)), thisEObj))
			
	//! [ESF] (static) Mesh Rendering.createWireSphere(VertexDescription, Sphere, segments)
	ES_FUN(lib, "createWireSphere", 3, 3, createWireSphere(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Sphere_f>(rt), parameter[2].toUInt()))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addWireSphere(Sphere, segments)
	ES_MFUN(mbTypeObject, MeshBuilder, "addWireSphere", 2, 2, (addWireSphere(*thisObj, parameter[0].to<Sphere_f>(rt), parameter[1].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createWireCircle(VertexDescription, float radius, uint8_t numSegments)
	ES_FUN(lib, "createWireCircle", 3, 3, createWireCircle(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toUInt()))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addWireCircle(float radius, uint8_t numSegments, float angle)
	ES_MFUN(mbTypeObject, MeshBuilder, "addWireCircle", 2, 2, (addWireCircle(*thisObj, parameter[0].toFloat(), parameter[1].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createWireFrustum(VertexDescription, Frustum)
	ES_FUN(lib, "createWireFrustum", 2, 2, createWireFrustum(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Frustum&>(rt)))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addWireFrustum(Frustum)
	ES_MFUN(mbTypeObject, MeshBuilder, "addWireFrustum", 1, 1, (addWireFrustum(*thisObj, parameter[0].to<Frustum&>(rt)), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createLine(VertexDescription, float radius, float length)
	ES_FUN(lib, "createLine", 3, 3, createLine(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Vec3&>(rt), parameter[2].to<Vec3&>(rt)))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addLine(float radius, float length)
	ES_MFUN(mbTypeObject, MeshBuilder, "addLine", 2, 2, (addLine(*thisObj, parameter[0].to<Vec3&>(rt), parameter[1].to<Vec3&>(rt)), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createWireRectangle(VertexDescription, Rect | [width,height])
	ES_FUNCTION(lib, "createWireRectangle", 2, 3, {
		if(parameter.count() == 2)
			return new E_Mesh(createWireRectangle(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Rect_f>(rt)));
		else
			return new E_Mesh(createWireRectangle(parameter[0].to<VertexDescription&>(rt), {0.0f, 0.0f, parameter[1].toFloat(), parameter[2].toFloat()}));
	})

	//! [ESMF] thisEObj Rendering.MeshBuilder.addWireRectangle(Rect | [width,height])
	ES_MFUNCTION(mbTypeObject, MeshBuilder, "addWireRectangle", 1, 2, {
		if(parameter.count() == 1)
			addWireRectangle(*thisObj, parameter[0].to<Rect_f>(rt));
		else
			addWireRectangle(*thisObj, {0.0f, 0.0f, parameter[0].toFloat(), parameter[1].toFloat()});
		return thisEObj;
	})
  
  //! @}
}

}
}
