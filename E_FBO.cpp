/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_FBO.h"

#include "Texture/E_Texture.h"
#include "E_RenderingContext.h"

#include <E_Geometry/E_Rect.h>
#include <Geometry/Rect.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>
using namespace EScript;
using namespace Rendering;

namespace E_Rendering {

//! (static)
EScript::Type * E_FBO::getTypeObject() {
	// E_FBO ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_FBO::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	//! [ESMF] new Rendering.FBO()
	ES_CTOR(typeObject,0,0,new FBO)

	//! [ESMF] thisEObj FBO.attachColorTexture(RenderingContext,Texture,colorBufferId=0,level=0,layer=0)
	ES_MFUN(typeObject,FBO,"attachColorTexture",2,5,(thisObj->
			attachColorTexture(parameter[0].to<RenderingContext&>(rt),
								parameter[1].to<Rendering::Texture*>(rt),
								static_cast<uint8_t>(parameter[2].to<uint32_t>(rt)),parameter[3].toUInt(0),parameter[4].toInt(-1)),thisEObj))

	//! [ESMF] thisEObj FBO.attachDepthStencilTexture(RenderingContext, Texture,level=0,layer=0)
	ES_MFUN(typeObject,FBO, "attachDepthStencilTexture", 2, 4,(thisObj->
			attachDepthStencilTexture(parameter[0].to<RenderingContext&>(rt),
								parameter[1].to<Rendering::Texture*>(rt),parameter[2].toUInt(0),parameter[3].toInt(-1)), thisEObj))

	//! [ESMF] thisEObj FBO.attachDepthTexture(RenderingContext,Texture,level=0,layer=0)
	ES_MFUN(typeObject,FBO,"attachDepthTexture",2,4,(thisObj->
			attachDepthTexture(parameter[0].to<RenderingContext&>(rt),
								parameter[1].to<Rendering::Texture*>(rt),parameter[2].toUInt(0),parameter[3].toInt(-1)),thisEObj))

	//! [ESMF] thisEObj FBO.attachTexture(RenderingContext,attachmentPoint,Texture, level=0,layer=0)
	ES_MFUN(typeObject,FBO,"attachTexture",3,5,(thisObj->
			attachTexture(parameter[0].to<RenderingContext&>(rt),
							parameter[1].to<uint32_t>(rt), parameter[2].to<Rendering::Texture*>(rt),parameter[3].toUInt(0),parameter[4].toInt(-1)),thisEObj))

	//! [ESMF] thisEObj FBO.detachColorTexture(RenderingContext,colorBufferId=0)
	ES_MFUN(typeObject,FBO,"detachColorTexture",1,2,(thisObj->
			detachColorTexture(parameter[0].to<RenderingContext&>(rt),
								parameter[1].to<uint32_t>(rt,0)),thisEObj))

	//! [ESMF] thisEObj FBO.detachDepthStencilTexture(RenderingContext)
	ES_MFUN(typeObject,FBO,"detachDepthStencilTexture",1,1,(thisObj->
			detachDepthStencilTexture(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! [ESMF] thisEObj FBO.detachDepthTexture(RenderingContext)
	ES_MFUN(typeObject,FBO,"detachDepthTexture",1,1,(thisObj->
			detachDepthTexture(parameter[0].to<RenderingContext&>(rt)),thisEObj))

	//! [ESMF] thisEObj FBO.detachTexture(RenderingContext,attachmentPoint)
	ES_MFUN(typeObject,FBO,"detachTexture",2,2,(thisObj->
			detachTexture(parameter[0].to<RenderingContext&>(rt),
							parameter[1].to<uint32_t>(rt)),thisEObj))

	//! [ESMF] thisEObj FBO.getStatusMessage(RenderingContext)
	ES_MFUN(typeObject,FBO,"getStatusMessage",1,1,String::create(thisObj->getStatusMessage(
							parameter[0].to<RenderingContext&>(rt))))

	//! [ESMF] Bool FBO.isComplete(RenderingContext)
	ES_MFUN(typeObject,FBO,"isComplete",1,1,Bool::create(thisObj->isComplete(
							parameter[0].to<RenderingContext&>(rt))))

	//! [ESMF] thisEObj FBO.setDrawBuffers(RenderingContext, Number)
	ES_MFUN(typeObject,FBO, "setDrawBuffers", 2, 2, (thisObj->setDrawBuffers(parameter[0].to<RenderingContext&>(rt), parameter[1].toUInt()),thisEObj))
	
		//! [ESMF] thisEObj FBO.blitToScreen(RenderingContext,Rect,Rect)
		ES_MFUN(typeObject,FBO,"blitToScreen",3,4,(thisObj->
				blitToScreen(	parameter[0].to<RenderingContext&>(rt),
							Geometry::Rect_i(parameter[1].to<Geometry::Rect&>(rt)), Geometry::Rect_i(parameter[2].to<Geometry::Rect&>(rt))),thisEObj))

}

}
