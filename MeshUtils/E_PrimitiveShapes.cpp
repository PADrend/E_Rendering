/*
 This file is part of the Rendering library.
 Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
 
 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_PrimitiveShapes.h"
#include "../Mesh/E_Mesh.h"
#include "../Mesh/E_VertexDescription.h"
#include "E_MeshBuilder.h"
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Box.h>
#include <E_Geometry/E_Sphere.h>
#include <E_Geometry/E_Rect.h>
#include <E_Util/Graphics/E_PixelAccessor.h>

#include <Rendering/MeshUtils/MeshBuilder.h>
#include <Rendering/MeshUtils/PrimitiveShapes.h>
#include <EScript/Basics.h>

namespace E_Rendering {
namespace E_PrimitiveShapes {

using namespace Geometry;
using namespace Rendering;
using namespace Rendering::MeshUtils;

void init(EScript::Namespace * lib) {
	auto mbTypeObject = E_MeshBuilder::getTypeObject();
	
	//! [ESF] (static) Mesh Rendering.createBox(VertexDescription, Box)
	ES_FUN(lib, "createBox", 2, 2, createBox(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Box>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addBox(Box)
	ES_MFUN(mbTypeObject, MeshBuilder, "addBox", 1, 1, (addBox(*thisObj, parameter[0].to<Box>(rt)), thisEObj))
		
	//! [ESF] (static) Mesh Rendering.createDome(VertexDescription, radius=100, int horiRes = 40, int vertRes = 40, double halfSphereFraction = 1.0, double imagePercentage = 1.0)
	ES_FUN(lib, "createDome", 1, 6, createDome(parameter[0].to<VertexDescription&>(rt), 
		parameter[1].toFloat(100.0),
		parameter[2].toInt(40),
		parameter[3].toInt(40),
		parameter[4].toFloat(1.0),
		parameter[5].toFloat(1.0))
	)
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addDome(radius=100, int horiRes = 40, int vertRes = 40, double halfSphereFraction = 1.0, double imagePercentage = 1.0)
	ES_MFUN(mbTypeObject, MeshBuilder, "addDome", 0, 5, (addDome(*thisObj,
		parameter[0].toFloat(100.0),
		parameter[1].toInt(40),
		parameter[2].toInt(40),
		parameter[3].toFloat(1.0),
		parameter[4].toFloat(1.0)), thisEObj)
	)
		
	//! [ESF] (static) Mesh Rendering.createSphere(VertexDescription, Sphere, inclinationSegments, azimuthSegments)
	ES_FUN(lib, "createSphere", 4, 4, createSphere(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Sphere_f>(rt), parameter[2].toUInt(), parameter[3].toUInt()))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addSphere(Sphere, inclinationSegments, azimuthSegments)
	ES_MFUN(mbTypeObject, MeshBuilder, "addSphere", 3, 3, (addSphere(*thisObj, parameter[0].to<Sphere_f>(rt), parameter[1].toUInt(), parameter[2].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createDiscSector(VertexDescription, float radius, uint8_t numSegments, float angle)
	ES_FUN(lib, "createDiscSector", 3, 4, createDiscSector(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toUInt(), parameter[3].toFloat(360)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addDiscSector(float radius, uint8_t numSegments, float angle)
	ES_MFUN(mbTypeObject, MeshBuilder, "addDiscSector", 2, 3, (addDiscSector(*thisObj, parameter[0].toFloat(), parameter[1].toUInt(), parameter[2].toFloat(360)), thisEObj))
		
	//! [ESF] (static) Mesh Rendering.createRingSector(VertexDescription, float innerRadius, float outerRadius, uint8_t numSegments, float angle)
	ES_FUN(lib, "createRingSector", 4, 5, createRingSector(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt(), parameter[4].toFloat(360)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addRingSector(float innerRadius, float outerRadius, uint8_t numSegments, float angle)
	ES_MFUN(mbTypeObject, MeshBuilder, "addRingSector", 3, 4, (addRingSector(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toUInt(), parameter[3].toFloat(360)), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createCone(VertexDescription, float radius, float height, uint8_t numSegments)
	ES_FUN(lib, "createCone", 4, 4, createCone(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addCone(float radius, float height, uint8_t numSegments)
	ES_MFUN(mbTypeObject, MeshBuilder, "addCone", 3, 3, (addCone(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createConicalFrustum(VertexDescription, float radiusBottom, float radiusTop, float height, uint8_t numSegments)
	ES_FUN(lib, "createConicalFrustum", 5, 5, createConicalFrustum(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toFloat(), parameter[4].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addConicalFrustum(float radiusBottom, float radiusTop, float height, uint8_t numSegments)
	ES_MFUN(mbTypeObject, MeshBuilder, "addConicalFrustum", 4, 4, (addConicalFrustum(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createArrow(VertexDescription, float radius, float length)
	ES_FUN(lib, "createArrow", 3, 3, createArrow(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addArrow(float radius, float length)
	ES_MFUN(mbTypeObject, MeshBuilder, "addArrow", 2, 2, (addArrow(*thisObj, parameter[0].toFloat(), parameter[1].toFloat()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createRectangle(VertexDescription, Rect | [width,height])
	ES_FUNCTION(lib, "createRectangle", 2, 3, {
		if(parameter.count() == 2)
			return new E_Mesh(createRectangle(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Rect_f>(rt)));
		else
			return new E_Mesh(createRectangle(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat()));
	})

	//! [ESMF] thisEObj Rendering.MeshBuilder.addRectangle(Rect | [width,height])
	ES_MFUNCTION(mbTypeObject, MeshBuilder, "addRectangle", 1, 2, {
		if(parameter.count() == 1)
			addRectangle(*thisObj, parameter[0].to<Rect_f>(rt));
		else
			addRectangle(*thisObj, parameter[0].toFloat(), parameter[1].toFloat());
		return thisEObj;
	})
	
	//! [ESF] (static) Mesh Rendering.createGrid(VertexDescription, float width, float height, uint32_t rows, uint32_t columns)
	ES_FUN(lib, "createGrid", 5, 5, createGrid(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt(), parameter[4].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addGrid(float width, float height, uint32_t rows, uint32_t columns)
	ES_MFUN(mbTypeObject, MeshBuilder, "addGrid", 4, 4, (addGrid(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toUInt(), parameter[3].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createHexGrid(VertexDescription, float width, float height, uint32_t rows, uint32_t columns)
	ES_FUN(lib, "createHexGrid", 5, 5, createHexGrid(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt(), parameter[4].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addHexGrid(float width, float height, uint32_t rows, uint32_t columns)
	ES_MFUN(mbTypeObject, MeshBuilder, "addHexGrid", 4, 4, (addHexGrid(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toUInt(), parameter[3].toUInt()), thisEObj))
			
	//! [ESF] (static) Mesh Rendering.createVoxelMesh(VertexDescription, Util.PixelAccessor voxelBitmap, Number depth)
	ES_FUN(lib, "createVoxelMesh", 3, 3, createVoxelMesh(parameter[0].to<VertexDescription&>(rt), parameter[1].to<Util::PixelAccessor&>(rt), parameter[2].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addVoxelMesh(Util.PixelAccessor voxelBitmap, Number depth)
	ES_MFUN(mbTypeObject, MeshBuilder, "addVoxelMesh", 2, 2, (addVoxelMesh(*thisObj, parameter[0].to<Util::PixelAccessor&>(rt), parameter[1].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createTorus(VertexDescription, float innerRadius, float outerRadius, uint32_t majorSegments, uint32_t minorSegments)
	ES_FUN(lib, "createTorus", 5, 5, createTorus(parameter[0].to<VertexDescription&>(rt), parameter[1].toFloat(), parameter[2].toFloat(), parameter[3].toUInt(), parameter[4].toUInt()))

	//! [ESMF] thisEObj Rendering.MeshBuilder.addTorus(float innerRadius, float outerRadius, uint32_t majorSegments, uint32_t minorSegments)
	ES_MFUN(mbTypeObject, MeshBuilder, "addTorus", 4, 4, (addTorus(*thisObj, parameter[0].toFloat(), parameter[1].toFloat(), parameter[2].toUInt(), parameter[3].toUInt()), thisEObj))
	
	//! [ESF] (static) Mesh Rendering.createMeshFromBitmaps(VertexDescription, Util.PixelAccessor depth, [Util.PixelAccessor color, [Util.PixelAccessor normal]])
	ES_FUN(lib, "createMeshFromBitmaps", 2, 4, 
		createMeshFromBitmaps(parameter[0].to<VertexDescription&>(rt), 
			parameter[1].to<Util::PixelAccessor*>(rt), 
			parameter[2].to<Util::PixelAccessor*>(rt, nullptr), 
			parameter[3].to<Util::PixelAccessor*>(rt, nullptr)
		)
	)

	//! [ESMF] thisEObj Rendering.MeshBuilder.addMeshFromBitmaps(Util.PixelAccessor depth, [Util.PixelAccessor color, [Util.PixelAccessor normal]])
	ES_MFUN(mbTypeObject, MeshBuilder, "addMeshFromBitmaps", 1, 3, (
		addMeshFromBitmaps(*thisObj, 
			parameter[0].to<Util::PixelAccessor*>(rt), 
			parameter[1].to<Util::PixelAccessor*>(rt, nullptr), 
			parameter[2].to<Util::PixelAccessor*>(rt, nullptr)
		), thisEObj)
	)
}

}
}
