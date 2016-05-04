/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef _E_Rendering_E_VertexAttributeAccessor_H_
#define _E_Rendering_E_VertexAttributeAccessor_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/Mesh/VertexAttributeAccessors.h>

namespace EScript {
class Namespace;
class Type;
}
namespace E_Rendering{

/*!	EScript wrapper class for Rendering::VertexAttributeAccessor.

		E_VertexAttributeAccessor ---|> EScript::ReferenceObject<Util::Reference<Rendering::VertexAttributeAccessor> >
			|
			--------------> Rendering::VertexAttributeAccessor		*/
class E_VertexAttributeAccessor : public EScript::ReferenceObject<Util::Reference<Rendering::VertexAttributeAccessor> > {
		ES_PROVIDES_TYPE_NAME(VertexAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		virtual ~E_VertexAttributeAccessor() {}

	protected:
		E_VertexAttributeAccessor(Util::Reference<Rendering::VertexAttributeAccessor> _obj,EScript::Type * type) :
				ReferenceObject_t(_obj, type) {}
};


//!	E_ColorAttributeAccessor ---|> E_VertexAttributeAccessor
class E_ColorAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(ColorAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_ColorAttributeAccessor(Util::Reference<Rendering::ColorAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_ColorAttributeAccessor::getTypeObject()) {}
		virtual ~E_ColorAttributeAccessor() {}

		Rendering::ColorAttributeAccessor * get()const	{	return static_cast<Rendering::ColorAttributeAccessor *>(ref().get());	}
};

//!	E_NormalAttributeAccessor ---|> E_VertexAttributeAccessor
class E_NormalAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(NormalAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_NormalAttributeAccessor(Util::Reference<Rendering::NormalAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_NormalAttributeAccessor::getTypeObject()) {}
		virtual ~E_NormalAttributeAccessor() {}

		Rendering::NormalAttributeAccessor * get()const	{	return static_cast<Rendering::NormalAttributeAccessor *>(ref().get());	}
};

//!	E_PositionAttributeAccessor ---|> E_VertexAttributeAccessor
class E_PositionAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(PositionAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_PositionAttributeAccessor(Util::Reference<Rendering::PositionAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_PositionAttributeAccessor::getTypeObject()) {}
		virtual ~E_PositionAttributeAccessor() {}

		Rendering::PositionAttributeAccessor * get()const	{	return static_cast<Rendering::PositionAttributeAccessor *>(ref().get());	}
};

//!	E_TexCoordAttributeAccessor ---|> E_VertexAttributeAccessor
class E_TexCoordAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(TexCoordAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_TexCoordAttributeAccessor(Util::Reference<Rendering::TexCoordAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_TexCoordAttributeAccessor::getTypeObject()) {}
		virtual ~E_TexCoordAttributeAccessor() {}

		Rendering::TexCoordAttributeAccessor * get()const	{	return static_cast<Rendering::TexCoordAttributeAccessor *>(ref().get());	}
};

//!	E_FloatAttributeAccessor ---|> E_VertexAttributeAccessor
class E_FloatAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(FloatAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_FloatAttributeAccessor(Util::Reference<Rendering::FloatAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_FloatAttributeAccessor::getTypeObject()) {}
		virtual ~E_FloatAttributeAccessor() {}

		Rendering::FloatAttributeAccessor * get()const	{	return static_cast<Rendering::FloatAttributeAccessor *>(ref().get());	}
};

//!	E_UIntAttributeAccessor ---|> E_VertexAttributeAccessor
class E_UIntAttributeAccessor : public E_VertexAttributeAccessor {
		ES_PROVIDES_TYPE_NAME(UIntAttributeAccessor)
	public:
		static EScript::Type * getTypeObject();
		static void init(EScript::Namespace & lib);

		E_UIntAttributeAccessor(Util::Reference<Rendering::UIntAttributeAccessor> _obj) :
				E_VertexAttributeAccessor(_obj.get(), E_UIntAttributeAccessor::getTypeObject()) {}
		virtual ~E_UIntAttributeAccessor() {}

		Rendering::UIntAttributeAccessor * get()const	{	return static_cast<Rendering::UIntAttributeAccessor *>(ref().get());	}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_VertexAttributeAccessor,					Rendering::VertexAttributeAccessor*,		(**eObj).get())

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_ColorAttributeAccessor,					Rendering::ColorAttributeAccessor*,			eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::ColorAttributeAccessor*, 					E_Rendering::E_ColorAttributeAccessor,		new E_Rendering::E_ColorAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::ColorAttributeAccessor>, 	E_Rendering::E_ColorAttributeAccessor,		new E_Rendering::E_ColorAttributeAccessor(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_NormalAttributeAccessor,					Rendering::NormalAttributeAccessor*,		eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::NormalAttributeAccessor*, 					E_Rendering::E_NormalAttributeAccessor,		new E_Rendering::E_NormalAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::NormalAttributeAccessor>, 	E_Rendering::E_NormalAttributeAccessor,		new E_Rendering::E_NormalAttributeAccessor(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_PositionAttributeAccessor,				Rendering::PositionAttributeAccessor*,		eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::PositionAttributeAccessor*, 					E_Rendering::E_PositionAttributeAccessor,	new E_Rendering::E_PositionAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::PositionAttributeAccessor>, 	E_Rendering::E_PositionAttributeAccessor,	new E_Rendering::E_PositionAttributeAccessor(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_TexCoordAttributeAccessor,				Rendering::TexCoordAttributeAccessor*,		eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::TexCoordAttributeAccessor*, 					E_Rendering::E_TexCoordAttributeAccessor,	new E_Rendering::E_TexCoordAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::TexCoordAttributeAccessor>, 	E_Rendering::E_TexCoordAttributeAccessor,	new E_Rendering::E_TexCoordAttributeAccessor(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_FloatAttributeAccessor,				Rendering::FloatAttributeAccessor*,		eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::FloatAttributeAccessor*, 					E_Rendering::E_FloatAttributeAccessor,	new E_Rendering::E_FloatAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::FloatAttributeAccessor>, 	E_Rendering::E_FloatAttributeAccessor,	new E_Rendering::E_FloatAttributeAccessor(obj))

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_UIntAttributeAccessor,				Rendering::UIntAttributeAccessor*,		eObj->get())
ES_CONV_OBJ_TO_EOBJ(Rendering::UIntAttributeAccessor*, 					E_Rendering::E_UIntAttributeAccessor,	new E_Rendering::E_UIntAttributeAccessor(obj))
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::UIntAttributeAccessor>, 	E_Rendering::E_UIntAttributeAccessor,	new E_Rendering::E_UIntAttributeAccessor(obj))

#endif // _E_Rendering_E_VertexAttributeAccessor_H_
