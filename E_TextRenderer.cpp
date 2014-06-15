/*
	This file is part of the E_Rendering library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_TextRenderer.h"
#include "E_RenderingContext.h"
#include <EScript/Basics.h>
#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Rect.h>
#include <E_Util/Graphics/E_Color.h>
#include <Util/Graphics/Bitmap.h>
#include <Util/Graphics/FontRenderer.h>
#include <Util/StringUtils.h>
#include <numeric>
#include <string>

namespace E_Rendering {

EScript::Type * E_TextRenderer::getTypeObject() {
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

void E_TextRenderer::init(EScript::Namespace & lib) {
	EScript::Type * typeObject = getTypeObject();
	EScript::declareConstant(&lib, getClassName(), typeObject);

	//! [ESF] new TextRenderer(String, Number)
	ES_CONSTRUCTOR(typeObject, 2, 2, {
		Util::FontRenderer fontRenderer(parameter[0].toString());
		std::u32string wideString(256, 0);
		std::iota(wideString.begin(), wideString.end(), 0);
		const auto result = fontRenderer.createGlyphBitmap(parameter[1].toUInt(), wideString);
		return new E_TextRenderer(*result.first.get(), result.second);
	})

	//! [ESMF] self TextRenderer.draw(RenderingContext, String, Vec2, Color)
	ES_MFUN(typeObject, Rendering::TextRenderer, "draw", 4, 4, (
		thisObj->draw(parameter[0].to<Rendering::RenderingContext&>(rt), 
					  Util::StringUtils::utf8_to_utf32(parameter[1].toString()),
					  parameter[2].to<const Geometry::Vec2 &>(rt),
					  parameter[3].to<Util::Color4f>(rt)),
		thisEObj
	))

	//! [ESMF] Rect TextRenderer.getTextSize(String)
	ES_MFUN(typeObject, Rendering::TextRenderer, "getTextSize", 1, 1,
		new E_Geometry::E_Rect(thisObj->getTextSize(Util::StringUtils::utf8_to_utf32(parameter[0].toString())))
	)
}

}
