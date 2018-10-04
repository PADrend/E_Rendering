/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "ELibRendering.h"
#include <sstream>

#include "E_BufferObject.h"
#include "E_Draw.h"
#include "E_FBO.h"
#include "E_OcclusionQuery.h"
#include "E_ParameterStructs.h"
#include "E_RenderingContext.h"
#include "E_StatisticsQuery.h"
#include "E_TextRenderer.h"
#include "Mesh/E_Mesh.h"
#include "Mesh/E_VertexAccessor.h"
#include "Mesh/E_VertexAttributeAccessors.h"
#include "Mesh/E_VertexAttribute.h"
#include "Mesh/E_VertexDescription.h"
#include "MeshUtils/E_MeshBuilder.h"
#include "MeshUtils/E_MeshUtils.h"
#include "MeshUtils/E_PlatonicSolids.h"
#include "MeshUtils/E_PrimitiveShapes.h"
#include "MeshUtils/E_TriangleAccessor.h"
#include "MeshUtils/E_ConnectivityAccessor.h"
#include "Shader/E_Shader.h"
#include "Shader/E_Uniform.h"
#include "Texture/E_Texture.h"
#include "Texture/E_TextureUtils.h"

#include <Rendering/Mesh/MeshDataStrategy.h>
#include <Rendering/Helper.h>
#include <Rendering/Serialization/StreamerXYZ.h>
#include <Rendering/Serialization/Serialization.h>
#include <Rendering/Texture/Texture.h>
#include <Rendering/Texture/TextureUtils.h>
#include <Util/IO/FileName.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <iostream>

namespace E_Rendering{

using namespace EScript;
using namespace Rendering;
//using namespace E_Geometry;
using namespace Geometry;
//using namespace E_Util;

//! E_MeshDataStrategy ---> EScript::ReferenceObject<MeshDataStrategy *>
struct E_MeshDataStrategy:public EScript::ReferenceObject<MeshDataStrategy *>{
	ES_PROVIDES_TYPE_NAME(MeshDataStrategy)
public:

	E_MeshDataStrategy(MeshDataStrategy * s):EScript::ReferenceObject<MeshDataStrategy *>(s){}
	virtual ~E_MeshDataStrategy()			{}

	MeshDataStrategy * get() const	{	return ref();	}
};

// ---------------------------------------------------------

void init(EScript::Namespace * globals) {
	Namespace * lib=new Namespace();
	declareConstant(globals,"Rendering",lib);

	E_BufferObject::init(*lib);
	E_Draw::init(*lib);
	E_FBO::init(*lib);
	E_MeshBuilder::init(*lib);
	E_Mesh::init(*lib);
	E_OcclusionQuery::init(*lib);
	E_ParameterStructs::init(*lib);
	E_PlatonicSolids::init(lib);
	E_PrimitiveShapes::init(lib);
	E_RenderingContext::init(*lib);
	E_StatisticsQuery::init(*lib);
	E_Shader::init(*lib);
	E_TextRenderer::init(*lib);
	E_Texture::init(*lib);
	E_Uniform::init(*lib);
	E_VertexAccessor::init(*lib);
	E_VertexAttributeAccessor::init(*lib);
	E_VertexAttribute::init(*lib);
	E_VertexDescription::init(*lib);
	E_TriangleAccessor::init(*lib);
	E_ConnectivityAccessor::init(*lib);

	initMeshUtils(lib);
	initTextureUtils(lib);

	// -----------------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------------
	// MeshDataStrategy

	//! [ESF] void Rendering.setDefaultMeshDataStrategy(MeshDataStrategy)
	ES_FUN(lib,"setDefaultMeshDataStrategy",1,1,(
		MeshDataStrategy::setDefaultStrategy(EScript::assertType<E_MeshDataStrategy>(rt,parameter[0])->get()),EScript::create(nullptr)))

	//! MeshDataStrategy Rendering.getDefaultMeshDataStrategy()
	ES_FUN(lib,"getDefaultMeshDataStrategy",0,0,new E_MeshDataStrategy(MeshDataStrategy::getDefaultStrategy()))

	//! MeshDataStrategy Rendering.getPureLocalStrategy()
	ES_FUN(lib,"getPureLocalStrategy",0,0,
			new E_MeshDataStrategy(SimpleMeshDataStrategy::getPureLocalStrategy()))

	//! MeshDataStrategy Rendering.getDebugStrategy()
	ES_FUN(lib,"getDebugStrategy",0,0,
			new E_MeshDataStrategy(SimpleMeshDataStrategy::getDebugStrategy()))

	//! MeshDataStrategy Rendering.getStaticDrawReleaseLocalStrategy()
	ES_FUN(lib,"getStaticDrawReleaseLocalStrategy",0,0,
			new E_MeshDataStrategy(SimpleMeshDataStrategy::getStaticDrawReleaseLocalStrategy()))

	//! MeshDataStrategy Rendering.getStaticDrawPreserveLocalStrategy()
	ES_FUN(lib,"getStaticDrawPreserveLocalStrategy",0,0,
			new E_MeshDataStrategy(SimpleMeshDataStrategy::getStaticDrawPreserveLocalStrategy()))

	//! MeshDataStrategy Rendering.getDynamicVertexStrategy()
	ES_FUN(lib,"getDynamicVertexStrategy",0,0,
			new E_MeshDataStrategy(SimpleMeshDataStrategy::getDynamicVertexStrategy()))

	// -----------------------------------------------------------------------------------------------------
	// -----------------------------------------------------------------------------------------------------

	//! [ESF] Void Rendering.enableGLErrorChecking()
	ES_FUN(lib, "enableGLErrorChecking", 0, 0, (Rendering::enableGLErrorChecking(), EScript::create(nullptr)))

	//! [ESF] Void Rendering.disableGLErrorChecking()
	ES_FUN(lib, "disableGLErrorChecking", 0, 0, (Rendering::disableGLErrorChecking(), EScript::create(nullptr)))

	//! [ESF] Void Rendering.checkGLError()
	ES_FUN(lib, "checkGLError", 0, 0, (Rendering::checkGLError(rt.getCurrentFile().c_str(), rt.getCurrentLine()), EScript::create(nullptr)))

	//! [ESF] String Rendering.getGLTypeString(Number)
	ES_FUN(lib, "getGLTypeString", 1, 1, Rendering::getGLTypeString(parameter[0].to<uint32_t>(rt)))

	//! [ESF] Void Rendering.outputGLInformation()
	ES_FUN(lib, "outputGLInformation", 0, 0, (Rendering::outputGLInformation(std::cout), EScript::create(nullptr)))

	//! [ESF] Number Rendering.readDepthValue(Number x, Number y)
	ES_FUN(lib, "readDepthValue", 2, 2, Rendering::readDepthValue(parameter[0].to<int32_t>(rt), parameter[1].to<int32_t>(rt)))

	//! [ESF] Void Rendering.enableDebugOutput()
	ES_FUN(lib, "enableDebugOutput", 0, 0, (Rendering::enableDebugOutput(), EScript::create(nullptr)))

	//! [ESF] Void Rendering.disableDebugOutput()
	ES_FUN(lib, "disableDebugOutput", 0, 0, (Rendering::disableDebugOutput(), EScript::create(nullptr)))

	//! [ESF] Void Rendering.clusterPoints(file,count)
	ES_FUN(lib, "clusterPoints", 2, 2, (Rendering::StreamerXYZ::clusterPoints(Util::FileName(parameter[0].toString()),
																					parameter[1].to<uint32_t>(rt)),EScript::create(nullptr)))
}
}
