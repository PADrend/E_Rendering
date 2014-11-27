/*
 This file is part of the Rendering library.
 Copyright (C) 2014 Sascha Brandt <myeti@mail.upb.de>

 This library is subject to the terms of the Mozilla Public License, v. 2.0.
 You should have received a copy of the MPL along with this library; see the
 file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifdef RENDERING_HAS_LIB_OPENCL

#include "ELibCL.h"

#include "E_Event.h"
#include "E_CommandQueue.h"
#include "E_Context.h"
#include "E_Device.h"
#include "E_Kernel.h"
#include "E_Platform.h"
#include "E_Program.h"
#include "E_UserEvent.h"
#include "E_Buffer.h"
#include "E_Image.h"
#include "E_Memory.h"
#include "E_Sampler.h"

#include <EScript/Basics.h>
#include <EScript/StdObjects.h>

namespace E_Rendering {
namespace E_CL {

using namespace EScript;
using namespace Rendering;

void init(EScript::Namespace * ns) {
	Namespace * lib=new Namespace();
	declareConstant(ns,"CL",lib);

	E_Event::init(*lib);
	E_CommandQueue::init(*lib);
	E_Context::init(*lib);
	E_Device::init(*lib);
	E_Kernel::init(*lib);
	E_Platform::init(*lib);
	E_Program::init(*lib);
	E_UserEvent::init(*lib);
	E_Buffer::init(*lib);
	E_Image::init(*lib);
	E_Memory::init(*lib);
	E_Sampler::init(*lib);
}

} /* namespace E_CL */
} /* namespace E_Rendering */
#endif /* RENDERING_HAS_LIB_OPENCL */
