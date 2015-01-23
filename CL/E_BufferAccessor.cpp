/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_BufferAccessor.h"
#include "E_Buffer.h"
#include "E_CommandQueue.h"

#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Vec4.h>

#include <Util/TypeConstant.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace EScript {
template<> inline int8_t convertTo<int8_t>(Runtime& rt,ObjPtr src)	{	return static_cast<int8_t>(convertTo<double>(rt,src));	}
template<> inline uint8_t convertTo<uint8_t>(Runtime& rt,ObjPtr src)	{	return static_cast<uint8_t>(convertTo<double>(rt,src));	}
}

namespace E_Rendering{
namespace E_CL{

template<typename T>
std::vector<T> arrayToVector(EScript::Runtime& rt, EScript::Array* arr) {
	using namespace E_Geometry;
	std::vector<T> out;
	out.reserve(arr->size());
	for(auto val : *arr) {
		E_Vec2* v2 = val.toType<E_Vec2>();
		E_Vec3* v3 = val.toType<E_Vec3>();
		E_Vec4* v4 = val.toType<E_Vec4>();
		if(v2 != nullptr) {
			out.push_back(static_cast<T>((**v2).x()));
			out.push_back(static_cast<T>((**v2).y()));
		} else if(v3 != nullptr) {
			out.push_back(static_cast<T>((**v3).x()));
			out.push_back(static_cast<T>((**v3).y()));
			out.push_back(static_cast<T>((**v3).z()));
		} else if(v4 != nullptr) {
			out.push_back(static_cast<T>((**v4).x()));
			out.push_back(static_cast<T>((**v4).y()));
			out.push_back(static_cast<T>((**v4).z()));
			out.push_back(static_cast<T>((**v4).w()));
		} else {
			out.push_back(val.to<T>(rt));
		}
	}
	return out;
}
template<typename T>
void writeEObj(Rendering::CL::BufferAccessor* accessor, EScript::Runtime& rt, EScript::ObjPtr obj) {
	using namespace E_Geometry;
	EScript::Array* arr = obj.toType<EScript::Array>();
	if(arr != nullptr) {
		accessor->writeArray(arrayToVector<T>(rt, arr));
		return;
	}
	E_Vec2* v2 = obj.toType<E_Vec2>();
	if(v2 != nullptr) {
		accessor->write<T>(static_cast<T>((**v2).x()));
		accessor->write<T>(static_cast<T>((**v2).y()));
		return;
	}
	E_Vec3* v3 = obj.toType<E_Vec3>();
	if(v3 != nullptr) {
		accessor->write<T>(static_cast<T>((**v3).x()));
		accessor->write<T>(static_cast<T>((**v3).y()));
		accessor->write<T>(static_cast<T>((**v3).z()));
		return;
	}
	E_Vec4* v4 = obj.toType<E_Vec4>();
	if(v4 != nullptr) {
		accessor->write<T>(static_cast<T>((**v4).x()));
		accessor->write<T>(static_cast<T>((**v4).y()));
		accessor->write<T>(static_cast<T>((**v4).z()));
		accessor->write<T>(static_cast<T>((**v4).w()));
		return;
	}
	accessor->write<T>(obj.to<T>(rt));
}

template<typename T>
EScript::Object* readEObj(Rendering::CL::BufferAccessor* accessor, EScript::Runtime& rt, size_t numValues) {
	if(numValues > 0) {
		EScript::Array* arr = EScript::Array::create();
		arr->reserve(numValues);
		auto vec = accessor->readArray<T>(numValues);
		for(auto v : vec)
			arr->pushBack(EScript::Number::create(v));
		return arr;
	}
	return EScript::Number::create(accessor->read<T>());
}


template<typename T>
EScript::Object* readVec2(Rendering::CL::BufferAccessor* accessor, EScript::Runtime& rt, size_t numValues) {
	using namespace Geometry;
	if(numValues > 0) {
		EScript::Array* arr = EScript::Array::create();
		arr->reserve(numValues);
		auto vec = accessor->readArray<T>(numValues * 2);
		for(uint32_t i = 0; i<vec.size(); i+=2) {
			arr->pushBack(EScript::create(Vec2(vec[i], vec[i+1])));
		}
		return arr;
	}
	return EScript::create(Vec2(accessor->read<T>(), accessor->read<T>()));
}


template<typename T>
EScript::Object* readVec3(Rendering::CL::BufferAccessor* accessor, EScript::Runtime& rt, size_t numValues) {
	using namespace Geometry;
	if(numValues > 0) {
		EScript::Array* arr = EScript::Array::create();
		arr->reserve(numValues);
		auto vec = accessor->readArray<T>(numValues * 3);
		for(uint32_t i = 0; i<vec.size(); i+=3) {
			arr->pushBack(EScript::create(Vec3(vec[i], vec[i+1], vec[i+2])));
		}
		return arr;
	}
	return EScript::create(Vec3(accessor->read<T>(), accessor->read<T>(), accessor->read<T>()));
}


template<typename T>
EScript::Object* readVec4(Rendering::CL::BufferAccessor* accessor, EScript::Runtime& rt, size_t numValues) {
	using namespace Geometry;
	if(numValues > 0) {
		EScript::Array* arr = EScript::Array::create();
		arr->reserve(numValues);
		auto vec = accessor->readArray<T>(numValues * 4);
		for(uint32_t i = 0; i<vec.size(); i+=4) {
			arr->pushBack(EScript::create(Vec4(vec[i], vec[i+1], vec[i+2], vec[i+3])));
		}
		return arr;
	}
	return EScript::create(Vec4(accessor->read<T>(), accessor->read<T>(), accessor->read<T>(), accessor->read<T>()));
}


//! (static)
EScript::Type * E_BufferAccessor::getTypeObject() {
	// E_BufferAccessor ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_BufferAccessor::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_BufferAccessor::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	// BufferAccessor(BufferRef buffer, CommandQueueRef queue)
	//! [ESF] new BufferAccessor(Buffer,CommandQueue)
	ES_CTOR(typeObject,2,2,
				new BufferAccessor(parameter[0].to<Buffer*>(rt), parameter[1].to<CommandQueue*>(rt)))

	// void begin(ReadWrite_t readWrite = ReadWrite_t::ReadWrite)
	//! [ESMF] self BufferAccessor.begin([readWrite])
	ES_MFUN(typeObject,BufferAccessor,"begin",0,1,
				(thisObj->begin(static_cast<ReadWrite_t>(parameter[0].toUInt(static_cast<uint32_t>(ReadWrite_t::ReadWrite)))),thisEObj))

	// void end()
	//! [ESMF] self BufferAccessor.end()
	ES_MFUN(typeObject,BufferAccessor,"end",0,0,
				(thisObj->end(),thisEObj))

	// inline bool isValid() const
	//! [ESMF] RESULT BufferAccessor.isValid()
	ES_MFUN(typeObject,const BufferAccessor,"isValid",0,0,
				EScript::create(thisObj->isValid()))

	// inline void setCursor(size_t offset)
	//! [ESMF] self BufferAccessor.setCursor(offset)
	ES_MFUN(typeObject,BufferAccessor,"setCursor",0,1,
				(thisObj->setCursor(parameter[0].toUInt(0)),thisEObj))

	// inline size_t getCursor() const
	//! [ESMF] RESULT BufferAccessor.getCursor()
	ES_MFUN(typeObject,const BufferAccessor,"getCursor",0,0,
				EScript::Number::create(thisObj->getCursor()))

	// inline size_t getSize() const
	//! [ESMF] RESULT BufferAccessor.getSize()
	ES_MFUN(typeObject,const BufferAccessor,"getSize",0,0,
				EScript::Number::create(thisObj->getSize()))

	// inline T read()
	//! [ESMF] RESULT BufferAccessor.read(Util::TypeConstant, numValues)
	ES_MFUNCTION(typeObject,BufferAccessor,"read",0,2, {
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[0].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		uint32_t size = parameter[1].toUInt(0);
		switch(type) {
		case Util::TypeConstant::UINT8:
			return readEObj<uint8_t>(thisObj, rt, size);
		case Util::TypeConstant::INT8:
			return readEObj<int8_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT16:
			return readEObj<uint16_t>(thisObj, rt, size);
		case Util::TypeConstant::INT16:
			return readEObj<int16_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT32:
			return readEObj<uint32_t>(thisObj, rt, size);
		case Util::TypeConstant::INT32:
			return readEObj<int32_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT64:
			return readEObj<uint64_t>(thisObj, rt, size);
		case Util::TypeConstant::INT64:
			return readEObj<int64_t>(thisObj, rt, size);
		case Util::TypeConstant::DOUBLE:
			return readEObj<double>(thisObj, rt, size);
		default:
			return readEObj<float>(thisObj, rt, size);
		}
	})

	//! [ESMF] RESULT BufferAccessor.readVec2(Util::TypeConstant, numValues)
	ES_MFUNCTION(typeObject,BufferAccessor,"readVec2",0,2, {
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[0].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		uint32_t size = parameter[1].toUInt(0);
		switch(type) {
		case Util::TypeConstant::UINT8:
			return readVec2<uint8_t>(thisObj, rt, size);
		case Util::TypeConstant::INT8:
			return readVec2<int8_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT16:
			return readVec2<uint16_t>(thisObj, rt, size);
		case Util::TypeConstant::INT16:
			return readVec2<int16_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT32:
			return readVec2<uint32_t>(thisObj, rt, size);
		case Util::TypeConstant::INT32:
			return readVec2<int32_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT64:
			return readVec2<uint64_t>(thisObj, rt, size);
		case Util::TypeConstant::INT64:
			return readVec2<int64_t>(thisObj, rt, size);
		case Util::TypeConstant::DOUBLE:
			return readVec2<double>(thisObj, rt, size);
		default:
			return readVec2<float>(thisObj, rt, size);
		}
	})

	//! [ESMF] RESULT BufferAccessor.readVec2(Util::TypeConstant, numValues)
	ES_MFUNCTION(typeObject,BufferAccessor,"readVec3",0,2, {
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[0].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		uint32_t size = parameter[1].toUInt(0);
		switch(type) {
		case Util::TypeConstant::UINT8:
			return readVec3<uint8_t>(thisObj, rt, size);
		case Util::TypeConstant::INT8:
			return readVec3<int8_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT16:
			return readVec3<uint16_t>(thisObj, rt, size);
		case Util::TypeConstant::INT16:
			return readVec3<int16_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT32:
			return readVec3<uint32_t>(thisObj, rt, size);
		case Util::TypeConstant::INT32:
			return readVec3<int32_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT64:
			return readVec3<uint64_t>(thisObj, rt, size);
		case Util::TypeConstant::INT64:
			return readVec3<int64_t>(thisObj, rt, size);
		case Util::TypeConstant::DOUBLE:
			return readVec3<double>(thisObj, rt, size);
		default:
			return readVec3<float>(thisObj, rt, size);
		}
	})

	//! [ESMF] RESULT BufferAccessor.readVec2(Util::TypeConstant, numValues)
	ES_MFUNCTION(typeObject,BufferAccessor,"readVec4",0,2, {
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[0].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		uint32_t size = parameter[1].toUInt(0);
		switch(type) {
		case Util::TypeConstant::UINT8:
			return readVec4<uint8_t>(thisObj, rt, size);
		case Util::TypeConstant::INT8:
			return readVec4<int8_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT16:
			return readVec4<uint16_t>(thisObj, rt, size);
		case Util::TypeConstant::INT16:
			return readVec4<int16_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT32:
			return readVec4<uint32_t>(thisObj, rt, size);
		case Util::TypeConstant::INT32:
			return readVec4<int32_t>(thisObj, rt, size);
		case Util::TypeConstant::UINT64:
			return readVec4<uint64_t>(thisObj, rt, size);
		case Util::TypeConstant::INT64:
			return readVec4<int64_t>(thisObj, rt, size);
		case Util::TypeConstant::DOUBLE:
			return readVec4<double>(thisObj, rt, size);
		default:
			return readVec4<float>(thisObj, rt, size);
		}
	})

	// inline void write(const T& value)
	//! [ESMF] self BufferAccessor.write(p0)
	ES_MFUNCTION(typeObject,BufferAccessor,"write",1,2, {
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[1].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));
		switch(type) {
		case Util::TypeConstant::UINT8:
			writeEObj<uint8_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::INT8:
			writeEObj<int8_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::UINT16:
			writeEObj<uint16_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::INT16:
			writeEObj<int16_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::UINT32:
			writeEObj<uint32_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::INT32:
			writeEObj<int32_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::UINT64:
			writeEObj<uint64_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::INT64:
			writeEObj<int64_t>(thisObj, rt, parameter[0]); break;
		case Util::TypeConstant::DOUBLE:
			writeEObj<double>(thisObj, rt, parameter[0]); break;
		default:
			writeEObj<float>(thisObj, rt, parameter[0]);
		}
		return thisEObj;
	})

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

