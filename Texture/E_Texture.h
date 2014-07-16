/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_TEXTURE_H
#define E_TEXTURE_H

#include <EScript/Objects/ReferenceObject.h>
#include <Util/References.h>

namespace Rendering{
class Texture;
}
namespace E_Rendering{

/**
 **   [E_Texture] ---|> [ReferenceObject] ---|> [Object]
 **/
class E_Texture : public EScript::ReferenceObject<Util::Reference<Rendering::Texture> > {
		ES_PROVIDES_TYPE_NAME(Texture)
	public:
		static void init(EScript::Namespace & lib);
		static EScript::Type* getTypeObject();

		E_Texture(Rendering::Texture * t);
		E_Texture(const Util::Reference<Rendering::Texture> & t);
		E_Texture(Util::Reference<Rendering::Texture> && t);
		
		Rendering::Texture * get()const{
			return ref().isNull()?nullptr:ref().get();
		}

		std::string toString() const override;
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Texture,	Rendering::Texture*,	(**eObj).get())
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_Texture,	Rendering::Texture&,	*(**eObj).get())
ES_CONV_OBJ_TO_EOBJ(Rendering::Texture*, 	E_Rendering::E_Texture,	obj ? new E_Rendering::E_Texture(obj) : nullptr)
ES_CONV_OBJ_TO_EOBJ(Util::Reference<Rendering::Texture>, 	E_Rendering::E_Texture,	obj ? new E_Rendering::E_Texture(obj) : nullptr)

#endif // E_TEXTURE_H
