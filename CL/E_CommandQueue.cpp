/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_CommandQueue.h"
#include "E_Context.h"
#include "E_Device.h"
#include "E_Buffer.h"
#include "E_Image.h"
#include "E_Event.h"
#include "E_Kernel.h"

#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Vec4.h>

#include <Util/TypeConstant.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <iostream>

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
bool fillBuffer(Rendering::CL::CommandQueue* queue, Rendering::CL::Buffer* buffer, size_t offset, size_t count, EScript::Runtime& rt, EScript::ObjPtr obj, const std::vector<Rendering::CL::Event*>& waitForEvents, Rendering::CL::Event* event) {
	using namespace E_Geometry;
	size_t typeSize = sizeof(T);
	EScript::Array* arr = obj.toType<EScript::Array>();
	if(arr != nullptr) {
		auto vec = arrayToVector<T>(rt, arr);
		return queue->fillBuffer(buffer, offset, vec.size()*typeSize*count, vec.data(), vec.size()*typeSize, waitForEvents, event);
	}
	E_Vec2* v2 = obj.toType<E_Vec2>();
	if(v2 != nullptr) {	
		return queue->fillBuffer(buffer, offset, 2*typeSize*count, (**v2).getVec(), 2*typeSize, waitForEvents, event);
	}
	E_Vec3* v3 = obj.toType<E_Vec3>();
	if(v3 != nullptr) {
		return queue->fillBuffer(buffer, offset, 3*typeSize*count, (**v3).getVec(), 3*typeSize, waitForEvents, event);
	}
	E_Vec4* v4 = obj.toType<E_Vec4>();
	if(v4 != nullptr) {
		return queue->fillBuffer(buffer, offset, 4*typeSize*count, (**v4).getVec(), 4*typeSize, waitForEvents, event);
	}
	T val = obj.to<T>(rt);
	return queue->fillBuffer(buffer, offset, typeSize*count, &val, typeSize, waitForEvents, event);
}

inline
Rendering::CL::RangeND_t toNDRange(EScript::Runtime& rt, EScript::ObjPtr obj) {
	using namespace Rendering::CL;
	const EScript::Array * a = obj.to<EScript::Array*>(rt);
	std::array<size_t, 3> range = {0,0,0};
	for(uint32_t i = 0; i < a->size(); ++i)
		range[i] = a->get(i)->toUInt();
	auto range_nd = RangeND_t();
	range_nd.dim = a->size();
	range_nd.range = range;
	return range_nd;
}

inline
std::vector<Rendering::CL::Event*> toEventVector(EScript::Runtime& rt, EScript::ObjPtr obj) {
	using namespace Rendering::CL;
	std::vector<Event*> events;
	const EScript::Array * a = obj.to<EScript::Array*>(rt);
	for(auto e : *a)
		events.push_back(e.to<Event*>(rt));
	return events;
}

//! (static)
EScript::Type * E_CommandQueue::getTypeObject() {
	// E_CommandQueue ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_CommandQueue::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_CommandQueue::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	using namespace Rendering;
	using namespace E_Geometry;
	using namespace Geometry;
	
	//!	[ESMF] CommandQueue new CommandQueue(Context, Device, [outOfOrder, [profiling]])
	ES_CTOR(typeObject,2,4,new E_CommandQueue(new CommandQueue(parameter[0].to<Context*>(rt), parameter[1].to<Device*>(rt), parameter[2].toBool(false), parameter[3].toBool(false))))

	// bool acquireGLObjects(const std::vector<Memory*>& buffers, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.acquireGLObjects(p0[,p1])
	ES_MFUNCTION(typeObject,CommandQueue,"acquireGLObjects",1,3, {
		const EScript::Array * a = parameter[0].to<EScript::Array*>(rt);
		std::vector<Memory*> buffers;
		for(auto buf : *a)
			buffers.push_back(buf.to<Memory*>(rt));
		std::vector<Event*> events = parameter.size() > 1 ? toEventVector(rt, parameter[1]) : std::vector<Event*>();
		return thisObj->acquireGLObjects(buffers, events, parameter.size() > 2 ? parameter[2].to<Event*>(rt) : nullptr);
	})

	// void barrier()
	//! [ESMF] self CommandQueue.barrier()
	ES_MFUN(typeObject,CommandQueue,"barrier",0,0,
				(thisObj->barrier(),thisEObj))

	// bool copyBuffer(Buffer* src, Buffer* dst, size_t srcOffset, size_t dstOffset, size_t size, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.copyBuffer(p0,p1,p2,p3,p4[,p5])
	ES_MFUNCTION(typeObject,CommandQueue,"copyBuffer",5,7, {
		std::vector<Event*> events;
		if(parameter.size() > 5) {
			const EScript::Array * a = parameter[5].to<EScript::Array*>(rt);
			for(auto e : *a)
				events.push_back(e.to<Event*>(rt));
		}
		return thisObj->copyBuffer(
				parameter[0].to<Buffer*>(rt),
				parameter[1].to<Buffer*>(rt),
				parameter[2].to<size_t>(rt),
				parameter[3].to<size_t>(rt),
				parameter[4].to<size_t>(rt),
				events,
				parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr);
	})

	// bool copyBufferRect(Buffer* src, Buffer* dst, const RangeND_t& srcOrigin, const RangeND_t& dstOrigin, const RangeND_t& region, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr, size_t bufferRowPitch = 0, size_t bufferSlicePitch = 0, size_t hostRowPitch = 0, size_t hostSlicePitch = 0)
	//! [ESMF] RESULT CommandQueue.copyBufferRect(p0,p1,p2,p3,p4[,p5])
	ES_MFUNCTION(typeObject,CommandQueue,"copyBufferRect",5,7, {
		std::vector<Event*> events = parameter.size() > 5 ? toEventVector(rt, parameter[5]) : std::vector<Event*>();
		return thisObj->copyBufferRect(
				parameter[0].to<Buffer*>(rt),
				parameter[1].to<Buffer*>(rt),
				toNDRange(rt, parameter[2]),
				toNDRange(rt, parameter[3]),
				toNDRange(rt, parameter[4]),
				events,
				parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr);
	})

	// bool copyBufferToImage(Buffer* src, Image* dst, size_t srcOffset, const RangeND_t& dstOrigin, const RangeND_t& region, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.copyBufferToImage(p0,p1,p2,p3,p4[,p5])
	ES_MFUNCTION(typeObject,CommandQueue,"copyBufferToImage",5,7, {
		std::vector<Event*> events = parameter.size() > 5 ? toEventVector(rt, parameter[5]) : std::vector<Event*>();
		return thisObj->copyBufferToImage(
				parameter[0].to<Buffer*>(rt),
				parameter[1].to<Image*>(rt),
				parameter[2].to<size_t>(rt),
				toNDRange(rt, parameter[3]),
				toNDRange(rt, parameter[4]),
				events,
				parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr);
	})

	// bool copyImage(Image* src, Image* dst, const RangeND_t& srcOrigin, const RangeND_t& dstOrigin, const RangeND_t& region, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.copyImage(p0,p1,p2,p3,p4[,p5])
	ES_MFUNCTION(typeObject,CommandQueue,"copyImage",5,7, {
		std::vector<Event*> events = parameter.size() > 5 ? toEventVector(rt, parameter[5]) : std::vector<Event*>();
		return thisObj->copyImage(
				parameter[0].to<Image*>(rt),
				parameter[1].to<Image*>(rt),
				toNDRange(rt, parameter[2]),
				toNDRange(rt, parameter[3]),
				toNDRange(rt, parameter[4]),
				events,
				parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr);
	})

	// bool copyImageToBuffer(Image* src, Buffer* dst, const RangeND_t& srcOrigin, const RangeND_t& region, size_t dstOffset, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.copyImageToBuffer(p0,p1,p2,p3,p4[,p5])
	ES_MFUNCTION(typeObject,CommandQueue,"copyImageToBuffer",5,7, {
		std::vector<Event*> events = parameter.size() > 5 ? toEventVector(rt, parameter[5]) : std::vector<Event*>();
		return thisObj->copyImageToBuffer(
				parameter[0].to<Image*>(rt),
				parameter[1].to<Buffer*>(rt),
				toNDRange(rt, parameter[2]),
				toNDRange(rt, parameter[3]),
				parameter[4].to<size_t>(rt),
				events,
				parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr);
	})

	// bool execute(Kernel* kernel, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.execute(p0)
	ES_MFUNCTION(typeObject,CommandQueue,"execute",4,6, {
		std::vector<Event*> events = parameter.size() > 4 ? toEventVector(rt, parameter[4]) : std::vector<Event*>();
		return thisObj->execute(
				parameter[0].to<Kernel*>(rt),
				toNDRange(rt, parameter[1]),
				toNDRange(rt, parameter[2]),
				toNDRange(rt, parameter[3]),
				events,
				parameter.size() > 5 ? parameter[5].to<Event*>(rt) : nullptr);
	})

	// void finish()
	//! [ESMF] self CommandQueue.finish()
	ES_MFUN(typeObject,CommandQueue,"finish",0,0,
				(thisObj->finish(),thisEObj))

	// void flush()
	//! [ESMF] self CommandQueue.flush()
	ES_MFUN(typeObject,CommandQueue,"flush",0,0,
				(thisObj->flush(),thisEObj))

	// Context* getContext() const
	//! [ESMF] RESULT CommandQueue.getContext()
	ES_MFUN(typeObject,const CommandQueue,"getContext",0,0,
				EScript::create(thisObj->getContext()))

	// Device* getDevice() const
	//! [ESMF] RESULT CommandQueue.getDevice()
	ES_MFUN(typeObject,const CommandQueue,"getDevice",0,0,
				EScript::create(thisObj->getDevice()))

	// void* mapBuffer(Buffer* buffer, bool blocking, MapFlags_t readWrite, size_t offset, size_t size, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] self CommandQueue.mapBuffer(p0,p1,p2,p3,p4[,p5])
//	ES_MFUN(typeObject,CommandQueue,"mapBuffer",5,6,
//				(thisObj->mapBuffer(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt)),thisEObj))

	// MappedImage mapImage(Image* image, bool blocking, MapFlags_t readWrite, const RangeND_t& origin, const RangeND_t& region, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.mapImage(p0,p1,p2,p3,p4[,p5])
//	ES_MFUN(typeObject,CommandQueue,"mapImage",5,6,
//				EScript::create(thisObj->mapImage(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt))))

	// void marker(Event* event = nullptr)
	//! [ESMF] self CommandQueue.marker([p0])
	ES_MFUN(typeObject,CommandQueue,"marker",0,1,
				(thisObj->marker(parameter.size() > 0 ? parameter[0].to<Event*>(rt) : nullptr),thisEObj))

	// bool readBuffer(Buffer* buffer, bool blocking, size_t offset, size_t size, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.readBuffer(p0,p1,p2,p3,p4[,p5])
//	ES_MFUN(typeObject,CommandQueue,"readBuffer",5,6,
//				EScript::create(thisObj->readBuffer(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt))))

	// bool readBufferRect(Buffer* buffer, bool blocking, const RangeND_t& bufferOffset, const RangeND_t& hostOffset, const RangeND_t& region, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr, size_t bufferRowPitch = 0, size_t bufferSlicePitch = 0, size_t hostRowPitch = 0, size_t hostSlicePitch = 0)
	//! [ESMF] RESULT CommandQueue.readBufferRect(p0,p1,p2,p3,p4,p5[,p6])
//	ES_MFUN(typeObject,CommandQueue,"readBufferRect",6,7,
//				EScript::create(thisObj->readBufferRect(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt),parameter[6].to<XXX>(rt))))

	// bool readImage(Image* image, bool blocking, const RangeND_t& origin, const RangeND_t& region, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr, size_t rowPitch = 0, size_t slicePitch = 0)
	//! [ESMF] RESULT CommandQueue.readImage(p0,p1,p2,p3,p4[,p5])
//	ES_MFUN(typeObject,CommandQueue,"readImage",5,6,
//				EScript::create(thisObj->readImage(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt))))

	// bool releaseGLObjects(const std::vector<Memory*>& buffers, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.releaseGLObjects(p0[,p1])
	ES_MFUNCTION(typeObject,CommandQueue,"releaseGLObjects",1,3, {
		const EScript::Array * a = parameter[0].to<EScript::Array*>(rt);
		std::vector<Memory*> buffers;
		for(auto buf : *a)
			buffers.push_back(buf.to<Memory*>(rt));
		std::vector<Event*> events = parameter.size() > 1 ? toEventVector(rt, parameter[1]) : std::vector<Event*>();
		return thisObj->releaseGLObjects(buffers, events, parameter.size() > 2 ? parameter[2].to<Event*>(rt) : nullptr);
	})

	// bool unmapMemory(Memory* memory, void* mappedPtr, const EventList_t& waitForEvents, Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.unmapMemory(p0,p1,p2[,p3])
//	ES_MFUN(typeObject,CommandQueue,"unmapMemory",3,4,
//				EScript::create(thisObj->unmapMemory(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt))))

	// void waitForEvents(const EventList_t& waitForEvents)
	//! [ESMF] self CommandQueue.waitForEvents(p0)
	ES_MFUNCTION(typeObject,CommandQueue,"waitForEvents",1,1, {
		std::vector<Event*> events = toEventVector(rt, parameter[0]);
		thisObj->waitForEvents(events);
		return thisEObj;
	})

	// bool writeBuffer(Buffer* buffer, bool blocking, size_t offset, size_t size, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr)
	//! [ESMF] RESULT CommandQueue.writeBuffer(Buffer, offset, Array [, waitForEvents [, event]])
	ES_MFUNCTION(typeObject,CommandQueue,"writeBuffer",3,5, {
		std::vector<Event*> events;
		if(parameter.size() > 3) {
			const EScript::Array * a = parameter[3].to<EScript::Array*>(rt);
			for(auto e : *a)
				events.push_back(e.to<Event*>(rt));
		}
		const EScript::Array * arr = parameter[2].to<EScript::Array*>(rt);
		std::vector<float> values;
		for(uint32_t i=0; i<arr->size(); ++i) {
			E_Vec2* v2 = arr->at(i).toType<E_Vec2>();
			E_Vec3* v3 = arr->at(i).toType<E_Vec3>();
			E_Vec4* v4 = arr->at(i).toType<E_Vec4>();
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
				values.push_back(arr->at(i).toFloat());
			}
		}

		return thisObj->writeBuffer(
				parameter[0].to<Buffer*>(rt),
				true, // only use with blocking
				parameter[1].to<size_t>(rt) * sizeof(float),
				values.size() * sizeof(float),
				values.data(),
				events,
				parameter.size() > 4 ? parameter[4].to<Event*>(rt) : nullptr);
	})
	
	
	// bool fillBuffer(Buffer* buffer, size_t offset, size_t size, const void* pattern, size_t patternSize, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr);
	//! [ESMF] RESULT CommandQueue.fillBuffer(Buffer, offset, Object, count, TypeConstant type [, waitForEvents [, event]])
	ES_MFUNCTION(typeObject,CommandQueue,"fillBuffer",4,7, {
		std::vector<Event*> events;
		if(parameter.size() > 5) {
			const EScript::Array * a = parameter[5].to<EScript::Array*>(rt);
			for(auto e : *a)
				events.push_back(e.to<Event*>(rt));
		}
		Event* event = parameter.size() > 6 ? parameter[6].to<Event*>(rt) : nullptr;
		
		Util::TypeConstant type = static_cast<Util::TypeConstant>(parameter[4].toUInt(static_cast<uint32_t>(Util::TypeConstant::FLOAT)));		
		switch(type) {
		case Util::TypeConstant::UINT8:
			return fillBuffer<uint8_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::INT8:
			return fillBuffer<int8_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::UINT16:
			return fillBuffer<uint16_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::INT16:
			return fillBuffer<int16_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::UINT32:
			return fillBuffer<uint32_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::INT32:
			return fillBuffer<int32_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::UINT64:
			return fillBuffer<uint64_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::INT64:
			return fillBuffer<int64_t>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::DOUBLE:
			return fillBuffer<double>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		case Util::TypeConstant::FLOAT:
		default:
			return fillBuffer<float>(thisObj, parameter[0].to<Buffer*>(rt), parameter[1].toUInt(), parameter[3].toUInt(), rt, parameter[2], events, event); break;
		}
	})

	// bool writeBufferRect(Buffer* buffer, bool blocking, const RangeND_t& bufferOffset, const RangeND_t& hostOffset, const RangeND_t& region, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr, size_t bufferRowPitch = 0, size_t bufferSlicePitch = 0, size_t hostRowPitch = 0, size_t hostSlicePitch = 0)
	//! [ESMF] RESULT CommandQueue.writeBufferRect(p0,p1,p2,p3,p4,p5[,p6])
//	ES_MFUN(typeObject,CommandQueue,"writeBufferRect",6,7,
//				EScript::create(thisObj->writeBufferRect(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt),parameter[6].to<XXX>(rt))))

	// bool writeImage(Image* image, bool blocking, const RangeND_t& origin, const RangeND_t& region, void* ptr, const EventList_t& waitForEvents = EventList_t(), Event* event = nullptr, size_t rowPitch = 0, size_t slicePitch = 0)
	//! [ESMF] RESULT CommandQueue.writeImage(p0,p1,p2,p3,p4[,p5])
//	ES_MFUN(typeObject,CommandQueue,"writeImage",5,6,
//				EScript::create(thisObj->writeImage(parameter[0].to<XXX>(rt),parameter[1].to<XXX>(rt),parameter[2].to<XXX>(rt),parameter[3].to<XXX>(rt),parameter[4].to<XXX>(rt),parameter[5].to<XXX>(rt))))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

