/*
	This file is part of the E_Rendering library.
	Copyright (C) 2013 Benjamin Eikel <benjamin@eikel.org>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_RENDERING_E_TEXTRENDERER_H
#define E_RENDERING_E_TEXTRENDERER_H

#include <EScript/Objects/ReferenceObject.h>
#include <Rendering/TextRenderer.h>

namespace E_Rendering {

//! EScript wrapper for Rendering::TextRenderer
class E_TextRenderer : public EScript::ReferenceObject<Rendering::TextRenderer, EScript::Policies::SameEObjects_ComparePolicy> {
		ES_PROVIDES_TYPE_NAME(TextRenderer)
	public:
		static void init(EScript::Namespace & lib);
		static EScript::Type * getTypeObject();

		template<typename...args>
		explicit E_TextRenderer(args&&... params) :
			ReferenceObject_t(E_TextRenderer::getTypeObject(), std::forward<args>(params)...) {
		}
		virtual ~E_TextRenderer() {
		}
};

}

ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_TextRenderer, Rendering::TextRenderer *, &(**eObj))
ES_CONV_EOBJ_TO_OBJ(E_Rendering::E_TextRenderer, Rendering::TextRenderer &, (**eObj))

#endif /* E_RENDERING_E_TEXTRENDERER_H */
