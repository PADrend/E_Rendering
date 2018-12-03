/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Texture.h"
#include "../E_RenderingContext.h"

#include <Rendering/Texture/Texture.h>
#include <E_Util/Graphics/E_Color.h>
#include <E_Util/E_FileName.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>
#include <sstream>

using namespace EScript;
using namespace Rendering;

namespace E_Rendering {

//! (static)
EScript::Type * E_Texture::getTypeObject() {
	// E_Texture ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_Texture::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! thisEObj Texture.allocateLocalData()
	ES_MFUN(typeObject,Texture,"allocateLocalData",0,0,	(thisObj->allocateLocalData(),thisEObj))

	//! thisEObj Texture.createMipmaps(RenderingContext)
	ES_MFUN(typeObject,Texture,"createMipmaps",1,1, (thisObj->createMipmaps(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! thisEObj Texture.dataChanged()
	ES_MFUN(typeObject,Texture,"dataChanged",0,0, (thisObj->dataChanged(),thisEObj))

	//! thisEObj Texture.download(RenderingContext)
	ES_MFUN(typeObject,Texture,"download",1,1, (thisObj->downloadGLTexture(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! thisEObj Texture._upload(RenderingContext)
	ES_MFUN(typeObject,Texture,"_upload",1,1, (thisObj->_uploadGLTexture(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! thisEObj Texture.clear([Color])
	ES_MFUN(typeObject,Texture,"clear",0,1, (thisObj->clearGLData(parameter[0].to(rt,Util::Color4f())),thisEObj))

	//! [ESMF] FileName Texture.getHasMipmaps()
	ES_MFUN(typeObject,const Texture,"getHasMipmaps",0,0, thisObj->getHasMipmaps())

	//! [ESMF] FileName Texture.getFileName()
	ES_MFUN(typeObject,const Texture,"getFileName",0,0,	thisObj->getFileName())

	//! Number Texture.getDataSize()
	ES_MFUN(typeObject,const Texture,"getDataSize",0,0,thisObj->getDataSize())

	//! Number Texture.getHeight()
	ES_MFUN(typeObject,const Texture,"getHeight",0,0, thisObj->getHeight())

	//! Number Texture.getNumLayers()
	ES_MFUN(typeObject,const Texture,"getNumLayers",0,0, thisObj->getNumLayers())
	
	//! Number Texture.getTextureType()
	ES_MFUN(typeObject,const Texture,"getTextureType",0,0, static_cast<uint32_t>(thisObj->getTextureType()))
	
	//! Number Texture.getWidth()
	ES_MFUN(typeObject,const Texture,"getWidth",0,0, thisObj->getWidth())
	
	//! Bool Texture.getUseLinearMinFilter()
	ES_MFUN(typeObject,const Texture,"getUseLinearMinFilter",0,0, thisObj->getUseLinearMinFilter())
	
	//! Bool Texture.getUseLinearMagFilter()
	ES_MFUN(typeObject,const Texture,"getUseLinearMagFilter",0,0, thisObj->getUseLinearMagFilter())
	
	//! self Texture.planMipmapCreation()
	ES_MFUN(typeObject,Texture,"planMipmapCreation",0,0, (thisObj->planMipmapCreation(),thisEObj))

	//! [ESMF] thisEObj FileName.setFileName([[FileName, String] name])
	ES_MFUNCTION(typeObject,Texture,"setFileName",0,1,{
				if(parameter.count() == 0){
					thisObj->setFileName(Util::FileName());
				}
				else{
					E_Util::E_FileName * t = dynamic_cast<E_Util::E_FileName *>(parameter[0].get());
					if(t){
						thisObj->setFileName(t->ref());
					}
					else{
						thisObj->setFileName(Util::FileName(parameter[0].toString()));
					}
				}
				return thisEObj;
	})
	
	EScript::declareConstant( typeObject, "TEXTURE_1D",				static_cast<uint32_t>(TextureType::TEXTURE_1D) );
	EScript::declareConstant( typeObject, "TEXTURE_1D_ARRAY",		static_cast<uint32_t>(TextureType::TEXTURE_1D_ARRAY) );
	EScript::declareConstant( typeObject, "TEXTURE_2D",				static_cast<uint32_t>(TextureType::TEXTURE_2D) );
	EScript::declareConstant( typeObject, "TEXTURE_2D_ARRAY",		static_cast<uint32_t>(TextureType::TEXTURE_2D_ARRAY) );
	EScript::declareConstant( typeObject, "TEXTURE_3D",				static_cast<uint32_t>(TextureType::TEXTURE_3D) );
	EScript::declareConstant( typeObject, "TEXTURE_CUBE_MAP",		static_cast<uint32_t>(TextureType::TEXTURE_CUBE_MAP) );
	EScript::declareConstant( typeObject, "TEXTURE_CUBE_MAP_ARRAY",	static_cast<uint32_t>(TextureType::TEXTURE_CUBE_MAP_ARRAY) );
	EScript::declareConstant( typeObject, "TEXTURE_BUFFER",			static_cast<uint32_t>(TextureType::TEXTURE_BUFFER) );
	EScript::declareConstant( typeObject, "TEXTURE_2D_MULTISAMPLE",	static_cast<uint32_t>(TextureType::TEXTURE_2D_MULTISAMPLE) );

}

E_Texture::E_Texture(Rendering::Texture * t) : ReferenceObject<Util::Reference<Rendering::Texture> >(t,getTypeObject()){}
E_Texture::E_Texture(const Util::Reference<Rendering::Texture> & t) : ReferenceObject<Util::Reference<Rendering::Texture> >(t,getTypeObject()){}
E_Texture::E_Texture(Util::Reference<Rendering::Texture> && t) : ReferenceObject<Util::Reference<Rendering::Texture> >(t,getTypeObject()){}

std::string E_Texture::toString() const {
	std::stringstream ss;
	ss << "#Texture:" << reinterpret_cast<void*>(this->get());
	return ss.str();
}

}
