/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_RenderingContext.h"

#include "Shader/E_Shader.h"
#include "Shader/E_Uniform.h"
#include "E_FBO.h"
#include "Texture/E_Texture.h"
#include "Mesh/E_Mesh.h"
#include <Rendering/Texture/Texture.h>
#include "E_ParameterStructs.h"

#include <E_Geometry/E_Matrix4x4.h>
#include <E_Geometry/E_Vec2.h>
#include <E_Geometry/E_Rect.h>
#include <E_Util/Graphics/E_Color.h>
#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

#include <Rendering/RenderingContext/RenderingContext.h>

using namespace Rendering;

namespace E_Rendering {

//! (static)
EScript::Type * E_RenderingContext::getTypeObject() {
	// E_RenderingContext ---|> Object
	static EScript::ERef<EScript::Type> typeObject = new EScript::Type(EScript::Object::getTypeObject());
	return typeObject.get();
}

//! initMembers
void E_RenderingContext::init(EScript::Namespace & lib) {
	using namespace E_ParameterStructs;

	EScript::Type * typeObject = getTypeObject();
	declareConstant(&lib,getClassName(),typeObject);

	// Genreal
	ES_MFUN(typeObject,RenderingContext, "applyChanges", 0,1, (thisObj->applyChanges(parameter[0].toBool(false)),thisEObj))
	ES_MFUN(typeObject,const RenderingContext, "getImmediateMode", 0, 0, thisObj->getImmediateMode())
	ES_MFUN(typeObject,RenderingContext, "setImmediateMode", 1, 1, (thisObj->setImmediateMode(parameter[0].toBool()),thisEObj))

	// GL Helper
	//! [ESMF] thisEObj RenderingContext.initGLState()
	ES_FUN(typeObject,"initGLState",0,0,
				(RenderingContext::initGLState(), EScript::create(nullptr)))

	//!	[ESMF] thisEObj RenderingContext.clearScreen(Util.Color4)
	ES_MFUN(typeObject,RenderingContext,"clearScreen", 1, 1,
				(thisObj->clearScreen(parameter[0].to<Util::Color4f>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.clearScreenRect(E_Rect, Util.Color4[,Bool clearDepth = true])
	ES_MFUN(typeObject,RenderingContext,"clearScreenRect", 2, 3,
				(thisObj->clearScreenRect( Geometry::Rect_i(parameter[0].to<const Geometry::Rect&>(rt)),
											parameter[1].to<Util::Color4f>(rt),
											parameter[2].toBool(true) ),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.clearColor(Util.Color4)
	ES_MFUN(typeObject,RenderingContext,"clearColor", 1, 1,
				(thisObj->clearColor(parameter[0].to<Util::Color4f>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.clearDepth(Number)
	ES_MFUN(typeObject,RenderingContext,"clearDepth", 1, 1,
				(thisObj->clearDepth(parameter[0].toFloat()),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.clearStencil(Number)
	ES_MFUN(typeObject,RenderingContext,"clearStencil", 1, 1,(thisObj->clearStencil(parameter[0].to<int32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.displayMesh(Mesh)
	ES_MFUN(typeObject,RenderingContext,"displayMesh", 1, 1,(thisObj->displayMesh(parameter[0].to<Rendering::Mesh*>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.finish()
	ES_MFUN(typeObject,RenderingContext, "finish", 0, 0, (thisObj->finish(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.flush()
	ES_MFUN(typeObject,RenderingContext, "flush", 0, 0, (thisObj->flush(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.barrier()
	ES_MFUN(typeObject,RenderingContext, "barrier", 0, 0, (thisObj->barrier(parameter[0].toUInt(0)),thisEObj))
	
	// Atomic counter buffers
	//! [ESF] Bool RenderingContext.isAtomicCountersSupported( )
	ES_FUN(typeObject,"isAtomicCountersSupported",0,0, RenderingContext::isAtomicCountersSupported())

	//! [ESF] Number RenderingContext.getMaxAtomicCounterBuffers( )
	ES_FUN(typeObject,"getMaxAtomicCounterBuffers",0,0, RenderingContext::getMaxAtomicCounterBuffers())

	//! [ESF] Number RenderingContext.getMaxAtomicCounterBufferSize( )
	ES_FUN(typeObject,"getMaxAtomicCounterBufferSize",0,0, RenderingContext::getMaxAtomicCounterBufferSize())

	//!	[ESMF] Texture|void RenderingContext.getAtomicCounterTextureBuffer( unit )
	ES_MFUN(typeObject,RenderingContext, "getAtomicCounterTextureBuffer",1,1,
			thisObj->getAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt)))

	//!	[ESMF] thisEObj RenderingContext.popAtomicCounterTextureBuffer( unit )
	ES_MFUN(typeObject,RenderingContext, "popAtomicCounterTextureBuffer",1,1,
			(thisObj->popAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetAtomicCounterTextureBuffer( unit, Texture|void )
	ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetAtomicCounterTextureBuffer", 2,2,{
		if(parameter[1].toBool())
			thisObj->pushAndSetAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt),parameter[1].to<Texture*>(rt));
		else
			thisObj->pushAndSetAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt),nullptr);
		return thisEObj;
	})

	//!	[ESMF] thisEObj RenderingContext.pushTexture( unit )
	ES_MFUN(typeObject,RenderingContext, "pushAtomicCounterTextureBuffer", 1,1,
			(thisObj->pushAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt)),thisEObj))


	//!	[ESMF] thisEObj RenderingContext.setAtomicCounterTextureBuffer( unit, Texture|void )
	ES_MFUNCTION(typeObject,RenderingContext, "setAtomicCounterTextureBuffer", 2,2,{
		if(parameter[1].toBool())
			thisObj->setAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt),parameter[1].to<Texture*>(rt));
		else
			thisObj->setAtomicCounterTextureBuffer(parameter[0].to<uint32_t>(rt),nullptr);
		return thisEObj;
	})
	
	// Blending
	//!	[ESMF] BlendingParameters RenderingContext.getBlendingParameters()
	ES_MFUN(typeObject,RenderingContext, "getBlendingParameters", 0, 0,
				new E_BlendingParameters(thisObj->getBlendingParameters()))

	//!	[ESMF] thisEObj RenderingContext.popBlending()
	ES_MFUN(typeObject,RenderingContext, "popBlending", 0, 0,
				(thisObj->popBlending(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushBlending()
	ES_MFUN(typeObject,RenderingContext, "pushBlending", 0, 0,
				(thisObj->pushBlending(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetBlending(BlendingParameters)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetBlending", 1, 1,
				(thisObj->pushAndSetBlending( parameter[0].to<const BlendingParameters&>(rt)),thisEObj))

	//! [ESMF] thisEObj RenderingContext.setBlending(BlendingParameters)
	ES_MFUN(typeObject,RenderingContext, "setBlending", 1, 1,
				(thisObj->setBlending( parameter[0].to<const BlendingParameters&>(rt)),thisEObj))


	// Lighting
	//! [ESMF] LightingParameters RenderingContext.getLightingParamters()
	ES_MFUN(typeObject,RenderingContext, "getLightingParamters", 0, 0,
				new E_LightingParameters(thisObj->getLightingParameters()))

	//! [ESMF] thisEObj RenderingContext.popLighting()
	ES_MFUN(typeObject,RenderingContext, "popLighting", 0, 0,
				(thisObj->popLighting(),thisEObj))

	//! [ESMF] thisEObj RenderingContext.pushLighting()
	ES_MFUN(typeObject,RenderingContext, "pushLighting", 0, 0,
				(thisObj->pushLighting(),thisEObj))

	//! [ESMF] thisEObj RenderingContext.pushAndSetLighting( LightingParameters | Bool )
	ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetLighting", 1, 1,{
		if( parameter[0].toType<EScript::Bool>() )
			thisObj->pushAndSetLighting(LightingParameters(parameter[0].toBool()));
		else
			thisObj->pushAndSetLighting( parameter[0].to<const LightingParameters&>(rt));
		return thisEObj;
	})

	//! [ESMF] thisEObj RenderingContext.setLighting( LightingParameters | Bool )
	ES_MFUNCTION(typeObject,RenderingContext, "setLighting", 1, 1,{
		if( parameter[0].toType<EScript::Bool>() )
			thisObj->setLighting(LightingParameters(parameter[0].toBool()));
		else
			thisObj->setLighting( parameter[0].to<const LightingParameters&>(rt));
		return thisEObj;
	})

	// CullFace

	//!	[ESMF] CullFaceParameters RenderingContext.getCullFaceParameters( )
	ES_MFUN(typeObject,RenderingContext, "getCullFaceParameters", 0, 0,thisObj->getCullFaceParameters())

	//!	[ESMF] thisEObj RenderingContext.popCullFace( )
	ES_MFUN(typeObject,RenderingContext, "popCullFace", 0, 0,(thisObj->popCullFace(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetCullFace( Rendering.CullFaceParameters )
	ES_MFUN(typeObject,RenderingContext, "pushAndSetCullFace", 1, 1,(thisObj->pushAndSetCullFace(parameter[0].to<const CullFaceParameters&>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushCullFace()
	ES_MFUN(typeObject,RenderingContext, "pushCullFace", 0, 0,(thisObj->pushCullFace(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.setCullFace( Rendering.CullFaceParameters )
	ES_MFUN(typeObject,RenderingContext, "setCullFace", 1, 1,(thisObj->setCullFace(parameter[0].to<const CullFaceParameters&>(rt)),thisEObj))

	// DepthBuffer
	
	//!	[ESMF] thisEObj RenderingContext.pushDepthBuffer()
	ES_MFUN(typeObject,RenderingContext, "pushDepthBuffer", 0, 0,
				(thisObj->pushDepthBuffer(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.popDepthBuffer()
	ES_MFUN(typeObject,RenderingContext, "popDepthBuffer", 0, 0,
				(thisObj->popDepthBuffer(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetDepthBuffer(Bool, Bool, Number)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetDepthBuffer", 3, 3,
				(thisObj->pushAndSetDepthBuffer(Rendering::DepthBufferParameters(
														parameter[0].to<bool>(rt),
														parameter[1].to<bool>(rt),
														static_cast<Rendering::Comparison::function_t>(parameter[2].to<uint32_t>(rt)))),thisEObj))
														

	//!	[ESMF] thisEObj RenderingContext.setDepthBuffer(Bool, Bool, Number)
	ES_MFUN(typeObject,RenderingContext, "setDepthBuffer", 3, 3,
	(thisObj->setDepthBuffer(Rendering::DepthBufferParameters(
											parameter[0].to<bool>(rt),
											parameter[1].to<bool>(rt),
											static_cast<Rendering::Comparison::function_t>(parameter[2].to<uint32_t>(rt)))),thisEObj))
	// Line
	//! [ESMF] thisEObj RenderingContext.popLine()
	ES_MFUN(typeObject,RenderingContext, "popLine", 0, 0,
				 (thisObj->popLine(),thisEObj))

	//! [ESMF] thisEObj RenderingContext.pushLine()
	ES_MFUN(typeObject,RenderingContext, "pushLine", 0, 0,
				 (thisObj->pushLine(),thisEObj))

	//! [ESMF] thisEObj RenderingContext.pushAndSetLine(Number)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetLine", 1, 1, (
				thisObj->pushAndSetLine(LineParameters(parameter[0].toFloat())),thisEObj))

	//! [ESMF] thisEObj RenderingContext.setLineWidth(Number)
	ES_MFUN(typeObject,RenderingContext, "setLineWidth", 1, 1, (
				thisObj->setLine(LineParameters(parameter[0].toFloat())),thisEObj))

	//! [ESMF] Number RenderingContext.getLineWidth()
	ES_MFUN(typeObject,RenderingContext, "getLineWidth", 0, 0,thisObj->getLineParameters().getWidth())
	// Point

	//!	[ESMF] thisEObj RenderingContext.getPointParamters( )
	ES_MFUN(typeObject,RenderingContext, "getPointParameters", 0, 0,thisObj->getPointParameters())

	//!	[ESMF] thisEObj RenderingContext.popPointParameters()
	ES_MFUN(typeObject,RenderingContext, "popPointParameters", 0, 0,
				(thisObj->popPointParameters(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetPointParameters( PointParameters )
	ES_MFUN(typeObject,RenderingContext, "pushAndSetPointParameters", 1, 1,
				(thisObj->pushAndSetPointParameters(parameter[0].to<const PointParameters&>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushPointParameters()
	ES_MFUN(typeObject,RenderingContext, "pushPointParameters", 0, 0,
				(thisObj->pushPointParameters(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.setPointParameters( PointParameters )
	ES_MFUN(typeObject,RenderingContext, "setPointParameters", 1, 1,
				(thisObj->setPointParameters(parameter[0].to<const PointParameters&>(rt)),thisEObj))

	// PolygonMode
	//!	[ESMF] thisEObj RenderingContext.popPolygonMode()
	ES_MFUN(typeObject,RenderingContext, "popPolygonMode", 0, 0,
				(thisObj->popPolygonMode(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushPolygonMode()
	ES_MFUN(typeObject,RenderingContext, "pushPolygonMode", 0, 0,
				(thisObj->pushPolygonMode(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetPolygonMode(Number)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetPolygonMode", 1, 1,
				(thisObj->pushAndSetPolygonMode(Rendering::PolygonModeParameters(static_cast<Rendering::PolygonModeParameters::polygonModeMode_t>(parameter[0].toUInt()))),thisEObj))

	// PolygonOffset
	//!	[ESMF] thisEObj RenderingContext.popPolygonOffset()
	ES_MFUN(typeObject,RenderingContext, "popPolygonOffset", 0, 0,
				(thisObj->popPolygonOffset(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetPolygonOffset(Number, Number)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetPolygonOffset", 2, 2,
				(thisObj->pushAndSetPolygonOffset(Rendering::PolygonOffsetParameters(parameter[0].toFloat(), parameter[1].toFloat())),thisEObj))

	// Scissor
	//!	[ESMF] ScissorParameters RenderingContext.getScissor()
	ES_MFUN(typeObject,RenderingContext, "getScissor", 0, 0, new E_ScissorParameters(thisObj->getScissor()))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetScissor(ScissorParameters)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetScissor", 1, 1,
				(thisObj->pushAndSetScissor(parameter[0].to<const ScissorParameters&>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushScissor()
	ES_MFUN(typeObject,RenderingContext, "pushScissor", 0, 0, (thisObj->pushScissor(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.popScissor()
	ES_MFUN(typeObject,RenderingContext, "popScissor", 0, 0, (thisObj->popScissor(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.setScissor(ScissorParameters)
	ES_MFUN(typeObject,RenderingContext, "setScissor", 1, 1,
				(thisObj->setScissor(parameter[0].to<const ScissorParameters&>(rt)),thisEObj))

	// Scissor
	//!	[ESMF] StencilParameters RenderingContext.getStencilParamters()
	ES_MFUN(typeObject,RenderingContext, "getStencilParamters", 0, 0, new E_StencilParameters(thisObj->getStencilParamters()))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetStencil(StencilParameters)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetStencil", 1, 1,
				(thisObj->pushAndSetStencil(parameter[0].to<const StencilParameters&>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushStencil()
	ES_MFUN(typeObject,RenderingContext, "pushStencil", 0, 0, (thisObj->pushStencil(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.popStencil()
	ES_MFUN(typeObject,RenderingContext, "popStencil", 0, 0, (thisObj->popStencil(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.setStencil(ScissorParameters)
	ES_MFUN(typeObject,RenderingContext, "setStencil", 1, 1,
				(thisObj->setStencil(parameter[0].to<const StencilParameters&>(rt)),thisEObj))


	// FBO
	//!	[ESMF] thisEObj void RenderingContext.pushFBO( [FBO|void] )
	ES_MFUNCTION(typeObject,RenderingContext, "pushFBO",0,0,{
		thisObj->pushFBO();
		return thisEObj;
	})

	// FBO
		//!	[ESMF] thisEObj void RenderingContext.pushAndSetFBO( [FBO|void] )
		ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetFBO",1,1,{
			FBO * fbo=nullptr;
			if(parameter[0].toBool())
				fbo = parameter[0].to<FBO*>(rt);
			thisObj->pushAndSetFBO(fbo);
			return thisEObj;
		})

	//!	[ESMF] thisEObj RenderingContext.popFBO()
	ES_MFUN(typeObject,RenderingContext, "popFBO",0,0,(thisObj->popFBO(),thisEObj))

	//!	[ESMF] thisEObj void RenderingContext.setFBO( FBO|void )
	ES_MFUNCTION(typeObject,RenderingContext, "setFBO",1,1,{
		FBO * fbo=nullptr;
		if(parameter[0].toBool())
			fbo = parameter[0].to<FBO*>(rt);
		thisObj->setFBO(fbo);

		return thisEObj;
	})
	// global uniform

		//! [ESMF] thisEObj RenderingContext.setUniform( Uniform )
	ES_MFUN(typeObject,RenderingContext,"setGlobalUniform",1,1,
		(thisObj->setGlobalUniform(	parameter[0].to<const Rendering::Uniform&>(rt) ),thisEObj))
		
		
		
	// image binding
	//! [ESF] Bool RenderingContext.isImageBindingSupported( )
	ES_FUN(typeObject,"isImageBindingSupported",0,0, RenderingContext::isImageBindingSupported())

	//! [ESMF] ImageBindParameters RenderingContext.getBoundImage( Number )
	ES_MFUN(typeObject,RenderingContext,"getBoundImage",1,1,thisObj->getBoundImage(	parameter[0].to<uint32_t>(rt) ))

	//! [ESMF] self RenderingContext.pushBoundImage( Number )
	ES_MFUN(typeObject,RenderingContext,"pushBoundImage",1,1,(thisObj->pushBoundImage(	parameter[0].to<uint32_t>(rt) ),thisEObj))

	//! [ESMF] self RenderingContext.popBoundImage( Number )
	ES_MFUN(typeObject,RenderingContext,"popBoundImage",1,1,(thisObj->popBoundImage(	parameter[0].to<uint32_t>(rt) ),thisEObj))
	
	//! [ESMF] self RenderingContext.setBoundImage( Number,ImageBindParameters )
	ES_MFUN(typeObject,RenderingContext,"setBoundImage",2,2,
				(thisObj->setBoundImage(parameter[0].to<uint32_t>(rt),parameter[1].to<const ImageBindParameters&>(rt)),thisEObj))
	
	//! [ESMF] self RenderingContext.pushAndSetBoundImage( Number,ImageBindParameters )
	ES_MFUN(typeObject,RenderingContext,"pushAndSetBoundImage",2,2,
				(thisObj->pushAndSetBoundImage(parameter[0].to<uint32_t>(rt),parameter[1].to<const ImageBindParameters&>(rt)),thisEObj))
	
	// Material
	//! [ESMF] thisEObj pushAndSetColorMaterial( [color] )
	ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetColorMaterial", 0, 1,{
		if(parameter.count()==1){
			thisObj->pushAndSetColorMaterial(parameter[0].to<Util::Color4f>(rt));
		}else{
			MaterialParameters p;
			p.enableColorMaterial();
			thisObj->pushAndSetMaterial(p);
		}
		return thisEObj;
	})
	ES_MFUN(typeObject,RenderingContext, "popMaterial", 0,0, (thisObj->popMaterial(),thisEObj))

	// Matrix
	ES_MFUN(typeObject,RenderingContext, "multMatrix", 1, 1, (thisObj->multMatrix_modelToCamera(parameter[0].to<const Geometry::Matrix4x4&>(rt)),thisEObj)) //! \deprecated alias.
	ES_MFUN(typeObject,RenderingContext, "multMatrix_modelToCamera", 1, 1, (thisObj->multMatrix_modelToCamera(parameter[0].to<const Geometry::Matrix4x4&>(rt)),thisEObj))
	ES_MFUN(typeObject,RenderingContext, "pushAndSetMatrix_modelToCamera", 1, 1, (thisObj->pushAndSetMatrix_modelToCamera(parameter[0].to<const Geometry::Matrix4x4&>(rt)),thisEObj))
	ES_MFUN(typeObject,RenderingContext, "pushMatrix", 0,0, (thisObj->pushMatrix_modelToCamera(),thisEObj))		//! \deprecated alias.
	ES_MFUN(typeObject,RenderingContext, "pushMatrix_modelToCamera", 0,0, (thisObj->pushMatrix_modelToCamera(),thisEObj))
	ES_MFUN(typeObject,RenderingContext, "popMatrix", 0,0, (thisObj->popMatrix_modelToCamera(),thisEObj))		//! \deprecated alias.
	ES_MFUN(typeObject,RenderingContext, "popMatrix_modelToCamera", 0,0, (thisObj->popMatrix_modelToCamera(),thisEObj))
	ES_MFUN(typeObject,RenderingContext, "resetMatrix", 0,0, (thisObj->resetMatrix(),thisEObj))
	ES_MFUN(typeObject,RenderingContext, "getMatrix", 0,0, new E_Geometry::E_Matrix4x4(thisObj->getMatrix_modelToCamera()))	//! \deprecated alias.
	ES_MFUN(typeObject,RenderingContext, "getMatrix_modelToCamera", 0,0, new E_Geometry::E_Matrix4x4(thisObj->getMatrix_modelToCamera()))
	
	ES_MFUN(typeObject,RenderingContext, "setMatrix", 1, 1, (thisObj->setMatrix_modelToCamera(parameter[0].to<const Geometry::Matrix4x4&>(rt)),thisEObj)) //! \deprecated alias.
	ES_MFUN(typeObject,RenderingContext, "setMatrix_modelToCamera", 1, 1, (thisObj->setMatrix_modelToCamera(parameter[0].to<const Geometry::Matrix4x4&>(rt)),thisEObj))

	ES_MFUN(typeObject,const RenderingContext, "getCameraMatrix", 0,0, thisObj->getMatrix_worldToCamera()) //! \deprecated alias.
	ES_MFUN(typeObject,const RenderingContext, "getMatrix_worldToCamera", 0,0, thisObj->getMatrix_worldToCamera())
	
	ES_MFUN(typeObject,const RenderingContext, "getInverseCameraMatrix", 0,0, thisObj->getMatrix_cameraToWorld())  //! \deprecated alias.
	ES_MFUN(typeObject,const RenderingContext, "getMatrix_cameraToWorld", 0,0, thisObj->getMatrix_cameraToWorld())
	ES_MFUN(typeObject,const RenderingContext, "getProjectionMatrix", 0,0, thisObj->getMatrix_cameraToClipping())	//! \deprecated alias.
	ES_MFUN(typeObject,const RenderingContext, "getMatrix_cameraToClipping", 0,0, thisObj->getMatrix_cameraToClipping())

	
	// Textures
	//!	[ESMF] Texture|void RenderingContext.getTexture( unit )
	ES_MFUNCTION(typeObject,RenderingContext, "getTexture",1,1,{
		Texture * t=thisObj->getTexture(parameter[0].to<uint32_t>(rt));
		if(t==nullptr)
			return EScript::create(nullptr);
		else return EScript::create(t);
	})

	//!	[ESMF] thisEObj RenderingContext.pushTexture( unit )
	ES_MFUN(typeObject,RenderingContext, "pushTexture", 1,1, (thisObj->pushTexture(parameter[0].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.popTexture( unit )
	ES_MFUN(typeObject,RenderingContext, "popTexture", 1,1, (thisObj->popTexture(parameter[0].to<uint32_t>(rt)),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.setTexture( unit, Texture|Void, [usage] )
	ES_MFUNCTION(typeObject,RenderingContext, "setTexture",2,3,{
		Texture * t = parameter[1].toBool() ? parameter[1].to<Rendering::Texture*>(rt) : nullptr;

		thisObj->setTexture(parameter[0].to<uint32_t>(rt),t,static_cast<TexUnitUsageParameter>(parameter[2].toInt(static_cast<int>(TexUnitUsageParameter::TEXTURE_MAPPING))));
		return thisEObj;
	})

	//!	[ESMF] thisEObj RenderingContext.pushAndSetTexture( unit, Texture|Void, [usage] )
		ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetTexture",2,3,{
			Texture * t = parameter[1].toBool() ? parameter[1].to<Rendering::Texture*>(rt) : nullptr;
			thisObj->pushAndSetTexture(parameter[0].to<uint32_t>(rt),t, static_cast<TexUnitUsageParameter>(parameter[2].toInt(static_cast<int>(TexUnitUsageParameter::TEXTURE_MAPPING))));
			return thisEObj;
		})


	// Shader
	//!	[ESMF] thisEObj RenderingContext.pushShader()
	ES_MFUNCTION(typeObject,RenderingContext, "pushShader", 0, 0,{
		thisObj->pushShader();
	  return thisEObj;
	})
	//!	[ESMF] thisEObj RenderingContext.pushAndSetShader( [Shader,Void] )
	ES_MFUNCTION(typeObject,RenderingContext, "pushAndSetShader", 1, 1,{
		Shader * s = parameter[0].toBool() ? parameter[0].to<Shader*>(rt) : nullptr;
		thisObj->pushAndSetShader(s);
		return thisEObj;
	})
	//!	[ESMF] thisEObj RenderingContext.popShader()
	ES_MFUN(typeObject,RenderingContext, "popShader", 0, 0, (thisObj->popShader(),thisEObj))


		//!	[ESMF] thisEObj RenderingContext.setShader( [Shader,Void] )
	ES_MFUNCTION(typeObject,RenderingContext, "setShader", 1, 1,{
		Shader * s = parameter[0].toBool() ? parameter[0].to<Shader*>(rt) : nullptr;
		thisObj->setShader(s);
		return thisEObj;
	})
	
	//!	[ESMF] thisEObj RenderingContext.dispatchCompute(dimX, [dimY, [dimZ]])
	ES_MFUN(typeObject,RenderingContext, "dispatchCompute", 1, 3, (
		thisObj->dispatchCompute(parameter[0].toUInt(), parameter[1].toUInt(1), parameter[2].toUInt(1))
	,thisEObj))
	
	//!	[ESMF] thisEObj RenderingContext.dispatchComputeIndirect([offset])
	ES_MFUN(typeObject,RenderingContext, "dispatchComputeIndirect", 0, 1, (
		thisObj->dispatchComputeIndirect(parameter[0].toUInt(0))
	,thisEObj))
	
	
	//!	[ESMF] thisEObj RenderingContext.loadUniformSubroutines( Number, Array )
	ES_MFUNCTION(typeObject,RenderingContext, "loadUniformSubroutines", 2, 2,{
		auto* values = parameter[1].to<EScript::Array*>(rt);
		auto shader = thisObj->getActiveShader();
		if(!shader)
			return thisEObj;
		std::vector<uint32_t> indices;
		for(const auto& value : *values) 
			indices.emplace_back(value->isA(EScript::Number::getTypeObject()) ? value.toUInt() : shader->getSubroutineIndex(parameter[0].toUInt(), value.toString()));
		thisObj->loadUniformSubroutines(parameter[0].toUInt(), indices);
		return thisEObj;
	})
	
	// Viewport & Window's size

	//!	[ESMF] Vec2 RenderingContext.getWindowClientArea()
	ES_MFUN(typeObject,RenderingContext, "getWindowClientArea", 0, 0, Geometry::Rect(thisObj->getWindowClientArea()))

	//!	[ESMF] Number RenderingContext.getWindowHeight()
	ES_MFUN(typeObject,RenderingContext, "getWindowHeight", 0, 0, thisObj->getWindowClientArea().getHeight())

	//!	[ESMF] Number RenderingContext.getWindowWidth()
	ES_MFUN(typeObject,RenderingContext, "getWindowWidth", 0, 0, thisObj->getWindowClientArea().getWidth())

	//!	[ESMF] Rect RenderingContext.getViewport()
	ES_MFUN(typeObject,RenderingContext, "getViewport", 0, 0, Geometry::Rect(thisObj->getViewport()))

	//!	[ESMF] thisEObj RenderingContext.pushViewport()
	ES_MFUN(typeObject,RenderingContext, "pushViewport", 0, 0, (thisObj->pushViewport(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.popViewport()
	ES_MFUN(typeObject,RenderingContext, "popViewport", 0, 0, (thisObj->popViewport(),thisEObj))


	//!	[ESMF] thisEObj RenderingContext.setViewport(Number, Number, Number, Number | Geometry.Rect)
	ES_MFUN(typeObject,RenderingContext, "setViewport", 1, 4, (thisObj->setViewport(
		parameter.size() == 4 ? Geometry::Rect_i(
			parameter[0].to<int32_t>(rt),
			parameter[1].to<int32_t>(rt),
			parameter[2].to<int32_t>(rt),
			parameter[3].to<int32_t>(rt)
		) : Geometry::Rect_i(parameter[0].to<const Geometry::Rect&>(rt))
	),thisEObj))
	
	//!	[ESMF] thisEObj RenderingContext.pushAndSetViewport(Number, Number, Number, Number | Geometry.Rect)
	ES_MFUN(typeObject,RenderingContext, "pushAndSetViewport", 1, 4, (thisObj->pushAndSetViewport(
		parameter.size() == 4 ? Geometry::Rect_i(
			parameter[0].to<int32_t>(rt),
			parameter[1].to<int32_t>(rt),
			parameter[2].to<int32_t>(rt),
			parameter[3].to<int32_t>(rt)
		) : Geometry::Rect_i(parameter[0].to<const Geometry::Rect&>(rt))
	),thisEObj))
	
	//!	[ESMF] thisEObj RenderingContext.setWindowClientArea(Vec2 | width,height)
	ES_MFUN(typeObject,RenderingContext, "setWindowClientArea", 1, 4, (thisObj->setWindowClientArea(
				parameter.count() == 1 	? Geometry::Rect_i(parameter[0].to<const Geometry::Rect&>(rt))
										: Geometry::Rect_i(parameter[0].to<int32_t>(rt), parameter[1].to<int32_t>(rt), parameter[2].to<int32_t>(rt), parameter[3].to<int32_t>(rt)))
				,thisEObj))
				
	// Color Buffer
	
	//!	[ESMF] thisEObj RenderingContext.popColorBuffer()
	ES_MFUN(typeObject,RenderingContext, "popColorBuffer", 0, 0,
				(thisObj->popColorBuffer(),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushAndSetColorBuffer( Bool, Bool, Bool, Bool )
	ES_MFUN(typeObject,RenderingContext, "pushAndSetColorBuffer", 4, 4,
				(thisObj->pushAndSetColorBuffer(
					ColorBufferParameters(parameter[0].toBool(), parameter[1].toBool(), parameter[2].toBool(), parameter[3].toBool())
				),thisEObj))

	//!	[ESMF] thisEObj RenderingContext.pushColorBuffer()
	ES_MFUN(typeObject,RenderingContext, "pushColorBuffer", 0, 0,
				(thisObj->pushColorBuffer(),thisEObj))
				
	//!	[ESMF] thisEObj RenderingContext.setColorBuffer( Bool, Bool, Bool, Bool )
	ES_MFUN(typeObject,RenderingContext, "setColorBuffer", 4, 4,
				(thisObj->setColorBuffer(
					ColorBufferParameters(parameter[0].toBool(), parameter[1].toBool(), parameter[2].toBool(), parameter[3].toBool())
				),thisEObj))



}

}
