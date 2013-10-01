#pragma once
#include "Operation.h"
#include <vector>
using namespace GPS;

void CreateOperation(const string& ActionName, 
					 const string PreStates[],
					 const string AddStates[],
					 const string DelStates[],
					 Operation& Ope)
{
}

void CreateTestData(set<State>& InitialStates, 
					set<State>& FinalStates, 
					set<Operation>& Opes)
{
	////////////////////////////////
	//Operation
	////////////////////////////////

	// Op1
	{
		Action Op_act("climb-on-chair");
		set<State> Op_preconds;
		Op_preconds.insert(State("chair-at-middle-room"));
		Op_preconds.insert(State("at-middle-room"));
		Op_preconds.insert(State("on-floor"));
		set<State> Op_add;
		Op_add.insert(State("at-bananas"));
		Op_add.insert(State("on-chair"));
		set<State> Op_del;
		Op_del.insert(State("at-middle-room"));
		Op_del.insert(State("on-floor"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op2
	{
		Action Op_act("push-chair-from-door-to-middle-room");
		set<State> Op_preconds;
		Op_preconds.insert(State("chair-at-door"));
		Op_preconds.insert(State("at-door"));
		set<State> Op_add;
		Op_add.insert(State("chair-at-middle-room"));
		Op_add.insert(State("at-middle-room"));
		set<State> Op_del;
		Op_del.insert(State("chair-at-door"));
		Op_del.insert(State("at-door"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op3
	{
		Action Op_act("walk-from-door-to-middle-room");
		set<State> Op_preconds;
		Op_preconds.insert(State("at-door"));
		Op_preconds.insert(State("on-floor"));
		set<State> Op_add;
		Op_add.insert(State("at-middle-room"));
		set<State> Op_del;
		Op_del.insert(State("at-door"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op4
	{
		Action Op_act("grasp-bananas");
		set<State> Op_preconds;
		Op_preconds.insert(State("at-bananas"));
		Op_preconds.insert(State("empty-handed"));
		set<State> Op_add;
		Op_add.insert(State("has-bananas"));
		set<State> Op_del;
		Op_del.insert(State("empty-handed"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op5
	{
		Action Op_act("drop-ball");
		set<State> Op_preconds;
		Op_preconds.insert(State("has-ball"));
		set<State> Op_add;
		Op_add.insert(State("empty-handed"));
		set<State> Op_del;
		Op_del.insert(State("has-ball"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op6
	{
		Action Op_act("eat-bananas");
		set<State> Op_preconds;
		Op_preconds.insert(State("has-bananas"));
		set<State> Op_add;
		Op_add.insert(State("empty-handed"));
		Op_add.insert(State("not-hungry"));
		set<State> Op_del;
		Op_del.insert(State("has-bananas"));
		Op_del.insert(State("hungry"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	////////////////////////////////
	//InitialState
	////////////////////////////////
	InitialStates.insert(State("at-door"));
	InitialStates.insert(State("on-floor"));
	InitialStates.insert(State("has-ball"));
	InitialStates.insert(State("hungry"));
	InitialStates.insert(State("chair-at-door"));

	////////////////////////////////
	//FinalState
	////////////////////////////////
	FinalStates.insert(State("not-hungry"));
}

void CreateTestData1(set<State>& InitialStates, 
					set<State>& FinalStates, 
					set<Operation>& Opes)
{
	////////////////////////////////
	//Operation
	////////////////////////////////

	// Op1
	{
		Action Op_act("椅子に登る");
		set<State> Op_preconds;
		Op_preconds.insert(State("椅子は部屋の真ん中にある"));
		Op_preconds.insert(State("部屋の真ん中にいる"));
		Op_preconds.insert(State("床の上にいる"));
		set<State> Op_add;
		Op_add.insert(State("バナナのそばにいる"));
		Op_add.insert(State("椅子の上にいる"));
		set<State> Op_del;
		Op_del.insert(State("部屋の真ん中にいる"));
		Op_del.insert(State("床の上にいる"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op2
	{
		Action Op_act("ドアの側から部屋の真ん中に椅子を押す");
		set<State> Op_preconds;
		Op_preconds.insert(State("椅子はドアの側にある"));
		Op_preconds.insert(State("ドアの側にいる"));
		set<State> Op_add;
		Op_add.insert(State("椅子は部屋の真ん中にある"));
		Op_add.insert(State("部屋の真ん中にいる"));
		set<State> Op_del;
		Op_del.insert(State("椅子はドアの側にある"));
		Op_del.insert(State("ドアの側にいる"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op3
	{
		Action Op_act("ドアの側から部屋の真ん中まで歩く");
		set<State> Op_preconds;
		Op_preconds.insert(State("ドアの側にいる"));
		Op_preconds.insert(State("床の上にいる"));
		set<State> Op_add;
		Op_add.insert(State("部屋の真ん中にいる"));
		set<State> Op_del;
		Op_del.insert(State("ドアの側にいる"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op4
	{
		Action Op_act("バナナをつかむ");
		set<State> Op_preconds;
		Op_preconds.insert(State("バナナのそばにいる"));
		Op_preconds.insert(State("何も持っていない"));
		set<State> Op_add;
		Op_add.insert(State("バナナを持っている"));
		set<State> Op_del;
		Op_del.insert(State("何も持っていない"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op5
	{
		Action Op_act("ボールを落とす");
		set<State> Op_preconds;
		Op_preconds.insert(State("ボールを持っている"));
		set<State> Op_add;
		Op_add.insert(State("何も持っていない"));
		set<State> Op_del;
		Op_del.insert(State("ボールを持っている"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op6
	{
		Action Op_act("バナナを食べる");
		set<State> Op_preconds;
		Op_preconds.insert(State("バナナを持っている"));
		set<State> Op_add;
		Op_add.insert(State("何も持っていない"));
		Op_add.insert(State("おなかが空いていない"));
		set<State> Op_del;
		Op_del.insert(State("バナナを持っている"));
		Op_del.insert(State("おなかが空いている"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	////////////////////////////////
	//InitialState
	////////////////////////////////
	InitialStates.insert(State("ドアの側にいる"));
	InitialStates.insert(State("床の上にいる"));
	InitialStates.insert(State("ボールを持っている"));
	InitialStates.insert(State("おなかが空いている"));
	InitialStates.insert(State("椅子はドアの側にある"));

	////////////////////////////////
	//FinalState
	////////////////////////////////
	FinalStates.insert(State("おなかが空いていない"));
}

void CreateTestData2(set<State>& InitialStates, 
					set<State>& FinalStates, 
					set<Operation>& Opes)
{
	////////////////////////////////
	//Operation
	////////////////////////////////

	// Op1
	{
		Action Op_act("eat");
		set<State> Op_preconds;
		Op_preconds.insert(State("hungry"));
		set<State> Op_add;
		Op_add.insert(State("not-hungry"));
		set<State> Op_del;
		Op_del.insert(State("hungry"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	////////////////////////////////
	//InitialState
	////////////////////////////////
	InitialStates.insert(State("hungry"));

	////////////////////////////////
	//FinalState
	////////////////////////////////
	FinalStates.insert(State("not-hungry"));
}

void CreateTestData3(set<State>& InitialStates, 
					set<State>& FinalStates, 
					set<Operation>& Opes)
{
	////////////////////////////////
	//Operation
	////////////////////////////////

	// Op1
	{
		Action Op_act("eat-banana");
		set<State> Op_preconds;
		Op_preconds.insert(State("heavy-hungry"));
		set<State> Op_add;
		Op_add.insert(State("hungry"));
		set<State> Op_del;
		Op_del.insert(State("heavy-hungry"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op2
	{
		Action Op_act("eat-add-cookie");
		set<State> Op_preconds;
		Op_preconds.insert(State("hungry"));
		set<State> Op_add;
		Op_add.insert(State("not-hungry"));
		set<State> Op_del;
		Op_del.insert(State("hungry"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	////////////////////////////////
	//InitialState
	////////////////////////////////
	InitialStates.insert(State("heavy-hungry"));

	////////////////////////////////
	//FinalState
	////////////////////////////////
	FinalStates.insert(State("not-hungry"));
}