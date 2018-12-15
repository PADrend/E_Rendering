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
#include <Rendering/Texture/Texture.h>
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
			Texture & t= parameter[2].to<Rendering::Texture&>(rt);
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

	//! [ESF] Bool haveSameFormat(Texture, Texture)
	ES_FUNCTION(lib,"haveSameFormat",2,2,{
		auto f1 = parameter[0].to<Rendering::Texture*>(rt)->getFormat();
		auto f2 = parameter[1].to<Rendering::Texture*>(rt)->getFormat();
		return Bool::create(f1 == f2);
	})

	//! [ESF] Texture createRedTexture(width,height)
	ES_FUN(lib,"createRedTexture",2,2, TextureUtils::createRedTexture(parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt)))

	//!	[ESF] E_Bitmap createBitmapFromTexture(RenderingContext,E_Texture)
	ES_FUNCTION(lib,"createBitmapFromTexture",2,2,{
		return EScript::create(Rendering::TextureUtils::createBitmapFromTexture(
																parameter[0].to<RenderingContext&>(rt),
																*parameter[1].to<Rendering::Texture*>(rt)));
	})

	//! [ESF] Texture createChessTexture(width,height,fieldSize_powOfTwo)
	ES_FUN(lib,"createChessTexture",3,3,
			TextureUtils::createChessTexture(parameter[0].to<uint32_t>(rt),parameter[1].to<uint32_t>(rt),parameter[2].to<uint32_t>(rt)))

	//! [ESF] Texture createColorTexture(TextureType, Number width, Number height, Number numLayers, Util:TypeConstant, Number numComponents, Bool filtering, Bool clampToEdge)
	ES_FUN(lib, "createColorTexture", 7, 8, TextureUtils::createColorTexture( static_cast<TextureType>(parameter[0].to<uint32_t>(rt)), 
																			parameter[1].to<uint32_t>(rt), parameter[2].to<uint32_t>(rt),parameter[3].to<uint32_t>(rt), 
																			static_cast<Util::TypeConstant>(parameter[4].to<uint32_t>(rt)), parameter[5].to<uint32_t>(rt),
																			parameter[6].toBool(),parameter[7].toBool(false)))

	//! [ESF] Texture createDataTexture(TextureType, Number width, Number height, Number numLayers, Util:TypeConstant, Number numComponents)
	ES_FUN(lib, "createDataTexture", 6, 6, TextureUtils::createDataTexture( static_cast<TextureType>(parameter[0].to<uint32_t>(rt)), 
																			parameter[1].to<uint32_t>(rt), parameter[2].to<uint32_t>(rt),parameter[3].to<uint32_t>(rt), 
																			static_cast<Util::TypeConstant>(parameter[4].to<uint32_t>(rt)), parameter[5].to<uint32_t>(rt)))

	//! [ESF] Texture createDepthTexture(Number width, Number height, [Number layers])
	ES_FUN(lib, "createDepthTexture", 2, 3, TextureUtils::createDepthTexture(parameter[0].toUInt(), parameter[1].toUInt(), parameter[2].toUInt(0)))

	//! [ESF] Texture createHDRDepthTexture(Number width, Number height, [Number layers])
	ES_FUN(lib, "createHDRDepthTexture", 2, 3, TextureUtils::createHDRDepthTexture(parameter[0].toUInt(), parameter[1].toUInt(), parameter[2].toUInt(0)))

	//! [ESF] Texture createMultisampleDepthTexture(Number width, Number height, [Number samples])
	ES_FUN(lib, "createMultisampleDepthTexture", 2, 3, TextureUtils::createMultisampleDepthTexture(parameter[0].toUInt(), parameter[1].toUInt(), parameter[2].toUInt(4)))

	//! [ESF] Texture createHDRCubeTexture(Number size,alpha)
	ES_FUN(lib,"createHDRCubeTexture",2,2, TextureUtils::createHDRCubeTexture(parameter[0].to<uint32_t>(rt), parameter[1].toBool()))

	//! [ESF] Texture createHDRTexture(width,height,alpha)
	ES_FUN(lib,"createHDRTexture",3,3, TextureUtils::createHDRTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool()))

	//! [ESF] Texture createStdCubeTexture(Number size,alpha)
	ES_FUN(lib,"createStdCubeTexture",2,2, TextureUtils::createStdCubeTexture(parameter[0].to<uint32_t>(rt), parameter[1].toBool()))

	//! [ESF] Texture createStdTexture(width,height,alpha)
	ES_FUN(lib,"createStdTexture",3,3, TextureUtils::createStdTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool()))

	//! [ESF] Texture createStdTexture(width,height,alpha,[Number samples])
	ES_FUN(lib,"createMultisampleTexture",3,4, TextureUtils::createMultisampleTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool(), parameter[3].toUInt(4)))

	//! [ESF] Texture createTextureFromBitmap(Util.Bitmap[,bool clampToEdges=false]])
	ES_FUN(lib,"createTextureFromBitmap",1,4, TextureUtils::createTextureFromBitmap(
			parameter[0].to<Util::Bitmap &>(rt), 
			static_cast<TextureType>( parameter[1].toUInt( static_cast<uint32_t>(TextureType::TEXTURE_2D) )), 
			parameter[2].toUInt(1), parameter[3].toBool(false)))

	//! [ESF] Texture createNoiseTexture(width,height,alpha[,scaling=1.0])
	ES_FUN(lib,"createNoiseTexture",3,4, TextureUtils::createNoiseTexture(
			parameter[0].to<uint32_t>(rt), parameter[1].to<uint32_t>(rt),parameter[2].toBool(),parameter[3].toFloat(1.0)))


	//! [ESF] Texture createTextureFromFile(String fileName, [Texture::TextureTyp, int layerNum])
	ES_FUN(lib, "createTextureFromFile", 1, 3, Rendering::Serialization::loadTexture( Util::FileName(parameter[0].toString()),
                                                                                  static_cast<TextureType>(parameter[1].toUInt(static_cast<uint32_t>(TextureType::TEXTURE_2D))),
                                                                                  parameter[2].toUInt(static_cast<uint32_t>(1))) )

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
			parameter[1].to<Rendering::Texture&>(rt));
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
			parameter[1].to<Rendering::Texture&>(rt));
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
			parameter[1].to<Rendering::Texture&>(rt));
		if(acc != nullptr) {
			return EScript::create(acc);
		} else {
			return EScript::create(nullptr);
		}
	})

	//! [ESF] Number minDepthDistance(RenderingContext, Texture, Texture);
	ES_FUN(lib, "minDepthDistance", 3, 3,
		TextureUtils::minDepthDistance(parameter[0].to<RenderingContext&>(rt),
															parameter[1].to<Rendering::Texture&>(rt),
															parameter[2].to<Rendering::Texture&>(rt)))

	//! [ESF] Bool saveTexture(RenderingContext,Texture tex, String fileName)
	ES_FUN(lib, "saveTexture", 3, 3,
		Rendering::Serialization::saveTexture(
			parameter[0].to<RenderingContext&>(rt),
			parameter[1].to<Rendering::Texture*>(rt),
			Util::FileName(parameter[2].toString())))

	//! [ESF] void updateTextureFromScreen( RenderingContext,[Texture,textureRect,screenX=0,screenY=0] )
	ES_FUNCTION(lib,"updateTextureFromScreen",2,5,{
		Texture & t= parameter[1].to<Rendering::Texture&>(rt);
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
