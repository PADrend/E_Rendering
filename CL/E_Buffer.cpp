#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Buffer.h"
#include "E_Context.h"
#include "../E_BufferObject.h"

#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Vec4.h>

#include <Rendering/CL/Context.h>
#include <Rendering/Mesh/MeshDataStrategy.h>

#include <Util/TypeConstant.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Buffer::getTypeObject() {
	// E_Buffer ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_Memory::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Buffer::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Buffer::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering;
	using namespace Rendering::CL;

	using namespace E_Geometry;
	using namespace Geometry;
	
	//!	[ESMF] Buffer new Buffer(Context, ReadWrite_t, (BufferObject | size | Array), [TypeConstant])
	ES_CONSTRUCTOR(typeObject,3,4,{
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[3].toUInt(static_cast<uint32_t>(Util::TypeConstant::UINT8)));


		Context* context = parameter[0].to<Context*>(rt);
		auto ebo = parameter[2].toType<E_BufferObject>();
		if(ebo) {
			BufferObject& bo = (**ebo)->get();
			bo.prepare(); // ensure the buffer object exists
			return new E_Buffer(new Buffer(context, static_cast<ReadWrite_t>(parameter[1].to<uint32_t>(rt)), bo));
		}
		auto arr = parameter[2].toType<EScript::Array>();
		if(arr) {
			std::vector<float> values;
			for(auto val : *arr) {
				E_Vec2* v2 = val.toType<E_Vec2>();
				E_Vec3* v3 = val.toType<E_Vec3>();
				E_Vec4* v4 = val.toType<E_Vec4>();
				if(v2) {
					values.push_back((**v2).getX());
					values.push_back((**v2).getY());
				} else if(v3) {
					values.push_back((**v3).getX());
					values.push_back((**v3).getY());
					values.push_back((**v3).getZ());
				} else if(v4) {
					values.push_back((**v4).getX());
					values.push_back((**v4).getY());
					values.push_back((**v4).getZ());
					values.push_back((**v4).getW());
				} else {
					values.push_back(val.toFloat());
				}
			}

			return new E_Buffer(new Buffer(context, values.size() * sizeof(float), static_cast<ReadWrite_t>(parameter[1].to<uint32_t>(rt)), HostPtr_t::AllocAndCopy, values.data()));
		}
		return new E_Buffer(new Buffer(context, parameter[2].to<size_t>(rt) * Util::getNumBytes(type), static_cast<ReadWrite_t>(parameter[1].to<uint32_t>(rt))));
	});

	// Buffer* createSubBuffer(ReadWrite_t readWrite, size_t origin, size_t size)
	//! [ESMF] RESULT Buffer.createSubBuffer(p0,p1,p2)
	ES_MFUN(typeObject,Buffer,"createSubBuffer",3,3,
				EScript::create(thisObj->createSubBuffer(static_cast<ReadWrite_t>(parameter[0].to<uint32_t>(rt)),parameter[1].to<size_t>(rt),parameter[2].to<size_t>(rt))))

	// BufferType_t getType() const
	//! [ESMF] RESULT Buffer.getType()
	ES_MFUN(typeObject,const Buffer,"getType",0,0,
				EScript::create(static_cast<uint32_t>(thisObj->getType())))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

