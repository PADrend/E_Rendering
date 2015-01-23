/*
	This file is part of the E_Rendering library.
	Copyright (C) 2014-2015 Sascha Brandt <myeti@mail.upb.de>

	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifdef RENDERING_HAS_LIB_OPENCL

#ifndef E_RENDERING_CL_E_IMAGE_H_
#define E_RENDERING_CL_E_IMAGE_H_

#include "E_Memory.h"

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/CL/Memory/Image.h>
#include <Util/References.h>


namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{
namespace E_CL{


/*!	EScript wrapper class for Rendering::CL::Image

		E_Image ---|> E_Memory ---|> EScript::ReferenceObject<Util::Reference<Rendering::CL::Image> >
			|
			--------------> Rendering::CL::Image		*/
class E_Image : public E_Memory {
		ES_PROVIDES_TYPE_NAME(Image)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_Image(Rendering::CL::Image * _obj,EScript::Type * type=nullptr) :
			E_Memory(_obj, type ? type : E_Image::getTypeObject()) {}
		virtual ~E_Image() {}

};
}
}


ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_CL::E_Image,	Rendering::CL::Image*, 			dynamic_cast<Rendering::CL::Image*>((**eObj).get()))
ES_CONV_OBJ_TO_EOBJ(Rendering::CL::Image *,			E_Rendering::E_CL::E_Image,	obj?new E_Rendering::E_CL::E_Image(obj) : nullptr)

#endif // E_RENDERING_CL_E_IMAGE_H_
#endif // RENDERING_HAS_LIB_OPENCL

