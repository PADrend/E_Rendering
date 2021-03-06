/*
	This file is part of the E_Rendering library.
	Copyright (C) 2009-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2009-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2009-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef E_DRAW_H_
#define E_DRAW_H_

// Forward declaration
namespace EScript {
class Namespace;
}

namespace E_Rendering {

class E_Draw {
	public:
		ERENDERINGAPI static void init(EScript::Namespace & lib);
};

}

#endif /* E_DRAW_H_ */
