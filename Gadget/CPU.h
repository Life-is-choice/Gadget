#pragma once
class PC {
	struct CPU {
		struct CU;
		struct EU;
	};
	struct Memory;
	Memory *pc;
	//*pc->CU->EU
};