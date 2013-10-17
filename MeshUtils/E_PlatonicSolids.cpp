/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_PlatonicSolids.h"
#include "../Mesh/E_Mesh.h"

#include <Rendering/MeshUtils/PlatonicSolids.h>
#include <EScript/Basics.h>

namespace E_Rendering {
namespace E_PlatonicSolids {

void init(EScript::Namespace * lib) {
	//! [ESF] (static) Mesh Rendering.createTetrahedron()
	ES_FUN(lib, "createTetrahedron", 0, 0, Rendering::MeshUtils::PlatonicSolids::createTetrahedron())

	//! [ESF] (static) Mesh Rendering.createCube()
	ES_FUN(lib, "createCube", 0, 0, Rendering::MeshUtils::PlatonicSolids::createCube())

	//! [ESF] (static) Mesh Rendering.createOctahedron()
	ES_FUN(lib, "createOctahedron", 0, 0, Rendering::MeshUtils::PlatonicSolids::createOctahedron())

	//! [ESF] (static) Mesh Rendering.createDodecahedron()
	ES_FUN(lib, "createDodecahedron", 0, 0, Rendering::MeshUtils::PlatonicSolids::createDodecahedron())

	//! [ESF] (static) Mesh Rendering.createIcosahedron(
	ES_FUN(lib, "createIcosahedron", 0, 0, Rendering::MeshUtils::PlatonicSolids::createIcosahedron())

	//! [ESF] (static) Mesh Rendering.createEdgeSubdivisionSphere(E_Mesh platonicSolid, Number subdivisions)
	ES_FUN(lib, "createEdgeSubdivisionSphere", 2, 2,
			Rendering::MeshUtils::PlatonicSolids::createEdgeSubdivisionSphere(parameter[0].to<Rendering::Mesh*>(rt), parameter[1].to<uint32_t>(rt)))
}

}
}
