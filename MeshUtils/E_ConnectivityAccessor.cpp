
#include "E_ConnectivityAccessor.h"

#include "E_TriangleAccessor.h"
#include "../Mesh/E_Mesh.h"

#include <E_Geometry/E_Triangle.h>
#include <E_Geometry/E_Vec3.h>

#include <Rendering/Mesh/VertexAttributeAccessors.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{


//! (static)
EScript::Type * E_ConnectivityAccessor::getTypeObject() {
	// E_ConnectivityAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_ConnectivityAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_ConnectivityAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::MeshUtils;

	//! [ESF] ConnectivityAccessor Rendering.MeshUtils.ConnectivityAccessor.create(Mesh,name)
	ES_FUN(typeObject,"create",1,1,ConnectivityAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)))

	//! [ESMF] Geometry::Vec3 ConnectivityAccessor.getVertex(vIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getVertex",1,1,
				thisObj->getVertex(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Array ConnectivityAccessor.getTriangle(tIndex)
	ES_MFUNCTION(typeObject,const ConnectivityAccessor,"getTriangle",1,1, {
		auto tri = thisObj->getTriangle(parameter[0].to<uint32_t>(rt));
		auto arr = EScript::Array::create();
		arr->pushBack(EScript::create(std::get<0>(tri)));
		arr->pushBack(EScript::create(std::get<1>(tri)));
		arr->pushBack(EScript::create(std::get<2>(tri)));
		return arr;
	})

	//! [ESMF] Number ConnectivityAccessor.getCorner(vIndex, tIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getCorner",2,2,
				thisObj->getCorner(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getVertexCorner(vIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getVertexCorner",1,1,
				thisObj->getVertexCorner(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getTriangleCorner(tIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getTriangleCorner",1,1,
				thisObj->getTriangleCorner(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getCornerVertex(cIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getCornerVertex",1,1,
				thisObj->getCornerVertex(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getCornerTriangle(cIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getCornerTriangle",1,1,
				thisObj->getCornerTriangle(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getNextVertexCorner(cIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getNextVertexCorner",1,1,
				thisObj->getNextVertexCorner(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.getNextTriangleCorner(cIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"getNextTriangleCorner",1,1,
				thisObj->getNextTriangleCorner(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] Array ConnectivityAccessor.getVertexAdjacentTriangles(vIndex)
	ES_MFUNCTION(typeObject,const ConnectivityAccessor,"getVertexAdjacentTriangles",1,1, {
		auto tri = thisObj->getVertexAdjacentTriangles(parameter[0].to<uint32_t>(rt));
		auto arr = EScript::Array::create();
		for(auto t : tri)
			arr->pushBack(EScript::create(t));
		return arr;
	})

	//! [ESMF] Array ConnectivityAccessor.getVertexAdjacentVertices(vIndex)
	ES_MFUNCTION(typeObject,const ConnectivityAccessor,"getVertexAdjacentVertices",1,1, {
		auto verts = thisObj->getVertexAdjacentVertices(parameter[0].to<uint32_t>(rt));
		auto arr = EScript::Array::create();
		for(auto v : verts)
			arr->pushBack(EScript::create(v));
		return arr;
	})

	//! [ESMF] Array ConnectivityAccessor.getAdjacentTriangles(tIndex)
	ES_MFUNCTION(typeObject,const ConnectivityAccessor,"getAdjacentTriangles",1,1, {
		auto tri = thisObj->getAdjacentTriangles(parameter[0].to<uint32_t>(rt));
		auto arr = EScript::Array::create();
		for(auto t : tri)
			arr->pushBack(EScript::create(t));
		return arr;
	})

	//! [ESMF] Number ConnectivityAccessor.isBorderEdge(vIndex1, vIndex2)
	ES_MFUN(typeObject,const ConnectivityAccessor,"isBorderEdge",2,2,
				thisObj->isBorderEdge(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)))

	//! [ESMF] Number ConnectivityAccessor.isBorderTriangle(tIndex)
	ES_MFUN(typeObject,const ConnectivityAccessor,"isBorderTriangle",1,1,
				thisObj->isBorderTriangle(parameter[0].to<uint32_t>(rt)))
}

}

