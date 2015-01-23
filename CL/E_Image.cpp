/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#include "E_Image.h"
#include "E_Context.h"

#include "../Texture/E_Texture.h"
#include <E_Util/Graphics/E_Bitmap.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering{
namespace E_CL{


//! (static)
EScript::Type * E_Image::getTypeObject() {
	// E_Image ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(E_Memory::getTypeObject());
	return typeObject.get();
}

//! (static) init members 
void E_Image::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = E_Image::getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	using namespace Rendering::CL;
	
	//!	[ESMF] Image new Image()
	ES_CONSTRUCTOR(typeObject,3,3,{
		Context* context = parameter[0].to<Context*>(rt);
		auto texture = parameter[2].toType<E_Texture>();
		if(texture) {
			return new E_Image(new Image(context, static_cast<ReadWrite_t>(parameter[1].to<uint32_t>(rt)), (**texture).get()));
		}
		auto bmp = parameter[2].to<Util::Reference<Util::Bitmap>>(rt);
		return new E_Image(new Image(context, static_cast<ReadWrite_t>(parameter[1].to<uint32_t>(rt)), bmp.get()));
	});

	// size_t getDepth() const
	//! [ESMF] RESULT Image.getDepth()
	ES_MFUN(typeObject,const Image,"getDepth",0,0,
			EScript::Number::create(thisObj->getDepth()))

	// size_t getElementSize() const
	//! [ESMF] RESULT Image.getElementSize()
	ES_MFUN(typeObject,const Image,"getElementSize",0,0,
			EScript::Number::create(thisObj->getElementSize()))

	// uint32_t getGLTextureTarget() const
	//! [ESMF] RESULT Image.getGLTextureTarget()
	ES_MFUN(typeObject,const Image,"getGLTextureTarget",0,0,
			EScript::Number::create(thisObj->getGLTextureTarget()))

	// size_t getHeight() const
	//! [ESMF] RESULT Image.getHeight()
	ES_MFUN(typeObject,const Image,"getHeight",0,0,
			EScript::Number::create(thisObj->getHeight()))

	// uint32_t getMipmapLevel() const
	//! [ESMF] RESULT Image.getMipmapLevel()
	ES_MFUN(typeObject,const Image,"getMipmapLevel",0,0,
				EScript::create(thisObj->getMipmapLevel()))

	// uint32_t getNumMipLevels() const
	//! [ESMF] RESULT Image.getNumMipLevels()
	ES_MFUN(typeObject,const Image,"getNumMipLevels",0,0,
				EScript::create(thisObj->getNumMipLevels()))

	// uint32_t getNumSamples() const
	//! [ESMF] RESULT Image.getNumSamples()
	ES_MFUN(typeObject,const Image,"getNumSamples",0,0,
				EScript::create(thisObj->getNumSamples()))

	// PixelFormatCL getPixelFormat() const
	//! [ESMF] RESULT Image.getPixelFormat()
	ES_MFUNCTION(typeObject,const Image,"getPixelFormat",0,0,{
		PixelFormatCL format = thisObj->getPixelFormat();
		auto arr = EScript::Array::create();
		arr->pushBack(EScript::Number::create(static_cast<uint8_t>(format.channelOrder)));
		arr->pushBack(EScript::Number::create(static_cast<uint8_t>(format.channelType)));
	})

	// size_t getRowPitch() const
	//! [ESMF] RESULT Image.getRowPitch()
	ES_MFUN(typeObject,const Image,"getRowPitch",0,0,
			EScript::Number::create(thisObj->getRowPitch()))

	// size_t getSlicePitch() const
	//! [ESMF] RESULT Image.getSlicePitch()
	ES_MFUN(typeObject,const Image,"getSlicePitch",0,0,
			EScript::Number::create(thisObj->getSlicePitch()))

	// ImageType getType() const
	//! [ESMF] RESULT Image.getType()
	ES_MFUN(typeObject,const Image,"getType",0,0,
			EScript::Number::create(static_cast<uint8_t>(thisObj->getType())))

	// size_t getWidth() const
	//! [ESMF] RESULT Image.getWidth()
	ES_MFUN(typeObject,const Image,"getWidth",0,0,
			EScript::Number::create(thisObj->getWidth()))

}
}

}
#endif // RENDERING_HAS_LIB_OPENCL

