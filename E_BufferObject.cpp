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

namespace E_Rendering {

//! (static)
EScript::Type * E_BufferObject::getTypeObject() {
	// E_BufferObject ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

template<typename T>
EScript::Array * downloadToArray(Rendering::BufferObject& bo, uint32_t target, size_t count) {
	EScript::Array * a = EScript::Array::create();
	auto data = bo.downloadData<T>(target, count);
	for(T& v : data)
			a->pushBack(EScript::Number::create(v));
	return a;
}

//! initMembers
void E_BufferObject::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);
	
	using namespace Rendering;

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
	
	//! [ESF] thisEObj BufferObject.uploadData(bufferTarget, Array of float/Vec3/Vec4/Matrix4x4, usageHint)
	ES_MFUNCTION(typeObject,BufferObject,"uploadData", 3, 3, {
		uint32_t bufferTarget = parameter[0].toUInt();
		uint32_t usageHint = parameter[2].toUInt();
		EScript::Array * a = parameter[1].to<EScript::Array*>(rt);
		if(a->empty())
			return thisEObj;
			
		const uint8_t* data;
		size_t size = 0; 
		
		if(a->at(0).toType<E_Geometry::E_Matrix4x4>()) {
			std::vector<Geometry::Matrix4x4> tmp;
			for(auto value : *a)
				tmp.push_back(value.to<Geometry::Matrix4x4>(rt).getTransposed());
			data = reinterpret_cast<const uint8_t*>(tmp.data());
			size = tmp.size() * sizeof(float) * 16;
			thisObj->uploadData(bufferTarget, data, size, usageHint);
		} else if(a->at(0).toType<E_Geometry::E_Vec3>()) {
			std::vector<Geometry::Vec3> tmp;
			for(auto value : *a)
				tmp.push_back(value.to<Geometry::Vec3>(rt));
			data = reinterpret_cast<const uint8_t*>(tmp.data());
			size = tmp.size() * sizeof(float) * 3;
			thisObj->uploadData(bufferTarget, data, size, usageHint);
		} else if(a->at(0).toType<E_Geometry::E_Vec4>()) {
			std::vector<Geometry::Vec4> tmp;
			for(auto value : *a)
				tmp.push_back(value.to<Geometry::Vec4>(rt));
			data = reinterpret_cast<const uint8_t*>(tmp.data());
			size = tmp.size() * sizeof(float) * 4;
			thisObj->uploadData(bufferTarget, data, size, usageHint);
		} else {
			std::vector<float> tmp;
			for(auto value : *a)
				tmp.push_back(value.toFloat());
			data = reinterpret_cast<const uint8_t*>(tmp.data());
			size = tmp.size() * sizeof(float);
			thisObj->uploadData(bufferTarget, data, size, usageHint);
		}		
		return thisEObj;
	})
		
	//! [ESF] Array BufferObject.downloadData(bufferTarget, count, type)
	ES_MFUNCTION(typeObject,BufferObject,"downloadData", 3, 3, {
		uint32_t bufferTarget = parameter[0].toUInt();
		uint32_t count = parameter[1].toUInt();
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[2].toUInt());
		
		EScript::Array * a = nullptr;
		switch(type){
			case Util::TypeConstant::UINT8:		a = downloadToArray<uint8_t>(*thisObj, bufferTarget, count);	break;
			case Util::TypeConstant::UINT32:	a = downloadToArray<uint32_t>(*thisObj, bufferTarget, count);	break;
			case Util::TypeConstant::FLOAT:		a = downloadToArray<float>(*thisObj, bufferTarget, count);		break;
			default: WARN("downloadData: invalid data type constant. Supported are UINT8, UINT32, FLOAT");
		}
		return a;
	})
	
	//! [ESMF] thisEObj BufferObject._bind(bufferTarget, [location])
	ES_MFUN(typeObject,BufferObject,"_bind",1,2,(parameter.count() == 1 ? thisObj->bind(parameter[0].toUInt()) : thisObj->bind(parameter[0].toUInt(), parameter[1].toUInt()), thisEObj))
	
	//! [ESMF] thisEObj BufferObject._unbind(bufferTarget, [location])
	ES_MFUN(typeObject,BufferObject,"_unbind",1,2,(parameter.count() == 1 ? thisObj->unbind(parameter[0].toUInt()) : thisObj->unbind(parameter[0].toUInt(), parameter[1].toUInt()), thisEObj))
}

}
