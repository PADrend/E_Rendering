/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_ParameterStructs.h"

#include "Texture/E_Texture.h"

#include <Rendering/Texture/Texture.h>
#include <E_Geometry/E_Rect.h>
#include <E_Util/Graphics/E_Color.h>
#include <EScript/Basics.h>

using namespace EScript;
using namespace Rendering;

namespace E_Rendering {
namespace E_ParameterStructs {

//! (static)
EScript::Type * E_BlendingParameters::getTypeObject() {
	// E_BlendingParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_AlphaTestParameters::getTypeObject() {
	// E_AlphaTestParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_PointParameters::getTypeObject() {
	// E_PointParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_PolygonModeParameters::getTypeObject() {
	// E_PolygonModeParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_ScissorParameters::getTypeObject() {
	// E_ScissorParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_StencilParameters::getTypeObject() {
	// E_StencilParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_LightingParameters::getTypeObject() {
	// E_LightingParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_MaterialParameters::getTypeObject() {
	// E_MaterialParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_CullFaceParameters::getTypeObject() {
	// E_CullFaceParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! (static)
EScript::Type * E_ImageBindParameters::getTypeObject() {
	// E_ImageBindParameters ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}
E_ImageBindParameters::E_ImageBindParameters(const Rendering::ImageBindParameters & p) : EScript::ReferenceObject<Rendering::ImageBindParameters>(p, getTypeObject()) {
}
E_ImageBindParameters::~E_ImageBindParameters(){}

//! initMembers
void init(EScript::Namespace & lib) {
	{
		EScript::Namespace * comparison = new EScript::Namespace;
		declareConstant(&lib, "Comparison", comparison);
		declareConstant(comparison, "NEVER", static_cast<uint32_t>(Rendering::Comparison::NEVER));
		declareConstant(comparison, "LESS", static_cast<uint32_t>(Rendering::Comparison::LESS));
		declareConstant(comparison, "EQUAL", static_cast<uint32_t>(Rendering::Comparison::EQUAL));
		declareConstant(comparison, "LEQUAL", static_cast<uint32_t>(Rendering::Comparison::LEQUAL));
		declareConstant(comparison, "GREATER", static_cast<uint32_t>(Rendering::Comparison::GREATER));
		declareConstant(comparison, "NOTEQUAL", static_cast<uint32_t>(Rendering::Comparison::NOTEQUAL));
		declareConstant(comparison, "GEQUAL", static_cast<uint32_t>(Rendering::Comparison::GEQUAL));
		declareConstant(comparison, "ALWAYS", static_cast<uint32_t>(Rendering::Comparison::ALWAYS));
	}
	{
		EScript::Namespace * blendingFunc = new EScript::Namespace;
		declareConstant(&lib, "BlendFunc", blendingFunc);
		declareConstant(blendingFunc, "ZERO", static_cast<uint32_t>(Rendering::BlendingParameters::ZERO));
		declareConstant(blendingFunc, "ONE", static_cast<uint32_t>(Rendering::BlendingParameters::ONE));
		declareConstant(blendingFunc, "SRC_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::SRC_COLOR));
		declareConstant(blendingFunc, "ONE_MINUS_SRC_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_SRC_COLOR));
		declareConstant(blendingFunc, "SRC_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::SRC_ALPHA));
		declareConstant(blendingFunc, "ONE_MINUS_SRC_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_SRC_ALPHA));
		declareConstant(blendingFunc, "DST_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::DST_ALPHA));
		declareConstant(blendingFunc, "ONE_MINUS_DST_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_DST_ALPHA));
		declareConstant(blendingFunc, "DST_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::DST_COLOR));
		declareConstant(blendingFunc, "ONE_MINUS_DST_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_DST_COLOR));
		declareConstant(blendingFunc, "SRC_ALPHA_SATURATE", static_cast<uint32_t>(Rendering::BlendingParameters::SRC_ALPHA_SATURATE));
		declareConstant(blendingFunc, "CONSTANT_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::CONSTANT_COLOR));
		declareConstant(blendingFunc, "ONE_MINUS_CONSTANT_COLOR", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_CONSTANT_COLOR));
		declareConstant(blendingFunc, "CONSTANT_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::CONSTANT_ALPHA));
		declareConstant(blendingFunc, "ONE_MINUS_CONSTANT_ALPHA", static_cast<uint32_t>(Rendering::BlendingParameters::ONE_MINUS_CONSTANT_ALPHA));
	}
	{
		EScript::Namespace * blendingEquation = new EScript::Namespace;
		declareConstant(&lib, "BlendEquation", blendingEquation);
		declareConstant(blendingEquation, "FUNC_ADD", static_cast<uint32_t>(Rendering::BlendingParameters::FUNC_ADD));
		declareConstant(blendingEquation, "FUNC_SUBTRACT", static_cast<uint32_t>(Rendering::BlendingParameters::FUNC_SUBTRACT));
		declareConstant(blendingEquation, "FUNC_REVERSE_SUBTRACT", static_cast<uint32_t>(Rendering::BlendingParameters::FUNC_REVERSE_SUBTRACT));
	}
	{	//AlphaTestParameters
		Type * typeObject = E_AlphaTestParameters::getTypeObject();
		declareConstant(&lib,E_AlphaTestParameters::getClassName(),typeObject);

		//! new AlphaTestParameters()
		ES_CONSTRUCTOR(typeObject,0,2,{
				Rendering::AlphaTestParameters para;
				if(parameter.count() >= 1)
					para.setMode(static_cast<Rendering::Comparison::function_t>(parameter[0].to<uint32_t>(rt)));
				if(parameter.count() >= 2)
					para.setReferenceValue(parameter[1].toFloat());
				return new E_AlphaTestParameters(para);
		})

		//! Number AlphaTestParameters.getMode()
		ES_MFUN(typeObject,AlphaTestParameters,"getMode",0,0,static_cast<uint32_t>(thisObj->getMode()))

		//! Number AlphaTestParameters.getReferenceValue()
		ES_MFUN(typeObject,AlphaTestParameters,"getReferenceValue",0,0,	thisObj->getReferenceValue())

		//! thisEObj AlphaTestParameters.setMode()
		ES_MFUN(typeObject,AlphaTestParameters,"setMode",1,1,
					(thisObj->setMode(static_cast<Rendering::Comparison::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj AlphaTestParameters.setReferenceValue()
		ES_MFUN(typeObject,AlphaTestParameters,"setReferenceValue",1,1,
					(thisObj->setReferenceValue(parameter[0].toFloat()),thisEObj))
	}
	//---------------------------------------------------------------------------------------------------------------------------------
	{
		//E_BlendingParameters
		Type * typeObject = E_BlendingParameters::getTypeObject();
		declareConstant(&lib, E_BlendingParameters::getClassName(), typeObject);

		//! new BlendingParameters([Rendering.BlendFunc, Rendering.BlendFunc])
		ES_CONSTRUCTOR(typeObject, 0, 2, {
			if(parameter.count() == 2) {
				return new E_BlendingParameters(
					BlendingParameters(
						static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt)),
						static_cast<BlendingParameters::function_t>(parameter[1].to<uint32_t>(rt))
					)
				);
			} else {
				return new E_BlendingParameters(BlendingParameters());
			}
		})

		//! Bool BlendingParameters.isEnabled()
		ES_MFUN(typeObject,const BlendingParameters, "isEnabled", 0, 0, thisObj->isEnabled())

		//! thisEObj BlendingParameters.enable()
		ES_MFUN(typeObject,BlendingParameters, "enable", 0, 0, (thisObj->enable(),thisEObj))

		//! thisEObj BlendingParameters.disable()
		ES_MFUN(typeObject,BlendingParameters, "disable", 0, 0, (thisObj->disable(),thisEObj))

		//! Color4f BlendingParameters.getBlendColor()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendColor", 0, 0,thisObj->getBlendColor())

		//! Number BlendingParameters.getBlendEquationAlpha()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendEquationAlpha", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendEquationAlpha()))

		//! Number BlendingParameters.getBlendEquationRGB()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendEquationRGB", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendEquationRGB()))

		//! Number BlendingParameters.getBlendFuncDstAlpha()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendFuncDstAlpha", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendFuncDstAlpha()))

		//! Number BlendingParameters.getBlendFuncDstRGB()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendFuncDstRGB", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendFuncDstRGB()))

		//! Number BlendingParameters.getBlendFuncSrcAlpha()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendFuncSrcAlpha", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendFuncSrcAlpha()))

		//! Number BlendingParameters.getBlendFuncSrcRGB()
		ES_MFUN(typeObject,const BlendingParameters, "getBlendFuncSrcRGB", 0, 0,
					static_cast<uint32_t>(thisObj->getBlendFuncSrcRGB()))


		//! thisEObj BlendingParameters.setBlendColor( color4f )
		ES_MFUN(typeObject,BlendingParameters, "setBlendColor", 1, 1,
					 (thisObj->setBlendColor(parameter[0].to<Util::Color4f>(rt)),thisEObj))

		//! thisEObj BlendingParameters.setBlendEquation(Rendering.BlendEquation)
		ES_MFUN(typeObject,BlendingParameters, "setBlendEquation", 1, 1,
					 (thisObj->setBlendEquation(static_cast<BlendingParameters::equation_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendEquationAlpha(Rendering.BlendEquation)
		ES_MFUN(typeObject,BlendingParameters, "setBlendEquationAlpha", 1, 1,
					 (thisObj->setBlendEquationAlpha(static_cast<BlendingParameters::equation_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendEquationRGB(Rendering.BlendEquation)
		ES_MFUN(typeObject,BlendingParameters, "setBlendEquationRGB", 1, 1,
					 (thisObj->setBlendEquationRGB(static_cast<BlendingParameters::equation_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendFunc(Rendering.BlendFunc, Rendering.BlendFunc)
		ES_MFUN(typeObject,BlendingParameters, "setBlendFunc", 2, 2,
					 (thisObj->setBlendFunc(static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt)), static_cast<BlendingParameters::function_t>(parameter[1].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendFuncDstAlpha(Rendering.BlendFunc)
		ES_MFUN(typeObject,BlendingParameters, "setBlendFuncDstAlpha", 1, 1,
					 (thisObj->setBlendFuncDstAlpha(static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendFuncDstRGB(Rendering.BlendFunc)
		ES_MFUN(typeObject,BlendingParameters, "setBlendFuncDstRGB", 1, 1,
					 (thisObj->setBlendFuncDstRGB(static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendFuncSrcAlpha(Rendering.BlendFunc)
		ES_MFUN(typeObject,BlendingParameters, "setBlendFuncSrcAlpha", 1, 1,
					 (thisObj->setBlendFuncSrcAlpha(static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! thisEObj BlendingParameters.setBlendFuncSrcRGB(Rendering.BlendFunc)
		ES_MFUN(typeObject,BlendingParameters, "setBlendFuncSrcRGB", 1, 1,
					 (thisObj->setBlendFuncSrcRGB(static_cast<BlendingParameters::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))
	}
	
//---------------------------------------------------------------------------------------------------------------------------------

	// cullFace
	declareConstant(&lib,"CULL_BACK",static_cast<uint32_t>(Rendering::CullFaceParameters::CULL_BACK));
	declareConstant(&lib,"CULL_FRONT",static_cast<uint32_t>(Rendering::CullFaceParameters::CULL_FRONT));
	declareConstant(&lib,"CULL_FRONT_AND_BACK",static_cast<uint32_t>(Rendering::CullFaceParameters::CULL_FRONT_AND_BACK));

	{
		//E_CullFaceParameters
		Type * typeObject = E_CullFaceParameters::getTypeObject();
		declareConstant(&lib, E_CullFaceParameters::getClassName(), typeObject);

		//! new CullFaceParameters()
		ES_CONSTRUCTOR(typeObject, 0, 1, {
			if(parameter.count() > 0) {
				return new E_CullFaceParameters(Rendering::CullFaceParameters(static_cast<Rendering::CullFaceParameters::cullFaceMode_t>(parameter[0].to<uint32_t>(rt))));
			}
			return new E_CullFaceParameters(Rendering::CullFaceParameters());
		})

		ES_MFUN(typeObject,CullFaceParameters, "enable", 0, 0,	(thisObj->enable(),thisEObj))

		ES_MFUN(typeObject,CullFaceParameters, "disable", 0, 0,	(thisObj->disable(),thisEObj))

		ES_MFUN(typeObject,CullFaceParameters, "isEnabled", 0, 0,thisObj->isEnabled())

		ES_MFUN(typeObject,CullFaceParameters, "getMode", 0, 0,	static_cast<uint32_t>(thisObj->getMode()))

		ES_MFUN(typeObject,CullFaceParameters, "setMode", 1, 1,
			(thisObj->setMode(static_cast<Rendering::CullFaceParameters::cullFaceMode_t>(parameter[0].to<uint32_t>(rt))),thisEObj))
	}

	//---------------------------------------------------------------------------------------------------------------------------------
	// ImageBindParameters

	{
		//E_ImageBindParameters
		Type * typeObject = E_ImageBindParameters::getTypeObject();
		declareConstant(&lib, E_ImageBindParameters::getClassName(), typeObject);

		//! new ImageBindParameters( [Texture] )
		ES_CONSTRUCTOR(typeObject, 0, 1, {
			if(parameter.count() > 0) {
				return EScript::create(Rendering::ImageBindParameters(parameter[0].to<Texture*>(rt)));
			}
			return EScript::create(Rendering::ImageBindParameters());
		})

		ES_MFUN(typeObject,const ImageBindParameters, "getLayer", 0, 0,				thisObj->getLayer())
		ES_MFUN(typeObject,const ImageBindParameters, "getLevel", 0, 0,				thisObj->getLevel())
		ES_MFUN(typeObject,const ImageBindParameters, "getMultiLayer", 0, 0,		thisObj->getMultiLayer())
		ES_MFUN(typeObject,const ImageBindParameters, "getReadOperations", 0, 0,	thisObj->getReadOperations())
		ES_MFUN(typeObject,const ImageBindParameters, "getTexture", 0, 0,			thisObj->getTexture())
		ES_MFUN(typeObject,const ImageBindParameters, "getWriteOperations", 0, 0,	thisObj->getWriteOperations())

		ES_MFUN(typeObject,ImageBindParameters, "setLayer", 1, 1,			(thisObj->setLayer( parameter[0].to<uint32_t>(rt) ),thisEObj))
		ES_MFUN(typeObject,ImageBindParameters, "setLevel", 1, 1,			(thisObj->setLevel( parameter[0].to<uint32_t>(rt) ),thisEObj))
		ES_MFUN(typeObject,ImageBindParameters, "setMultiLayer", 1, 1,		(thisObj->setMultiLayer( parameter[0].toBool()) ,thisEObj))
		ES_MFUN(typeObject,ImageBindParameters, "setReadOperations", 1, 1,	(thisObj->setReadOperations( parameter[0].toBool()) ,thisEObj))
		ES_MFUN(typeObject,ImageBindParameters, "setWriteOperations", 1, 1,	(thisObj->setWriteOperations( parameter[0].toBool()) ,thisEObj))
		ES_MFUN(typeObject,ImageBindParameters, "setTexture", 1, 1,			(thisObj->setTexture( parameter[0].to<Texture*>(rt) ),thisEObj))
	}

	//---------------------------------------------------------------------------------------------------------------------------------
	{
		//E_LightingParameters
		Type * typeObject = E_LightingParameters::getTypeObject();
		declareConstant(&lib, E_LightingParameters::getClassName(), typeObject);

		//! new LightingParameters(Bool)
		ES_CTOR(typeObject, 1, 1, new E_LightingParameters(Rendering::LightingParameters(parameter[0].toBool())))
	}

	//---------------------------------------------------------------------------------------------------------------------------------
	{
		//E_MaterialParameters
		Type * typeObject = E_MaterialParameters::getTypeObject();
		declareConstant(&lib, E_MaterialParameters::getClassName(), typeObject);

		//! new MaterialParameters()
		ES_CTOR(typeObject,0, 0, Rendering::MaterialParameters())

		//! thisEObj MaterialParameters.enableColorMaterial()
		ES_MFUN(typeObject,MaterialParameters, "enableColorMaterial", 0, 0, (thisObj->enableColorMaterial(),thisEObj))

		//! thisEObj MaterialParameters.disableColorMaterial()
		ES_MFUN(typeObject,MaterialParameters, "disableColorMaterial", 0, 0, (thisObj->disableColorMaterial(),thisEObj))

		//! Util.Color4f MaterialParameters.getAmbient()
		ES_MFUN(typeObject,const MaterialParameters, "getAmbient", 0, 0, thisObj->getAmbient())

		//! Bool MaterialParameters.getColorMaterial()
		ES_MFUN(typeObject,const MaterialParameters, "getColorMaterial", 0, 0, thisObj->getColorMaterial())

		//! Util.Color4f MaterialParameters.getDiffuse()
		ES_MFUN(typeObject,const MaterialParameters, "getDiffuse", 0, 0, thisObj->getDiffuse())

		//! Util.Color4f MaterialParameters.getSpecular()
		ES_MFUN(typeObject,const MaterialParameters, "getSpecular", 0, 0, thisObj->getSpecular())

		//! Util.Color4f MaterialParameters.getEmission()
		ES_MFUN(typeObject,const MaterialParameters, "getEmission", 0, 0, thisObj->getEmission())

		//! Number MaterialParameters.getShininess()
		ES_MFUN(typeObject,const MaterialParameters, "getShininess", 0, 0, thisObj->getShininess())

		//! thisEObj MaterialParameters.setAmbient(Util.Color4f)
		ES_MFUN(typeObject,MaterialParameters, "setAmbient", 1, 1,
					 (thisObj->setAmbient(parameter[0].to<Util::Color4f>(rt)),thisEObj))

		//! thisEObj MaterialParameters.setDiffuse(Util.Color4f)
		ES_MFUN(typeObject,MaterialParameters, "setDiffuse", 1, 1,
					 (thisObj->setDiffuse(parameter[0].to<Util::Color4f>(rt)),thisEObj))

		//! thisEObj MaterialParameters.setSpecular(Util.Color4f)
		ES_MFUN(typeObject,MaterialParameters, "setSpecular", 1, 1,
					 (thisObj->setSpecular(parameter[0].to<Util::Color4f>(rt)),thisEObj))

		//! thisEObj MaterialParameters.setEmission(Util.Color4f)
		ES_MFUN(typeObject,MaterialParameters, "setEmission", 1, 1,
					 (thisObj->setEmission(parameter[0].to<Util::Color4f>(rt)),thisEObj))

		//! thisEObj MaterialParameters.setShininess(Float)
		ES_MFUN(typeObject,MaterialParameters, "setShininess", 1, 1,
					 (thisObj->setShininess(parameter[0].toFloat()),thisEObj))
	}
	//---------------------------------------------------------------------------------------------------------------------------------
	{ //E_PointParameters
		Type * typeObject = E_PointParameters::getTypeObject();

		declareConstant(&lib,E_PointParameters::getClassName(),typeObject);

		//! new PointParameters([size[,smoth]])
		ES_CTOR(typeObject,0,2,
					new E_PointParameters(Rendering::PointParameters(parameter[0].toFloat(1.0),
																		parameter[1].toBool(false))))

		//! Number PointParameters.getSize()
		ES_MFUN(typeObject,PointParameters,"getSize",0,0,thisObj->getSize())
		
		//! thisEObj PointParameters.enablePointSmoothing()
		ES_MFUN(typeObject,PointParameters,"enablePointSmoothing",0,0,
					(thisObj->enablePointSmoothing(),thisEObj))

		//! thisEObj PointParameters.disablePointSmoothing()
		ES_MFUN(typeObject,PointParameters,"disablePointSmoothing",0,0,
					(thisObj->disablePointSmoothing(),thisEObj))

		//! thisEObj PointParameters.setSize()
		ES_MFUN(typeObject,PointParameters,"setSize",1,1,
					(thisObj->setSize(parameter[0].toFloat()),thisEObj))
	}
	//---------------------------------------------------------------------------------------------------------------------------------
	{ //E_PolygonModeParameters
		Type * typeObject = E_PolygonModeParameters::getTypeObject();

		declareConstant(&lib,E_PolygonModeParameters::getClassName(),typeObject);

		//! new PolygonModeParameters()
		ES_CONSTRUCTOR(typeObject,0,1,{
				Rendering::PolygonModeParameters para;
				if(parameter.count() >= 1)
					para.setMode(static_cast<Rendering::PolygonModeParameters::polygonModeMode_t>(parameter[0].to<uint32_t>(rt)));
				return new E_PolygonModeParameters(para);
		})

		//! Number PolygonModeParameters.getMode()
		ES_MFUN(typeObject,PolygonModeParameters,"getMode",0,0,
					static_cast<uint32_t>(thisObj->getMode()))

		//! thisEObj PolygonModeParameters.setMode()
		ES_MFUN(typeObject,PolygonModeParameters,"setMode",1,1,
					(thisObj->setMode(static_cast<Rendering::PolygonModeParameters::polygonModeMode_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		declareConstant(typeObject,"FILL",static_cast<uint32_t>(Rendering::PolygonModeParameters::FILL));
		declareConstant(typeObject,"LINE",static_cast<uint32_t>(Rendering::PolygonModeParameters::LINE));
		declareConstant(typeObject,"POINT",static_cast<uint32_t>(Rendering::PolygonModeParameters::POINT));
	}

	//---------------------------------------------------------------------------------------------------------------------------------
	{ //E_ScissorParameters
		Type * typeObject = E_ScissorParameters::getTypeObject();
		declareConstant(&lib, E_ScissorParameters::getClassName(), typeObject);

		//! new ScissorParameters([Rect])
		ES_CONSTRUCTOR(typeObject, 0, 1, {
				if(parameter.count() > 0) {
					const Geometry::Rect_i scissorRect(parameter[0].to<const Geometry::Rect&>(rt));
					return new E_ScissorParameters(Rendering::ScissorParameters(scissorRect));
				}
				return new E_ScissorParameters(Rendering::ScissorParameters());
		})
	}

	//---------------------------------------------------------------------------------------------------------------------------------
	{ //E_StencilParameters
		Type * typeObject = E_StencilParameters::getTypeObject();

		declareConstant(&lib,E_StencilParameters::getClassName(),typeObject);

		//! new StencilParameters()
		ES_CONSTRUCTOR(typeObject,0,0,{
				Rendering::StencilParameters para;
				para.enable();
				return new E_StencilParameters(para);
		})

		//! thisEObj StencilParameters.enable()
		ES_MFUN(typeObject,StencilParameters,"enable",0,0,
					(thisObj->enable(),thisEObj))

		//! thisEObj StencilParameters.disable()
		ES_MFUN(typeObject,StencilParameters,"disable",0,0,
					(thisObj->disable(),thisEObj))

		//! Number PolygonModeParameters.getFunction()
		ES_MFUN(typeObject,StencilParameters,"getFunction",0,0,
					static_cast<uint32_t>(thisObj->getFunction()))

		//! thisEObj PolygonModeParameters.setFunction(Number)
		ES_MFUN(typeObject,StencilParameters,"setFunction",1,1,
					(thisObj->setFunction(static_cast<Rendering::Comparison::function_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! Number PolygonModeParameters.getFunction()
		ES_MFUN(typeObject,StencilParameters,"getReferenceValue",0,0,
					static_cast<int32_t>(thisObj->getReferenceValue()))

		//! thisEObj PolygonModeParameters.setFunction(Number)
		ES_MFUN(typeObject,StencilParameters,"setReferenceValue",1,1,
					(thisObj->setReferenceValue(parameter[0].to<int32_t>(rt)),thisEObj))

		//! Number PolygonModeParameters.getBitMask()
		ES_MFUN(typeObject,StencilParameters,"getBitMask",0,0,
					thisObj->getBitMask().to_string())

		//! thisEObj PolygonModeParameters.setBitMask(String)
		ES_MFUNCTION(typeObject,StencilParameters,"setBitMask",1,1, {
			std::bitset<32> mask(parameter[0].toString());
			thisObj->setBitMask(mask);
			return thisEObj;
		})

		//! Number PolygonModeParameters.getFailAction()
		ES_MFUN(typeObject,StencilParameters,"getFailAction",0,0,
					static_cast<uint32_t>(thisObj->getFailAction()))

		//! thisEObj PolygonModeParameters.setFailAction(Number)
		ES_MFUN(typeObject,StencilParameters,"setFailAction",1,1,
					(thisObj->setFailAction(static_cast<Rendering::StencilParameters::action_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! Number PolygonModeParameters.getDepthTestFailAction()
		ES_MFUN(typeObject,StencilParameters,"getDepthTestFailAction",0,0,
					static_cast<uint32_t>(thisObj->getDepthTestFailAction()))

		//! thisEObj PolygonModeParameters.setDepthTestFailAction(Number)
		ES_MFUN(typeObject,StencilParameters,"setDepthTestFailAction",1,1,
					(thisObj->setDepthTestFailAction(static_cast<Rendering::StencilParameters::action_t>(parameter[0].to<uint32_t>(rt))),thisEObj))

		//! Number PolygonModeParameters.getDepthTestPassAction()
		ES_MFUN(typeObject,StencilParameters,"getDepthTestPassAction",0,0,
					static_cast<uint32_t>(thisObj->getDepthTestPassAction()))

		//! thisEObj PolygonModeParameters.setDepthTestPassAction(Number)
		ES_MFUN(typeObject,StencilParameters,"setDepthTestPassAction",1,1,
					(thisObj->setDepthTestPassAction(static_cast<Rendering::StencilParameters::action_t>(parameter[0].to<uint32_t>(rt))),thisEObj))


		declareConstant(typeObject,"KEEP",static_cast<uint32_t>(Rendering::StencilParameters::KEEP));
		declareConstant(typeObject,"ZERO",static_cast<uint32_t>(Rendering::StencilParameters::ZERO));
		declareConstant(typeObject,"REPLACE",static_cast<uint32_t>(Rendering::StencilParameters::REPLACE));
		declareConstant(typeObject,"INCR",static_cast<uint32_t>(Rendering::StencilParameters::INCR));
		declareConstant(typeObject,"INCR_WRAP",static_cast<uint32_t>(Rendering::StencilParameters::INCR_WRAP));
		declareConstant(typeObject,"DECR",static_cast<uint32_t>(Rendering::StencilParameters::DECR));
		declareConstant(typeObject,"DECR_WRAP",static_cast<uint32_t>(Rendering::StencilParameters::DECR_WRAP));
		declareConstant(typeObject,"INVERT",static_cast<uint32_t>(Rendering::StencilParameters::INVERT));
	}

	// ------------------------------------------------------------------------------------------------------------------------------------
	{ // E_TexUnitUsageParameter
		EScript::Namespace * ns = new EScript::Namespace;
		declareConstant(&lib, "TexUnitUsageParameter", ns);
		declareConstant(ns, "DISABLED", static_cast<uint32_t>(Rendering::TexUnitUsageParameter::DISABLED));
		declareConstant(ns, "GENERAL_PURPOSE", static_cast<uint32_t>(Rendering::TexUnitUsageParameter::GENERAL_PURPOSE));
		declareConstant(ns, "TEXTURE_MAPPING", static_cast<uint32_t>(Rendering::TexUnitUsageParameter::TEXTURE_MAPPING));
	}
}

}
}
