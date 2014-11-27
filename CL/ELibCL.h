/*
 This file is part of the E_Rendering library.
 Copyright (C) 2014 Sascha Brandt <myeti@mail.upb.de>

 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifdef RENDERING_HAS_LIB_OPENCL
#ifndef E_RENDERING_CL_ELIBCL_H_
#define E_RENDERING_CL_ELIBCL_H_

// Forward declaration.
namespace EScript {
class Namespace;
}

namespace E_Rendering {
namespace E_CL {

void init(EScript::Namespace * ns);

} /* namespace E_CL */
} /* namespace E_Rendering */

#endif /* E_RENDERING_CL_ELIBCL_H_ */
#endif /* RENDERING_HAS_LIB_OPENCL */
