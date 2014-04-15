/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_TextureUtils.h"

#include "E_Texture.h"
#include "../E_RenderingContext.h"

#include <Rendering/Texture/TextureUtils.h>
#include <Rendering/Serialization/Serialization.h>

#include <E_Geometry/E_Rect.h>
#include <E_Util/Graphics/E_Bitmap.h>
#include <E_Util/Graphics/E_PixelAccessor.h>
#include <Util/Graphics/PixelAccessor.h>

#include <EScript/EScript.h>

namespace E_Rendering{

using namespace EScript;
using namespace Rendering;

//! (static)
void initTextureUtils(EScript::Namespace * lib) {

	//! [ESF] void drawTextureToScreen(RenderingContext,screenRect,(Texture,textureRect) | (TextureArray,textureRectArray))
	ES_FUNCTION(lib,"drawTextureToScreen",4,4,{
		const Geometry::Rect_i screenRect(parameter[1].toType<E_Geometry::E_Rect>()->ref());

		Array * textureArray=parameter[2].toType<Array>();
		if(textureArray==nullptr){
			Texture * t= parameter[2].to<Rendering::Texture*>(rt);
			const Geometry::Rect_f & textureRect = parameter[3].toType<E_Geometry::E_Rect>()->ref();
			TextureUtils::drawTextureToScreen(parameter[0].to<RenderingContext&>(rt), screenRect, t, textureRect);
		}else{
			Array * textureRectArray = EScript::assertType<Array>(rt,parameter[3]);
			ERef<Iterator> it1=textureArray->getIterator();
			ERef<Iterator> it2=textureRectArray->getIterator();
			std::vector<Texture *> textures;
			std::vector<Geometry::Rect_f> textureRects;
			for(; !it1->end() && !it2->end()  ; it1->next(),it2->next()){
				ObjRef value1=it1.get()->value();
				ObjRef value2=it2.get()->value();

				Texture * t= value1.to<Rendering::Texture*>(rt);
				const Geometry::Rect_f & textureRect = value2.to<const Geometry::Rect_f&>(rt);

				textures.push_back(t);
				textureRects.push_back(textureRect);
			}
			TextureUtils::drawTextureToScreen(parameter[0].to<RenderingContext&>(rt), screenRect, textures, textureRects);
		}
		return EScript::create(nullptr);
	})

	//! [ESF] Bool compareTextures(Texture, Texture)
	ES_FUNCTION(lib,"compareTextures",2,2,{
		Texture * t1 = parameter[0].to<Rendering::Texture*>(rt);
		Texture * t2 = parameter[1].to<Rendering::Texture*>(rt);
		return Bool::create(TextureUtils::compareTextures(t1,t2));
	})

	//! [ESF] Texture createRedTexture(width,height)
	ES_FUN(lib,"createRedTexture",2,2, TextureUtils::createRedTexture(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)))

	//!	[ESF] E_Bitmap createBitmapFromTexture(RenderingContext,E_Texture)
	ES_FUNCTION(lib,"createBitmapFromTexture",2,2,{
		return EScript::create(Rendering::TextureUtils::createBitmapFromTexture(
																parameter[0].to<RenderingContext&>(rt),
																parameter[1].to<Rendering::Texture*>(rt)));
	})

	//! [ESF] Texture createChessTexture(width,height,fieldSize_powOfTwo,[, Bool useMipmaps = false])
	ES_FUN(lib,"createChessTexture",3,4,
		new E_Texture(TextureUtils::createChessTexture(parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt),parameter[3].toBool(false))))

	//! [ESF] Texture createDepthTexture(Number width, Number height)
	ES_FUN(lib, "createDepthTexture", 2, 2, TextureUtils::createDepthTexture(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)))

	//! [ESF] Texture createHDRTexture(width,height,alpha[,bool mipmap=false])
	ES_FUN(lib,"createHDRTexture",3,4, TextureUtils::createHDRTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool(),parameter[3].toBool(false)))

	//! [ESF] Texture createStdTexture(width,height,alpha[,bool mipmap=false[,bool clampToEdges=false]])
	ES_FUN(lib,"createStdTexture",3,5, TextureUtils::createStdTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool(),parameter[3].toBool(false),parameter[4].toBool(false)))

	//! [ESF] Texture createTextureFromBitmap(Util.Bitmap[,bool mipmap=false[,bool clampToEdges=false]])
	ES_FUN(lib,"createTextureFromBitmap",1,3, TextureUtils::createTextureFromBitmap(
			parameter[0].to<Util::Bitmap &>(rt),parameter[1].toBool(false),parameter[2].toBool(false)))

	//! [ESF] Texture createNoiseTexture(width,height,alpha[,bool mipmap=false[,scaling=1.0]])
	ES_FUN(lib,"createNoiseTexture",3,5, TextureUtils::createNoiseTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool(),parameter[3].toBool(false),parameter[4].toFloat(1.0)))


	//! [ESF] Texture createTextureFromFile(String fileName[, Bool useMipmaps = false[, Bool clampToEdge = false]])
	ES_FUN(lib, "createTextureFromFile", 1, 3, 
		Rendering::Serialization::loadTexture(
			Util::FileName(parameter[0].toString()),
			parameter[1].toBool(false),
			parameter[2].toBool(false)
	))

	//! [ESF] Texture createTextureFromScreen( Rect screenRect,bool alpha=true] |  [bool alpha=true])
	ES_FUNCTION(lib,"createTextureFromScreen",0,2,{
		E_Geometry::E_Rect * er = parameter[0].toType<E_Geometry::E_Rect>();
		if(er!=nullptr){
			return EScript::create(TextureUtils::createTextureFromScreen(
									(**er).getMinX(),(**er).getMinY(), (**er).getWidth(), (**er).getHeight(),parameter[1].toBool(true)));
		}else{
			assertParamCount(rt,parameter,0,1);
			return EScript::create(TextureUtils::createTextureFromScreen(0,0,-1,-1,parameter[0].toBool(true)));
		}
	})

	//! [ESF] PixelAccessor|Void createColorPixelAccessor(RenderingContext, Texture)
	ES_FUNCTION(lib, "createColorPixelAccessor", 2, 2, {
		Util::Reference<Util::PixelAccessor> acc = TextureUtils::createColorPixelAccessor(
			parameter[0].to<RenderingContext&>(rt),
			parameter[1].to<Rendering::Texture*>(rt));
		if(acc != nullptr) {
			return EScript::create(acc);
		} else {
			return EScript::create(nullptr);
		}
	})

	//! [ESF] PixelAccessor|Void createDepthPixelAccessor(RenderingContext, Texture)
	ES_FUNCTION(lib, "createDepthPixelAccessor", 2, 2, {
		Util::Reference<Util::PixelAccessor> acc = TextureUtils::createDepthPixelAccessor(
			parameter[0].to<RenderingContext&>(rt),
			parameter[1].to<Rendering::Texture*>(rt));
		if(acc != nullptr) {
			return EScript::create(acc);
		} else {
			return EScript::create(nullptr);
		}
	})

	//! [ESF] PixelAccessor|Void createStencilPixelAccessor(RenderingContext, Texture)
	ES_FUNCTION(lib, "createStencilPixelAccessor", 2, 2, {
		Util::Reference<Util::PixelAccessor> acc = TextureUtils::createStencilPixelAccessor(
			parameter[0].to<RenderingContext&>(rt),
			parameter[1].to<Rendering::Texture*>(rt));
		if(acc != nullptr) {
			return EScript::create(acc);
		} else {
			return EScript::create(nullptr);
		}
	})

	//! [ESF] Number minDepthDistance(RenderingContext, Texture, Texture);
	ES_FUN(lib, "minDepthDistance", 3, 3,
		TextureUtils::minDepthDistance(parameter[0].to<RenderingContext&>(rt),
															parameter[1].to<Rendering::Texture*>(rt),
															parameter[2].to<Rendering::Texture*>(rt)))

	//! [ESF] Bool saveTexture(RenderingContext,Texture tex, String fileName)
	ES_FUN(lib, "saveTexture", 3, 3,
		Rendering::Serialization::saveTexture(
			parameter[0].to<RenderingContext&>(rt),
			parameter[1].to<Rendering::Texture*>(rt),
			Util::FileName(parameter[2].toString())))

	//! [ESF] void updateTextureFromScreen( RenderingContext,[Texture,textureRect,screenX=0,screenY=0] )
	ES_FUNCTION(lib,"updateTextureFromScreen",2,5,{
		Texture * t= parameter[1].to<Rendering::Texture*>(rt);
		if(parameter.count()==1){
			TextureUtils::updateTextureFromScreen(parameter[0].to<RenderingContext&>(rt),t);
		}else{
			Geometry::Rect_i textureRect(parameter[2].toType<E_Geometry::E_Rect>()->ref());
			TextureUtils::updateTextureFromScreen(parameter[0].to<RenderingContext&>(rt), t,textureRect,parameter[3].toInt(0),parameter[4].toInt(0));
		}
		return EScript::create(nullptr);
	})

}
}
