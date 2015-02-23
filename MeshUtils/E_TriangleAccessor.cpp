
#include "E_TriangleAccessor.h"

#include "../Mesh/E_Mesh.h"

#include <E_Geometry/E_Triangle.h>

#include <Rendering/Mesh/VertexAttributeAccessors.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{


//! (static)
EScript::Type * E_TriangleAccessor::getTypeObject() {
	// E_TriangleAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_TriangleAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_TriangleAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::MeshUtils;

	//! [ESF] TriangleAccessor Rendering.MeshUtils.TriangleAccessor.create(Mesh,name)
	ES_FUN(typeObject,"create",1,1,TriangleAccessor::create(
					parameter[0].to<Rendering::Mesh*>(rt)))

	//! [ESMF] Geometry.Triangle<Geometry::Vec3> TriangleAccessor.getTriangle(index)
	ES_MFUN(typeObject,const TriangleAccessor,"getTriangle",1,1,
				thisObj->getTriangle(parameter[0].to<uint32_t>(rt)))

	//! [ESMF] thisEObj TriangleAccessor.setTriangle(index,Geometry.Triangle<Geometry::Vec3>)
	ES_MFUN(typeObject,TriangleAccessor,"setTriangle",2,2,
				(thisObj->setTriangle(parameter[0].to<uint32_t>(rt),parameter[1].to<Geometry::Triangle3>(rt)),thisEObj))

	//! [ESMF] Array TriangleAccessor.getTriangle(index)
	ES_MFUNCTION(typeObject,const TriangleAccessor,"getIndices",1,1, {
		auto tri = thisObj->getIndices(parameter[0].to<uint32_t>(rt));
		auto arr = EScript::Array::create();
		arr->pushBack(EScript::create(std::get<0>(tri)));
		arr->pushBack(EScript::create(std::get<1>(tri)));
		arr->pushBack(EScript::create(std::get<2>(tri)));
		return arr;
	})

}

}

