/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_FBO_H
#define E_FBO_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/FBO.h>
#include <Util/References.h>

namespace E_Rendering{

/*! EScript wrapper for Rendering::FBO.
	[E_FBO] ---|> [ReferenceObject] ---|> [Object]  */
class E_FBO : public EScript::ReferenceObject<Util::Reference<Rendering::FBO> > {
		ES_PROVIDES_TYPE_NAME(FBO)
	public:
		static void init(EScript::Namespace & lib);
		static EScript::Type* getTypeObject();

		E_FBO(Rendering::FBO * t) : ReferenceObject_t(t,getTypeObject()){}
		virtual ~E_FBO(){}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_FBO,	Rendering::FBO*,	(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::FBO*, 	E_Rendering::E_FBO,	new E_Rendering::E_FBO(obj))

#endif // E_FBO_H
