/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_MESHBUILDER_H_
#define E_MESHBUILDER_H_

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/MeshUtils/MeshBuilder.h>
#include <Util/References.h>

namespace E_Rendering{

/**
 **   [E_MeshBuilder] ---|> [ReferenceObject] ---|> [Object]
 **/
class E_MeshBuilder : public EScript::ReferenceObject<Util::Reference<Rendering::MeshUtils::MeshBuilder> > {
		ES_PROVIDES_TYPE_NAME(MeshBuilder)
	public:
		static EScript::Type* getTypeObject();
		static void init(EScript::Namespace & lib);

		E_MeshBuilder(Rendering::MeshUtils::MeshBuilder * t):ReferenceObject_t(t,getTypeObject())	{}
		virtual ~E_MeshBuilder()	{}

};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_MeshBuilder,				Rendering::MeshUtils::MeshBuilder*,	(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::MeshUtils::MeshBuilder*, 	E_Rendering::E_MeshBuilder,			obj ? new E_Rendering::E_MeshBuilder(obj) : nullptr)

#endif /* E_MESHBUILDER_H_ */
