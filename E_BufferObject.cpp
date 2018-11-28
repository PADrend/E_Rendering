/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_BufferObject.h"
#include "E_Geometry/E_Vec3.h"
#include "E_Geometry/E_Vec4.h"
#include "E_Geometry/E_Matrix4x4.h"

#include <Util/TypeConstant.h>
#include <Util/Macros.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace EScript {
template<> inline uint8_t convertTo<uint8_t>(Runtime& rt,ObjPtr src) { return static_cast<uint8_t>(convertTo<double>(rt,src)); }
template<> inline int8_t convertTo<int8_t>(Runtime& rt,ObjPtr src) { return static_cast<uint8_t>(convertTo<double>(rt,src)); }
} 

namespace E_Rendering {
		
//! (static)
EScript::Type * E_BufferObject::getTypeObject() {
	// E_BufferObject ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

template<typename T>
EScript::Array * downloadToArray(EScript::Runtime& rt, Rendering::BufferObject& bo, uint32_t target, size_t count, size_t offset) {
	EScript::Array * a = EScript::Array::create();
	auto data = bo.downloadData<T>(target, count, offset);
	for(T& v : data)
			a->pushBack(EScript::Number::create(v));
	return a;
}

template<typename T>
void uploadArray(EScript::Runtime& rt, Rendering::BufferObject& bo, EScript::Array* a, uint32_t target, uint32_t hint) {
	std::vector<T> tmp;
	for(auto value : *a)
		tmp.push_back(value.to<T>(rt));
	auto data = reinterpret_cast<const uint8_t*>(tmp.data());
	auto size = tmp.size() * sizeof(T);
	bo.uploadData(target, data, size, hint);
}

template<>
void uploadArray<Geometry::Matrix4x4>(EScript::Runtime& rt, Rendering::BufferObject& bo, EScript::Array* a, uint32_t target, uint32_t hint) {
	std::vector<Geometry::Matrix4x4> tmp;
	for(auto value : *a)
		tmp.push_back(value.to<Geometry::Matrix4x4>(rt).getTransposed());
	auto data = reinterpret_cast<const uint8_t*>(tmp.data());
	auto size = tmp.size() * sizeof(Geometry::Matrix4x4);
	bo.uploadData(target, data, size, hint);
}

template<typename T>
void uploadSubArray(EScript::Runtime& rt, Rendering::BufferObject& bo, EScript::Array* a, uint32_t target, size_t offset) {
	std::vector<T> tmp;
	for(auto value : *a)
		tmp.push_back(value.to<T>(rt));
	auto data = reinterpret_cast<const uint8_t*>(tmp.data());
	auto size = tmp.size() * sizeof(T);
	bo.uploadSubData(target, data, size, offset);
}

template<>
void uploadSubArray<Geometry::Matrix4x4>(EScript::Runtime& rt, Rendering::BufferObject& bo, EScript::Array* a, uint32_t target, size_t offset) {
	std::vector<Geometry::Matrix4x4> tmp;
	for(auto value : *a)
		tmp.push_back(value.to<Geometry::Matrix4x4>(rt).getTransposed());
	auto data = reinterpret_cast<const uint8_t*>(tmp.data());
	auto size = tmp.size() * sizeof(Geometry::Matrix4x4);
	bo.uploadSubData(target, data, size, offset);
}

//! initMembers
void E_BufferObject::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	using namespace Rendering;	
	using namespace EScript;
	
	declareConstant(&lib,"TARGET_ARRAY_BUFFER", Number::create(BufferObject::TARGET_ARRAY_BUFFER));
	declareConstant(&lib,"TARGET_ATOMIC_COUNTER_BUFFER", Number::create(BufferObject::TARGET_ATOMIC_COUNTER_BUFFER));
	declareConstant(&lib,"TARGET_COPY_READ_BUFFER", Number::create(BufferObject::TARGET_COPY_READ_BUFFER));
	declareConstant(&lib,"TARGET_COPY_WRITE_BUFFER", Number::create(BufferObject::TARGET_COPY_WRITE_BUFFER));
	declareConstant(&lib,"TARGET_DISPATCH_INDIRECT_BUFFER", Number::create(BufferObject::TARGET_DISPATCH_INDIRECT_BUFFER));
	declareConstant(&lib,"TARGET_DRAW_INDIRECT_BUFFER", Number::create(BufferObject::TARGET_DRAW_INDIRECT_BUFFER));
	declareConstant(&lib,"TARGET_ELEMENT_ARRAY_BUFFER", Number::create(BufferObject::TARGET_ELEMENT_ARRAY_BUFFER));
	declareConstant(&lib,"TARGET_PIXEL_PACK_BUFFER", Number::create(BufferObject::TARGET_PIXEL_PACK_BUFFER));
	declareConstant(&lib,"TARGET_PIXEL_UNPACK_BUFFER", Number::create(BufferObject::TARGET_PIXEL_UNPACK_BUFFER));
	declareConstant(&lib,"TARGET_QUERY_BUFFER", Number::create(BufferObject::TARGET_QUERY_BUFFER));
	declareConstant(&lib,"TARGET_SHADER_STORAGE_BUFFER", Number::create(BufferObject::TARGET_SHADER_STORAGE_BUFFER));
	declareConstant(&lib,"TARGET_TEXTURE_BUFFER", Number::create(BufferObject::TARGET_TEXTURE_BUFFER));
	declareConstant(&lib,"TARGET_TRANSFORM_FEEDBACK_BUFFER", Number::create(BufferObject::TARGET_TRANSFORM_FEEDBACK_BUFFER));
	declareConstant(&lib,"TARGET_UNIFORM_BUFFER", Number::create(BufferObject::TARGET_UNIFORM_BUFFER));
		
	declareConstant(&lib,"USAGE_STREAM_DRAW", Number::create(BufferObject::USAGE_STREAM_DRAW));
	declareConstant(&lib,"USAGE_STREAM_READ", Number::create(BufferObject::USAGE_STREAM_READ));
	declareConstant(&lib,"USAGE_STREAM_COPY", Number::create(BufferObject::USAGE_STREAM_COPY));
	declareConstant(&lib,"USAGE_STATIC_DRAW", Number::create(BufferObject::USAGE_STATIC_DRAW));
	declareConstant(&lib,"USAGE_STATIC_READ", Number::create(BufferObject::USAGE_STATIC_READ));
	declareConstant(&lib,"USAGE_STATIC_COPY", Number::create(BufferObject::USAGE_STATIC_COPY));
	declareConstant(&lib,"USAGE_DYNAMIC_DRAW", Number::create(BufferObject::USAGE_DYNAMIC_DRAW));
	declareConstant(&lib,"USAGE_DYNAMIC_READ", Number::create(BufferObject::USAGE_DYNAMIC_READ));
	declareConstant(&lib,"USAGE_DYNAMIC_COPY", Number::create(BufferObject::USAGE_DYNAMIC_COPY));

	//! [ESMF] BufferObject new BufferObject()
	ES_CTOR(typeObject,0,0,new CountedBufferObject(BufferObject()));

	//! [ESMF] Bool BufferObject.isValid()
	ES_MFUN(typeObject,BufferObject,"isValid",0,0,thisObj->isValid())

	//! [ESMF] thisEObj BufferObject.swap(E_BufferObject)
	ES_MFUN(typeObject,BufferObject,"swap",0,0,(thisObj->swap(*parameter[1].to<BufferObject*>(rt)),thisEObj))

	//! [ESMF] thisEObj BufferObject.prepare()
	ES_MFUN(typeObject,BufferObject,"prepare",0,0,(thisObj->prepare(), thisEObj))

	//! [ESMF] thisEObj BufferObject.destroy()
	ES_MFUN(typeObject,BufferObject,"destroy",0,0,(thisObj->destroy(), thisEObj))

	//! [ESMF] thisEObj BufferObject.allocateData(bufferTarget, numBytes, usageHint)
	ES_MFUN(typeObject,BufferObject,"allocateData",3,3,(thisObj->allocateData<uint8_t>(parameter[0].toUInt(), parameter[1].toUInt(), parameter[2].toUInt()), thisEObj))
	
	//! [ESF] thisEObj BufferObject.uploadData(bufferTarget, Array of float/Vec3/Vec4/Matrix4x4, usageHint, [type])
	ES_MFUNCTION(typeObject,BufferObject,"uploadData", 3, 4, {
		uint32_t bufferTarget = parameter[0].toUInt();
		uint32_t usageHint = parameter[2].toUInt();
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[3].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		EScript::Array * a = parameter[1].to<EScript::Array*>(rt);
		if(a->empty())
			return thisEObj;
		
		if(a->at(0).toType<E_Geometry::E_Matrix4x4>()) {
			uploadArray<Geometry::Matrix4x4>(rt, *thisObj, a, bufferTarget, usageHint);
		} else if(a->at(0).toType<E_Geometry::E_Vec3>()) {
			uploadArray<Geometry::Vec3>(rt, *thisObj, a, bufferTarget, usageHint);
		} else if(a->at(0).toType<E_Geometry::E_Vec4>()) {
			uploadArray<Geometry::Vec4>(rt, *thisObj, a, bufferTarget, usageHint);
		} else {
			switch(type){
				case Util::TypeConstant::DOUBLE:	uploadArray<double>(rt, *thisObj, a, bufferTarget, usageHint);		break;
				case Util::TypeConstant::FLOAT: 	uploadArray<float>(rt, *thisObj, a, bufferTarget, usageHint);			break;
				case Util::TypeConstant::INT16: 	uploadArray<int16_t>(rt, *thisObj, a, bufferTarget, usageHint);		break;
				case Util::TypeConstant::INT32: 	uploadArray<int32_t>(rt, *thisObj, a, bufferTarget, usageHint);		break;
				case Util::TypeConstant::INT64: 	uploadArray<int64_t>(rt, *thisObj, a, bufferTarget, usageHint);		break;
				case Util::TypeConstant::INT8: 		uploadArray<int8_t>(rt, *thisObj, a, bufferTarget, usageHint);		break;
				case Util::TypeConstant::UINT16: 	uploadArray<uint16_t>(rt, *thisObj, a, bufferTarget, usageHint);	break;
				case Util::TypeConstant::UINT32: 	uploadArray<uint32_t>(rt, *thisObj, a, bufferTarget, usageHint);	break;
				case Util::TypeConstant::UINT64: 	uploadArray<uint64_t>(rt, *thisObj, a, bufferTarget, usageHint);	break;
				case Util::TypeConstant::UINT8:		uploadArray<uint8_t>(rt, *thisObj, a, bufferTarget, usageHint);		break;
			}
		}
		
		return thisEObj;
	})
	
	//! [ESF] thisEObj BufferObject.uploadSubData(bufferTarget, Array of float/Vec3/Vec4/Matrix4x4, offset, [type])
	ES_MFUNCTION(typeObject,BufferObject,"uploadSubData", 3, 4, {
		uint32_t bufferTarget = parameter[0].toUInt();
		size_t offset = parameter[2].toUInt();
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[3].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		EScript::Array * a = parameter[1].to<EScript::Array*>(rt);
		if(a->empty())
			return thisEObj;
		
		if(a->at(0).toType<E_Geometry::E_Matrix4x4>()) {
			uploadSubArray<Geometry::Matrix4x4>(rt, *thisObj, a, bufferTarget, offset);
		} else if(a->at(0).toType<E_Geometry::E_Vec3>()) {
			uploadSubArray<Geometry::Vec3>(rt, *thisObj, a, bufferTarget, offset);
		} else if(a->at(0).toType<E_Geometry::E_Vec4>()) {
			uploadSubArray<Geometry::Vec4>(rt, *thisObj, a, bufferTarget, offset);
		} else {
			switch(type){
				case Util::TypeConstant::DOUBLE:	uploadSubArray<double>(rt, *thisObj, a, bufferTarget, offset);		break;
				case Util::TypeConstant::FLOAT: 	uploadSubArray<float>(rt, *thisObj, a, bufferTarget, offset);			break;
				case Util::TypeConstant::INT16: 	uploadSubArray<int16_t>(rt, *thisObj, a, bufferTarget, offset);		break;
				case Util::TypeConstant::INT32: 	uploadSubArray<int32_t>(rt, *thisObj, a, bufferTarget, offset);		break;
				case Util::TypeConstant::INT64: 	uploadSubArray<int64_t>(rt, *thisObj, a, bufferTarget, offset);		break;
				case Util::TypeConstant::INT8: 		uploadSubArray<int8_t>(rt, *thisObj, a, bufferTarget, offset);		break;
				case Util::TypeConstant::UINT16: 	uploadSubArray<uint16_t>(rt, *thisObj, a, bufferTarget, offset);	break;
				case Util::TypeConstant::UINT32: 	uploadSubArray<uint32_t>(rt, *thisObj, a, bufferTarget, offset);	break;
				case Util::TypeConstant::UINT64: 	uploadSubArray<uint64_t>(rt, *thisObj, a, bufferTarget, offset);	break;
				case Util::TypeConstant::UINT8:		uploadSubArray<uint8_t>(rt, *thisObj, a, bufferTarget, offset);		break;
			}
		}
		
		return thisEObj;
	})
	
	//! [ESF] Array BufferObject.downloadData(bufferTarget, count, type, [offset])
	ES_MFUNCTION(typeObject,BufferObject,"downloadData", 3, 4, {
		uint32_t bufferTarget = parameter[0].toUInt();
		uint32_t count = parameter[1].toUInt();
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[2].toUInt());
		uint32_t offset = parameter[3].toUInt(0);
		
		EScript::Array * a = nullptr;
		switch(type){
			case Util::TypeConstant::DOUBLE:	a = downloadToArray<double>(rt, *thisObj, bufferTarget, count, offset);		break;
			case Util::TypeConstant::FLOAT: 	a = downloadToArray<float>(rt, *thisObj, bufferTarget, count, offset);			break;
			case Util::TypeConstant::INT16: 	a = downloadToArray<int16_t>(rt, *thisObj, bufferTarget, count, offset);		break;
			case Util::TypeConstant::INT32: 	a = downloadToArray<int32_t>(rt, *thisObj, bufferTarget, count, offset);		break;
			case Util::TypeConstant::INT64: 	a = downloadToArray<int64_t>(rt, *thisObj, bufferTarget, count, offset);		break;
			case Util::TypeConstant::INT8: 		a = downloadToArray<int8_t>(rt, *thisObj, bufferTarget, count, offset);		break;
			case Util::TypeConstant::UINT16: 	a = downloadToArray<uint16_t>(rt, *thisObj, bufferTarget, count, offset);	break;
			case Util::TypeConstant::UINT32: 	a = downloadToArray<uint32_t>(rt, *thisObj, bufferTarget, count, offset);	break;
			case Util::TypeConstant::UINT64: 	a = downloadToArray<uint64_t>(rt, *thisObj, bufferTarget, count, offset);	break;
			case Util::TypeConstant::UINT8:		a = downloadToArray<uint8_t>(rt, *thisObj, bufferTarget, count, offset);		break;
		}
		return a;
	})
	
	//! [ESMF] thisEObj BufferObject.clear(bufferTarget, [internalFormat=GL_R8UI, format=GL_RED_INTEGER, type=GL_UNSIGNED_BYTE])
	ES_MFUN(typeObject,BufferObject,"clear",1,4,(thisObj->clear(parameter[0].toUInt(), parameter[1].toUInt(0x8232), parameter[2].toUInt(0x8D94), parameter[3].toUInt(0x1401)), thisEObj))
	
	//! [ESMF] thisEObj BufferObject.copy(BufferObject source, sourceOffset, targetOffset, size)
	ES_MFUN(typeObject,BufferObject,"copy",4,4,(thisObj->copy(*parameter[0].to<BufferObject*>(rt), parameter[1].toUInt(), parameter[2].toUInt(), parameter[3].toUInt()), thisEObj))
				
	//! [ESMF] thisEObj BufferObject._bind(bufferTarget, [location])
	ES_MFUN(typeObject,BufferObject,"_bind",1,2,(parameter.count() == 1 ? thisObj->bind(parameter[0].toUInt()) : thisObj->bind(parameter[0].toUInt(), parameter[1].toUInt()), thisEObj))
	
	//! [ESMF] thisEObj BufferObject._unbind(bufferTarget, [location])
	ES_MFUN(typeObject,BufferObject,"_unbind",1,2,(parameter.count() == 1 ? thisObj->unbind(parameter[0].toUInt()) : thisObj->unbind(parameter[0].toUInt(), parameter[1].toUInt()), thisEObj))
}

}
