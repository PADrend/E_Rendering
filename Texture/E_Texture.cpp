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

	//! Number Texture.getDataSize()
	ES_MFUN(typeObject,Texture,"getDataSize",0,0,
				Number::create(thisObj->getDataSize()))

	//! Number Texture.getWidth()
	ES_MFUN(typeObject,Texture,"getWidth",0,0,
				Number::create(thisObj->getWidth()))

	//! Number Texture.getHeight()
	ES_MFUN(typeObject,Texture,"getHeight",0,0,
				Number::create(thisObj->getHeight()))

	//! thisEObj Texture.download(RenderingContext)
	ES_MFUN(typeObject,Texture,"download",1,1,
				(thisObj->downloadGLTexture(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! thisEObj Texture.allocateLocalData()
	ES_MFUN(typeObject,Texture,"allocateLocalData",0,0,
				(thisObj->allocateLocalData(),thisEObj))

	//! thisEObj Texture.createMipMaps(RenderingContext)
	ES_MFUN(typeObject,Texture,"createMipMaps",1,1,
				(thisObj->createMipMaps(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! thisEObj Texture.dataChanged()
	ES_MFUN(typeObject,Texture,"dataChanged",0,0,
				(thisObj->dataChanged(),thisEObj))

	//! [ESMF] FileName Texture.getFileName()
	ES_MFUN(typeObject,Texture,"getFileName",0,0,
				new E_Util::E_FileName(thisObj->getFileName()))

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

}

//! (ctor)
E_Texture::E_Texture(Rendering::Texture * t):ReferenceObject<Util::Reference<Rendering::Texture> >(t,getTypeObject()){
}

std::string E_Texture::toString() const {
	std::stringstream ss;
	ss << "#Texture:" << reinterpret_cast<void*>(this->get());
	return ss.str();
}

}
