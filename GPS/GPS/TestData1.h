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
		Action Op_act("�֎q�ɓo��");
		set<State> Op_preconds;
		Op_preconds.insert(State("�֎q�͕����̐^�񒆂ɂ���"));
		Op_preconds.insert(State("�����̐^�񒆂ɂ���"));
		Op_preconds.insert(State("���̏�ɂ���"));
		set<State> Op_add;
		Op_add.insert(State("�o�i�i�̂��΂ɂ���"));
		Op_add.insert(State("�֎q�̏�ɂ���"));
		set<State> Op_del;
		Op_del.insert(State("�����̐^�񒆂ɂ���"));
		Op_del.insert(State("���̏�ɂ���"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op2
	{
		Action Op_act("�h�A�̑����畔���̐^�񒆂Ɉ֎q������");
		set<State> Op_preconds;
		Op_preconds.insert(State("�֎q�̓h�A�̑��ɂ���"));
		Op_preconds.insert(State("�h�A�̑��ɂ���"));
		set<State> Op_add;
		Op_add.insert(State("�֎q�͕����̐^�񒆂ɂ���"));
		Op_add.insert(State("�����̐^�񒆂ɂ���"));
		set<State> Op_del;
		Op_del.insert(State("�֎q�̓h�A�̑��ɂ���"));
		Op_del.insert(State("�h�A�̑��ɂ���"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op3
	{
		Action Op_act("�h�A�̑����畔���̐^�񒆂܂ŕ���");
		set<State> Op_preconds;
		Op_preconds.insert(State("�h�A�̑��ɂ���"));
		Op_preconds.insert(State("���̏�ɂ���"));
		set<State> Op_add;
		Op_add.insert(State("�����̐^�񒆂ɂ���"));
		set<State> Op_del;
		Op_del.insert(State("�h�A�̑��ɂ���"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op4
	{
		Action Op_act("�o�i�i������");
		set<State> Op_preconds;
		Op_preconds.insert(State("�o�i�i�̂��΂ɂ���"));
		Op_preconds.insert(State("���������Ă��Ȃ�"));
		set<State> Op_add;
		Op_add.insert(State("�o�i�i�������Ă���"));
		set<State> Op_del;
		Op_del.insert(State("���������Ă��Ȃ�"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op5
	{
		Action Op_act("�{�[���𗎂Ƃ�");
		set<State> Op_preconds;
		Op_preconds.insert(State("�{�[���������Ă���"));
		set<State> Op_add;
		Op_add.insert(State("���������Ă��Ȃ�"));
		set<State> Op_del;
		Op_del.insert(State("�{�[���������Ă���"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	// Op6
	{
		Action Op_act("�o�i�i��H�ׂ�");
		set<State> Op_preconds;
		Op_preconds.insert(State("�o�i�i�������Ă���"));
		set<State> Op_add;
		Op_add.insert(State("���������Ă��Ȃ�"));
		Op_add.insert(State("���Ȃ����󂢂Ă��Ȃ�"));
		set<State> Op_del;
		Op_del.insert(State("�o�i�i�������Ă���"));
		Op_del.insert(State("���Ȃ����󂢂Ă���"));
		Opes.insert(Operation(Op_act, Op_preconds, Op_add, Op_del));
	}

	////////////////////////////////
	//InitialState
	////////////////////////////////
	InitialStates.insert(State("�h�A�̑��ɂ���"));
	InitialStates.insert(State("���̏�ɂ���"));
	InitialStates.insert(State("�{�[���������Ă���"));
	InitialStates.insert(State("���Ȃ����󂢂Ă���"));
	InitialStates.insert(State("�֎q�̓h�A�̑��ɂ���"));

	////////////////////////////////
	//FinalState
	////////////////////////////////
	FinalStates.insert(State("���Ȃ����󂢂Ă��Ȃ�"));
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