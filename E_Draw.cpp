/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "E_Draw.h"
#include "Mesh/E_Mesh.h"

#include "E_RenderingContext.h"
#include "E_BufferObject.h"

#include <E_Geometry/E_Box.h>
#include <E_Geometry/E_Rect.h>
#include <E_Geometry/E_Vec3.h>
#include <E_Geometry/E_Matrix4x4.h>
#include <E_Util/Graphics/E_Color.h>
#include <Rendering/Draw.h>
#include <Rendering/DrawCompound.h>
#include <Rendering/RenderingContext/RenderingContext.h>

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

using namespace Rendering;

namespace E_Rendering {

void E_Draw::init(EScript::Namespace & lib) {

	//!	[ESF] Void Rendering.drawCoordSys(RenderingContext, [Number scale])
	ES_FUN(&lib, "drawCoordSys", 1, 2, (Rendering::drawCoordSys(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].toFloat(1.0f)), EScript::create(nullptr)))

	//!	[ESF] void Rendering.drawFullScreenRect(RenderingContext)
	ES_FUN(&lib, "drawFullScreenRect", 1, 1, (Rendering::drawFullScreenRect(parameter[0].to<Rendering::RenderingContext&>(rt)),EScript::create(nullptr)))

	//!	[ESF] void Rendering.drawGrid(RenderingContext, [scale])
	ES_FUN(&lib, "drawGrid", 1, 2, (Rendering::drawGrid(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].toFloat(1.0f)), EScript::create(nullptr)))

	//!	[ESF] void Rendering.drawQuad(RenderingContext, lower left, lower right, upper right, upper left, [color])
	ES_FUNCTION(&lib, "drawQuad", 5, 6, {
					 if(parameter.count()==5) Rendering::drawQuad(
							parameter[0].to<Rendering::RenderingContext&>(rt),
							parameter[1].to<Geometry::Vec3f&>(rt),
							parameter[2].to<Geometry::Vec3f&>(rt),
							parameter[3].to<Geometry::Vec3f&>(rt),
							parameter[4].to<Geometry::Vec3f&>(rt));
					 else Rendering::drawQuad(
							parameter[0].to<Rendering::RenderingContext&>(rt),
							parameter[1].to<Geometry::Vec3f&>(rt),
							parameter[2].to<Geometry::Vec3f&>(rt),
							parameter[3].to<Geometry::Vec3f&>(rt),
							parameter[4].to<Geometry::Vec3f&>(rt),
							parameter[5].to<Util::Color4f>(rt));
					 return EScript::create(nullptr);
				 })

	//! [ESF] void Rendering.drawBox(RenderingContext, Box, [Color])
	ES_FUNCTION(&lib, "drawBox", 2, 3, {
		if(parameter.count() == 2) {
			Rendering::drawBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt));
		} else {
			Rendering::drawBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt), parameter[2].to<Util::Color4f>(rt));
		}
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.drawAbsBox(RenderingContext, Box, [Color])
		ES_FUNCTION(&lib, "drawAbsBox", 2, 3, {
			if(parameter.count() == 2) {
				Rendering::drawAbsBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt));
			} else {
				Rendering::drawAbsBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt), parameter[2].to<Util::Color4f>(rt));
			}
			return EScript::create(nullptr);
		})

	//![[ESF] void Rendering.drawRect(RenderingContext, Rec, [Color])
	ES_FUNCTION(&lib,"drawRect", 2, 3, {
		if(parameter.count() == 2) {
			Rendering::drawRect(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Rect&>(rt));
		} else {
			Rendering::drawRect(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Rect&>(rt), parameter[2].to<Util::Color4f>(rt));
		}
		return EScript::create(nullptr);
	})

	//! [[ESF] void Rendering.drawWireframeBox(RenderingContext, Box, [Color])
	ES_FUNCTION(&lib,"drawWireframeBox", 2, 3, {
		if(parameter.count() == 2) {
			Rendering::drawWireframeBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt));
		} else {
			Rendering::drawWireframeBox(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Box&>(rt), parameter[2].to<Util::Color4f>(rt));
		}
		return EScript::create(nullptr);
	})

	//![[ESF] void Rendering.drawWireframeRect(RenderingContext, Rec, [Color])
	ES_FUNCTION(&lib,"drawWireframeRect", 2, 3, {
		if(parameter.count() == 2) {
			Rendering::drawWireframeRect(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Rect&>(rt));
		} else {
			Rendering::drawWireframeRect(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<const Geometry::Rect&>(rt), parameter[2].to<Util::Color4f>(rt));
		}
		return EScript::create(nullptr);
	})

	//! [ESF] void Rendering.drawVector(RenderingContext, Vec3 from, Vec3 to, [Color])
	ES_FUNCTION(&lib,"drawVector", 3, 4, {
		if(parameter.count() == 3) {
			Rendering::drawVector(parameter[0].to<Rendering::RenderingContext&>(rt), parameter[1].to<Geometry::Vec3>(rt), parameter[2].to<Geometry::Vec3>(rt));
		} else {
			Rendering::drawVector(parameter[0].to<Rendering::RenderingContext&>(rt),
							  parameter[1].to<Geometry::Vec3>(rt),
							  parameter[2].to<Geometry::Vec3>(rt),
							  parameter[3].to<Util::Color4f>(rt));
		}
		return EScript::create(nullptr);
	})

	//! [ESF] Void Rendering.enable2DMode(RenderingContext [Geometry.Rect screenRect])
	ES_FUNCTION(&lib, "enable2DMode", 1, 2, {
		if(parameter.count() == 2)
			Rendering::enable2DMode(parameter[0].to<Rendering::RenderingContext&>(rt),Geometry::Rect_i(parameter[1].to<Geometry::Rect>(rt)));
		else
			Rendering::enable2DMode(parameter[0].to<Rendering::RenderingContext&>(rt));
		return EScript::create(nullptr);
	})

	//! [ESF] Void Rendering.disable2DMode(RenderingContext)
	ES_FUN(&lib, "disable2DMode", 1, 1, (Rendering::disable2DMode(parameter[0].to<Rendering::RenderingContext&>(rt)), EScript::create(nullptr)))
	
	//! [ESF] void Rendering.enableInstanceBuffer(RenderingContext, BufferObject positions, location, elements)
	ES_FUN(&lib, "enableInstanceBuffer", 4, 4, (	Rendering::enableInstanceBuffer(
		parameter[0].to<Rendering::RenderingContext&>(rt),
		parameter[1].to<Rendering::CountedBufferObject*>(rt)->get(),
		parameter[2].toUInt(),
		parameter[3].toUInt()
	), EScript::create(nullptr)))
	
	//! [ESF] void Rendering.disableInstanceBuffer(RenderingContext, BufferObject positions, location, elements)
	ES_FUN(&lib, "disableInstanceBuffer", 4, 4, (	Rendering::disableInstanceBuffer(
		parameter[0].to<Rendering::RenderingContext&>(rt),
		parameter[1].to<Rendering::CountedBufferObject*>(rt)->get(),
		parameter[2].toUInt(),
		parameter[3].toUInt()
	), EScript::create(nullptr)))
	
	//! [ESF] void Rendering.drawInstances(RenderingContext, Mesh, firstElement, elementCount, instanceCount))
	ES_FUN(&lib, "drawInstances", 5, 5, (	Rendering::drawInstances(
		parameter[0].to<Rendering::RenderingContext&>(rt),
		parameter[1].to<Rendering::Mesh*>(rt),
		parameter[2].toUInt(),
		parameter[3].toUInt(),
		parameter[4].toUInt()
	), EScript::create(nullptr)))
		
}

}
