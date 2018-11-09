/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2018 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_PlatonicSolids.h"
#include "../Mesh/E_Mesh.h"
#include "../Mesh/E_VertexDescription.h"
#include "E_MeshBuilder.h"

#include <Rendering/MeshUtils/PlatonicSolids.h>
#include <EScript/Basics.h>

namespace E_Rendering {
namespace E_PlatonicSolids {
using namespace Rendering;
using namespace Rendering::MeshUtils;
using namespace Rendering::MeshUtils::PlatonicSolids;

void init(EScript::Namespace * lib) {
	auto mbTypeObject = E_MeshBuilder::getTypeObject();
	
	//! [ESF] (static) Mesh Rendering.createTetrahedron()
	ES_FUN(lib, "createTetrahedron", 0, 1, parameter.count() == 0 ? createTetrahedron() : createTetrahedron(parameter[0].to<VertexDescription&>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addTetrahedron()
	ES_MFUN(mbTypeObject, MeshBuilder, "addTetrahedron", 0, 0, (addTetrahedron(*thisObj), thisEObj))

	//! [ESF] (static) Mesh Rendering.createCube()
	ES_FUN(lib, "createCube", 0, 1, parameter.count() == 0 ? createCube() : createCube(parameter[0].to<VertexDescription&>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addCube()
	ES_MFUN(mbTypeObject, MeshBuilder, "addCube", 0, 0, (addCube(*thisObj), thisEObj))

	//! [ESF] (static) Mesh Rendering.createOctahedron()
	ES_FUN(lib, "createOctahedron", 0, 1, parameter.count() == 0 ? createOctahedron() : createOctahedron(parameter[0].to<VertexDescription&>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addOctahedron()
	ES_MFUN(mbTypeObject, MeshBuilder, "addOctahedron", 0, 0, (addOctahedron(*thisObj), thisEObj))

	//! [ESF] (static) Mesh Rendering.createDodecahedron()
	ES_FUN(lib, "createDodecahedron", 0, 1, parameter.count() == 0 ? createDodecahedron() : createDodecahedron(parameter[0].to<VertexDescription&>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addDodecahedron()
	ES_MFUN(mbTypeObject, MeshBuilder, "addDodecahedron", 0, 0, (addDodecahedron(*thisObj), thisEObj))

	//! [ESF] (static) Mesh Rendering.createIcosahedron()
	ES_FUN(lib, "createIcosahedron", 0, 1, parameter.count() == 0 ? createIcosahedron() : createIcosahedron(parameter[0].to<VertexDescription&>(rt)))
	
	//! [ESMF] thisEObj Rendering.MeshBuilder.addIcosahedron()
	ES_MFUN(mbTypeObject, MeshBuilder, "addIcosahedron", 0, 0, (addIcosahedron(*thisObj), thisEObj))

	//! [ESF] (static) Mesh Rendering.createEdgeSubdivisionSphere(E_Mesh platonicSolid, Number subdivisions)
	ES_FUN(lib, "createEdgeSubdivisionSphere", 2, 2,
			createEdgeSubdivisionSphere(parameter[0].to<Rendering::Mesh*>(rt), parameter[1].to<uint32_t>(rt)))
}

}
}
