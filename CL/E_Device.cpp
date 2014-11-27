#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Device.h"
#include "E_Platform.h"

#include <Rendering/CL/Platform.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Device::getTypeObject() {
	// E_Device ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Device::init(EScript::Namespace & lib) {
	using namespace Rendering::CL;
	EScript::Type * typeObject = E_Device::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);


	declareConstant(&lib, "TYPE_DEFAULT", Device::TYPE_DEFAULT);
	declareConstant(&lib, "TYPE_ACCELERATOR", Device::TYPE_ACCELERATOR);
	declareConstant(&lib, "TYPE_ALL", Device::TYPE_ALL);
	declareConstant(&lib, "TYPE_CPU", Device::TYPE_CPU);
	declareConstant(&lib, "TYPE_GPU", Device::TYPE_GPU);
	declareConstant(&lib, "TYPE_CUSTOM", Device::TYPE_CUSTOM);
	
	// std::vector<DeviceRef> createSubDevices(const std::vector<intptr_t>& properties)
	//! [ESMF] RESULT Device.createSubDevices(p0)
//	ES_MFUNCTION(typeObject,Device,"createSubDevices",1,1, {
//		auto devices = thisObj->createSubDevices({});
//		auto arr = EScript::Array::create();
//		for(auto dev : devices)
//			arr->pushBack(EScript::create(dev.get()));
//		return arr;
//	})

	// uint32_t getAddressBits() const
	//! [ESMF] RESULT Device.getAddressBits()
	ES_MFUN(typeObject,const Device,"getAddressBits",0,0,
				EScript::create(thisObj->getAddressBits()))

	// std::string getBuiltInKernels() const
	//! [ESMF] RESULT Device.getBuiltInKernels()
	ES_MFUN(typeObject,const Device,"getBuiltInKernels",0,0,
				EScript::create(thisObj->getBuiltInKernels()))

	// uint32_t getDoubleFPConfig() const; //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getDoubleFPConfig()
	ES_MFUN(typeObject,Device,"getDoubleFPConfig",0,0,
				EScript::create(thisObj->getDoubleFPConfig()))

	// std::string getDriverVersion() const
	//! [ESMF] RESULT Device.getDriverVersion()
	ES_MFUN(typeObject,const Device,"getDriverVersion",0,0,
				EScript::create(thisObj->getDriverVersion()))

	// uint32_t getExecutionCapabilities() const; //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getExecutionCapabilities()
	ES_MFUN(typeObject,Device,"getExecutionCapabilities",0,0,
				EScript::create(thisObj->getExecutionCapabilities()))

	// std::string getExtensions() const
	//! [ESMF] RESULT Device.getExtensions()
	ES_MFUN(typeObject,const Device,"getExtensions",0,0,
				EScript::create(thisObj->getExtensions()))

	// size_t getGlobalMemCacheSize() const
	//! [ESMF] RESULT Device.getGlobalMemCacheSize()
	ES_MFUN(typeObject,const Device,"getGlobalMemCacheSize",0,0,
				EScript::Number::create(thisObj->getGlobalMemCacheSize()))

	// CacheType_t getGlobalMemCacheType() const
	//! [ESMF] RESULT Device.getGlobalMemCacheType()
	ES_MFUN(typeObject,const Device,"getGlobalMemCacheType",0,0,
				EScript::create(static_cast<uint32_t>(thisObj->getGlobalMemCacheType())))

	// uint32_t getGlobalMemCachelineSize() const
	//! [ESMF] RESULT Device.getGlobalMemCachelineSize()
	ES_MFUN(typeObject,const Device,"getGlobalMemCachelineSize",0,0,
				EScript::create(thisObj->getGlobalMemCachelineSize()))

	// size_t getGlobalMemSize() const
	//! [ESMF] RESULT Device.getGlobalMemSize()
	ES_MFUN(typeObject,const Device,"getGlobalMemSize",0,0,
				EScript::Number::create(thisObj->getGlobalMemSize()))

	// uint32_t getHalfFPConfig() const; //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getHalfFPConfig()
	ES_MFUN(typeObject,Device,"getHalfFPConfig",0,0,
				EScript::create(thisObj->getHalfFPConfig()))

	// size_t getImage2DMaxHeight() const
	//! [ESMF] RESULT Device.getImage2DMaxHeight()
	ES_MFUN(typeObject,const Device,"getImage2DMaxHeight",0,0,
				EScript::Number::create(thisObj->getImage2DMaxHeight()))

	// size_t getImage2DMaxWidth() const
	//! [ESMF] RESULT Device.getImage2DMaxWidth()
	ES_MFUN(typeObject,const Device,"getImage2DMaxWidth",0,0,
			EScript::Number::create(thisObj->getImage2DMaxWidth()))

	// size_t getImage3DMaxDepth() const
	//! [ESMF] RESULT Device.getImage3DMaxDepth()
	ES_MFUN(typeObject,const Device,"getImage3DMaxDepth",0,0,
			EScript::Number::create(thisObj->getImage3DMaxDepth()))

	// size_t getImage3DMaxHeight() const
	//! [ESMF] RESULT Device.getImage3DMaxHeight()
	ES_MFUN(typeObject,const Device,"getImage3DMaxHeight",0,0,
			EScript::Number::create(thisObj->getImage3DMaxHeight()))

	// size_t getImage3DMaxWidth() const
	//! [ESMF] RESULT Device.getImage3DMaxWidth()
	ES_MFUN(typeObject,const Device,"getImage3DMaxWidth",0,0,
			EScript::Number::create(thisObj->getImage3DMaxWidth()))

	// //	size_t getImageMaxArraySize() const
	//! [ESMF] RESULT Device.getImageMaxArraySize()
//	ES_MFUN(typeObject,const Device,"getImageMaxArraySize",0,0,
//				EScript::create(thisObj->getImageMaxArraySize()))

	// //	size_t getImageMaxBufferSize() const
	//! [ESMF] RESULT Device.getImageMaxBufferSize()
//	ES_MFUN(typeObject,const Device,"getImageMaxBufferSize",0,0,
//				EScript::create(thisObj->getImageMaxBufferSize()))

	// size_t getLocalMemSize() const
	//! [ESMF] RESULT Device.getLocalMemSize()
	ES_MFUN(typeObject,const Device,"getLocalMemSize",0,0,
			EScript::Number::create(thisObj->getLocalMemSize()))

	// MemType_t getLocalMemType() const
	//! [ESMF] RESULT Device.getLocalMemType()
	ES_MFUN(typeObject,const Device,"getLocalMemType",0,0,
			EScript::Number::create(thisObj->getLocalMemType()))

	// uint32_t getMaxClockFrequency() const
	//! [ESMF] RESULT Device.getMaxClockFrequency()
	ES_MFUN(typeObject,const Device,"getMaxClockFrequency",0,0,
				EScript::create(thisObj->getMaxClockFrequency()))

	// uint32_t getMaxComputeUnits() const
	//! [ESMF] RESULT Device.getMaxComputeUnits()
	ES_MFUN(typeObject,const Device,"getMaxComputeUnits",0,0,
				EScript::create(thisObj->getMaxComputeUnits()))

	// uint32_t getMaxConstantArgs() const
	//! [ESMF] RESULT Device.getMaxConstantArgs()
	ES_MFUN(typeObject,const Device,"getMaxConstantArgs",0,0,
				EScript::create(thisObj->getMaxConstantArgs()))

	// size_t getMaxConstantBufferSize() const
	//! [ESMF] RESULT Device.getMaxConstantBufferSize()
	ES_MFUN(typeObject,const Device,"getMaxConstantBufferSize",0,0,
			EScript::Number::create(thisObj->getMaxConstantBufferSize()))

	// size_t getMaxMemAllocSize() const
	//! [ESMF] RESULT Device.getMaxMemAllocSize()
	ES_MFUN(typeObject,const Device,"getMaxMemAllocSize",0,0,
			EScript::Number::create(thisObj->getMaxMemAllocSize()))

	// size_t getMaxParameterSize() const
	//! [ESMF] RESULT Device.getMaxParameterSize()
	ES_MFUN(typeObject,const Device,"getMaxParameterSize",0,0,
			EScript::Number::create(thisObj->getMaxParameterSize()))

	// uint32_t getMaxReadImageArgs() const
	//! [ESMF] RESULT Device.getMaxReadImageArgs()
	ES_MFUN(typeObject,const Device,"getMaxReadImageArgs",0,0,
			EScript::Number::create(thisObj->getMaxReadImageArgs()))

	// uint32_t getMaxSamplers() const
	//! [ESMF] RESULT Device.getMaxSamplers()
	ES_MFUN(typeObject,const Device,"getMaxSamplers",0,0,
			EScript::Number::create(thisObj->getMaxSamplers()))

	// size_t getMaxWorkGroupSize() const
	//! [ESMF] RESULT Device.getMaxWorkGroupSize()
	ES_MFUN(typeObject,const Device,"getMaxWorkGroupSize",0,0,
			EScript::Number::create(thisObj->getMaxWorkGroupSize()))

	// uint32_t getMaxWorkItemDimensions() const
	//! [ESMF] RESULT Device.getMaxWorkItemDimensions()
	ES_MFUN(typeObject,const Device,"getMaxWorkItemDimensions",0,0,
			EScript::Number::create(thisObj->getMaxWorkItemDimensions()))

	// std::vector<size_t> getMaxWorkItemSizes() const
	//! [ESMF] RESULT Device.getMaxWorkItemSizes()
	ES_MFUNCTION(typeObject,const Device,"getMaxWorkItemSizes",0,0, {
		auto sizes = thisObj->getMaxWorkItemSizes();
		auto arr = EScript::Array::create();
		for(auto s : sizes)
			arr->pushBack(EScript::Number::create(s));
		return arr;
	})

	// uint32_t getMaxWriteImageArgs() const
	//! [ESMF] RESULT Device.getMaxWriteImageArgs()
	ES_MFUN(typeObject,const Device,"getMaxWriteImageArgs",0,0,
				EScript::create(thisObj->getMaxWriteImageArgs()))

	// uint32_t getMemBaseAddrAlign() const
	//! [ESMF] RESULT Device.getMemBaseAddrAlign()
	ES_MFUN(typeObject,const Device,"getMemBaseAddrAlign",0,0,
				EScript::create(thisObj->getMemBaseAddrAlign()))

	// uint32_t getMinDataTypeAlignSize() const
	//! [ESMF] RESULT Device.getMinDataTypeAlignSize()
	ES_MFUN(typeObject,const Device,"getMinDataTypeAlignSize",0,0,
				EScript::create(thisObj->getMinDataTypeAlignSize()))

	// std::string getName() const
	//! [ESMF] RESULT Device.getName()
	ES_MFUN(typeObject,const Device,"getName",0,0,
				EScript::create(thisObj->getName()))

	// uint32_t getNativeVectorWidthChar() const
	//! [ESMF] RESULT Device.getNativeVectorWidthChar()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthChar",0,0,
				EScript::create(thisObj->getNativeVectorWidthChar()))

	// uint32_t getNativeVectorWidthDouble() const
	//! [ESMF] RESULT Device.getNativeVectorWidthDouble()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthDouble",0,0,
				EScript::create(thisObj->getNativeVectorWidthDouble()))

	// uint32_t getNativeVectorWidthFloat() const
	//! [ESMF] RESULT Device.getNativeVectorWidthFloat()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthFloat",0,0,
				EScript::create(thisObj->getNativeVectorWidthFloat()))

	// uint32_t getNativeVectorWidthHalf() const
	//! [ESMF] RESULT Device.getNativeVectorWidthHalf()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthHalf",0,0,
				EScript::create(thisObj->getNativeVectorWidthHalf()))

	// uint32_t getNativeVectorWidthInt() const
	//! [ESMF] RESULT Device.getNativeVectorWidthInt()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthInt",0,0,
				EScript::create(thisObj->getNativeVectorWidthInt()))

	// uint32_t getNativeVectorWidthLong() const
	//! [ESMF] RESULT Device.getNativeVectorWidthLong()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthLong",0,0,
				EScript::create(thisObj->getNativeVectorWidthLong()))

	// uint32_t getNativeVectorWidthShort() const
	//! [ESMF] RESULT Device.getNativeVectorWidthShort()
	ES_MFUN(typeObject,const Device,"getNativeVectorWidthShort",0,0,
				EScript::create(thisObj->getNativeVectorWidthShort()))

	// std::string getOpenCL_CVersion() const
	//! [ESMF] RESULT Device.getOpenCL_CVersion()
	ES_MFUN(typeObject,const Device,"getOpenCL_CVersion",0,0,
				EScript::create(thisObj->getOpenCL_CVersion()))

	// Device getParentDevice() const
	//! [ESMF] RESULT Device.getParentDevice()
//	ES_MFUN(typeObject,const Device,"getParentDevice",0,0,
//				EScript::create(thisObj->getParentDevice()))

	// uint32_t getPartitionAffinityDomain() const;  //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getPartitionAffinityDomain()
//	ES_MFUN(typeObject,Device,"getPartitionAffinityDomain",0,0,
//				EScript::create(thisObj->getPartitionAffinityDomain()))

	// //	uint32_t getPartitionMaxSubDevices() const
	//! [ESMF] RESULT Device.getPartitionMaxSubDevices()
//	ES_MFUN(typeObject,const Device,"getPartitionMaxSubDevices",0,0,
//				EScript::create(thisObj->getPartitionMaxSubDevices()))

	// std::vector<intptr_t> getPartitionProperties() const;  //TODO: use enums
	//! [ESMF] RESULT Device.getPartitionProperties()
//	ES_MFUN(typeObject,Device,"getPartitionProperties",0,0,
//				EScript::create(thisObj->getPartitionProperties()))

	// std::vector<intptr_t> getPartitionType() const;  //TODO: use enums
	//! [ESMF] RESULT Device.getPartitionType()
//	ES_MFUN(typeObject,Device,"getPartitionType",0,0,
//				EScript::create(thisObj->getPartitionType()))

	// Platform getPlatform() const
	//! [ESMF] RESULT Device.getPlatform()
	ES_MFUN(typeObject,const Device,"getPlatform",0,0,
				EScript::create(thisObj->getPlatform()))

	// uint32_t getPreferredVectorWidthChar() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthChar()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthChar",0,0,
				EScript::create(thisObj->getPreferredVectorWidthChar()))

	// uint32_t getPreferredVectorWidthDouble() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthDouble()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthDouble",0,0,
				EScript::create(thisObj->getPreferredVectorWidthDouble()))

	// uint32_t getPreferredVectorWidthFloat() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthFloat()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthFloat",0,0,
				EScript::create(thisObj->getPreferredVectorWidthFloat()))

	// uint32_t getPreferredVectorWidthHalf() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthHalf()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthHalf",0,0,
				EScript::create(thisObj->getPreferredVectorWidthHalf()))

	// uint32_t getPreferredVectorWidthInt() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthInt()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthInt",0,0,
				EScript::create(thisObj->getPreferredVectorWidthInt()))

	// uint32_t getPreferredVectorWidthLong() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthLong()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthLong",0,0,
				EScript::create(thisObj->getPreferredVectorWidthLong()))

	// uint32_t getPreferredVectorWidthShort() const
	//! [ESMF] RESULT Device.getPreferredVectorWidthShort()
	ES_MFUN(typeObject,const Device,"getPreferredVectorWidthShort",0,0,
				EScript::create(thisObj->getPreferredVectorWidthShort()))

	// std::string getProfile() const
	//! [ESMF] RESULT Device.getProfile()
	ES_MFUN(typeObject,const Device,"getProfile",0,0,
				EScript::create(thisObj->getProfile()))

	// size_t getProfilingTimerResolution() const
	//! [ESMF] RESULT Device.getProfilingTimerResolution()
	ES_MFUN(typeObject,const Device,"getProfilingTimerResolution",0,0,
			EScript::Number::create(thisObj->getProfilingTimerResolution()))

	// uint32_t getQueueProperties() const; //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getQueueProperties()
	ES_MFUN(typeObject,Device,"getQueueProperties",0,0,
				EScript::create(thisObj->getQueueProperties()))

	// uint32_t getSingleFPConfig() const; //TODO: use string or bitfield
	//! [ESMF] RESULT Device.getSingleFPConfig()
	ES_MFUN(typeObject,Device,"getSingleFPConfig",0,0,
				EScript::create(thisObj->getSingleFPConfig()))

	// uint32_t getType() const
	//! [ESMF] RESULT Device.getType()
	ES_MFUN(typeObject,const Device,"getType",0,0,
				EScript::create(thisObj->getType()))

	// std::string getVendor() const
	//! [ESMF] RESULT Device.getVendor()
	ES_MFUN(typeObject,const Device,"getVendor",0,0,
				EScript::create(thisObj->getVendor()))

	// uint32_t getVendorId() const
	//! [ESMF] RESULT Device.getVendorId()
	ES_MFUN(typeObject,const Device,"getVendorId",0,0,
				EScript::create(thisObj->getVendorId()))

	// std::string getVersion() const
	//! [ESMF] RESULT Device.getVersion()
	ES_MFUN(typeObject,const Device,"getVersion",0,0,
				EScript::create(thisObj->getVersion()))

	// bool hasHostUnifiedMemory() const
	//! [ESMF] RESULT Device.hasHostUnifiedMemory()
	ES_MFUN(typeObject,const Device,"hasHostUnifiedMemory",0,0,
				EScript::create(thisObj->hasHostUnifiedMemory()))

	// bool isAvailable() const
	//! [ESMF] RESULT Device.isAvailable()
	ES_MFUN(typeObject,const Device,"isAvailable",0,0,
				EScript::create(thisObj->isAvailable()))

	// bool isCompilerAvailable() const
	//! [ESMF] RESULT Device.isCompilerAvailable()
	ES_MFUN(typeObject,const Device,"isCompilerAvailable",0,0,
				EScript::create(thisObj->isCompilerAvailable()))

	// bool isEndianLittle() const
	//! [ESMF] RESULT Device.isEndianLittle()
	ES_MFUN(typeObject,const Device,"isEndianLittle",0,0,
				EScript::create(thisObj->isEndianLittle()))

	// bool isErrorCorrectionSupported() const
	//! [ESMF] RESULT Device.isErrorCorrectionSupported()
	ES_MFUN(typeObject,const Device,"isErrorCorrectionSupported",0,0,
				EScript::create(thisObj->isErrorCorrectionSupported()))

	// bool isImageSupported() const
	//! [ESMF] RESULT Device.isImageSupported()
	ES_MFUN(typeObject,const Device,"isImageSupported",0,0,
				EScript::create(thisObj->isImageSupported()))

	// bool isInteropUserSyncPreferred() const
	//! [ESMF] RESULT Device.isInteropUserSyncPreferred()
	ES_MFUN(typeObject,const Device,"isInteropUserSyncPreferred",0,0,
				EScript::create(thisObj->isInteropUserSyncPreferred()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

