#include <iostream>	
#include <string>
using namespace std;

struct states
{
	string state;
	states *next;
}start, goal, dgoals, common, *opH, *opT, currst;

bool armempty = true;

void fillstates(string str, states *temp)
{
	int flag = 0;
	for (int i=0; i<str.length(); i++)
	{
		if(str.at(i) == '(')
		{
			temp->state.push_back(str.at(i));
			flag = 1;
		}
		else if(str.at(i) == ')')
		{
			temp->state.push_back(str.at(i));
			flag = 0;
		}
		else if(str.at(i) == ',' && flag == 0)
		{
			states *temp1;
			temp->next = temp1;
			temp = temp1;
		}
		else
			temp->state.push_back(str.at(i));
	}
}

void takeInput()
{
	string startStr, goalStr;
	cout<<"Enter start state : ";
	cin>>startStr;
	cout<<"Enter goal state : ";
	cin>>goalStr;

	fillstates(startStr, &start);
	currst = start;
	fillstates(goalStr, &goal);
}

int findinState(string str, states st)
{
	states *sp = &st;
	while(sp != NULL)
	{
		if(sp->state.compare(str) == 0)
			return 1;
		sp = sp->next;
	}
	return 0;
}

states preconfunc(string str)
{
	states precon;
	states *pre = precon;
	if (str.at(0) == 's')
	{	
		states temp;
		pre->next = temp;
		strcpy(pre->state, "cl(");
		pre->state.push_back(str.at(4));
		strcat(pre->state, ")");
		pre = pre->next;
		pre->next = NULL;
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "ont(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "cl(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
		states temp1;
		pre->next = temp1;
		pre = pre->next;
		pre->next = NULL;
		strcpy(pre->state, "ae");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'u')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "on(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ",");
		pre->state.push_back(str.at(4));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "cl(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ")");
		states temp1;
		pre->next = temp1;
		pre = pre->next;
		pre->next = NULL;
		strcpy(pre->state, "ae");
	}
	return precon;
}

states addfunc(string str)
{
	states add;
	states *pre = add;
	if (str.at(0) == 's')
	{	
		states temp;
		pre->next = temp;
		strcpy(pre->state, "ae");
		pre = pre->next;
		pre->next = NULL;
		strcpy(pre->state, "on(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ",");
		pre->state.push_back(str.at(4));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "ont(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "ae");
	}
	else if (str.at(0) == 'u')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "cl(");
		pre->state.push_back(str.at(4));
		strcar(pre->state, ")");
	}
	return add;
}

states delfunc(string str)
{
	states del;
	states *pre = del;
	if (str.at(0) == 's')
	{	
		states temp;
		pre->next = temp;
		strcpy(pre->state, "cl(");
		pre->state.push_back(str.at(4));
		strcar(pre->state, ")");
		pre = pre->next;
		pre->next = NULL;
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "ont(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "ae");
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		strcpy(pre->state, "hold(");
		pre->state.push_back(str.at(3));
		strcar(pre->state, ")");
	}
	else if (str.at(0) == 'u')
	{
		states temp;
		pre->next = temp;
		strcpy(pre->state, "on(");
		pre->state.push_back(str.at(2));
		strcar(pre->state, ",");
		pre->state.push_back(str.at(4));
		strcar(pre->state, ")");
		pre = pre->next;
		strcpy(pre->state, "ae");
	}
	return del;
}

void pushInOp(string str)
{
	if(opH == NULL)
		{
			states temp;
			strcpy(temp.state, str);
			opH = &temp;
			opT = opH;
		}
		else
		{
			states temp;
			strcpy(temp.state, str);
			opT->next = &temp;
			opT = opT->next;
		}
}

void callPrecon(string str)
{
	states temp = preconfunc(str);
	states *ptr = dgp, *tp = &temp;
	dgp = &temp;
	if (findinState(tp->state, common) == 1)
	{
		tp = tp->next;
	}
	while(tp->next!=NULL)
	{
		if (findinState(tp->next->state, common) == 1)
		{
			tp->next = tp->next->next;
		}
		tp = tp->next;
	}
	tp->next = ptr;
	dgoals = *dgp;
}

void checkAE()
{
	
}

void modifyState(string str)
{
	states addn = addfunc(str);
	states deln = addfunc(str);
	states *add = &addn;
	states *del = &deln;
	states *tcur = &currst;

	while(del != NULL)
	{
		while(tcur->next != NULL)
		{
			if (strcmp(del->state, tcur->next->state) == 0) ///////////////check if 0 or 1///////////////////
			{
				tcur->next = tcur->next->next;
			}
		}
	}
	if (strcmp(del->state, tcur->state) == 0) ///////////////check if 0 or 1///////////////////
	{
		tcur = tcur->next;
	}
	tcur = &currst;
	currst = *add;
	while(add->next!=NULL)
	{
		add = add->next;
	}
	add->next = tcur;

	checkAE();
}

void planner()
{
	//if state is false
	if (findinState(dgp->state, currst) == 0)
	{
		if(dgp->state.at(0) == 'o' && dgp->state.at(2) == 't')
		{
			string str;
			strcpy(str, "pd(");
			str.push_back(dgp->state.at(4));
			stcat(str, ")");

			pushInOp(str);
			modifyState(str);
			callPrecon();
		}
		else if(dgp->state.at(0) == 'o')
		{
			string str;
			strcpy(str, "s(");
			str.push_back(dgp->state.at(3));
			strcat(str, ",");
			strcat(str, dgp->state.at(5));
			strcat(str, ")");

			pushInOp(str);
			modifyState(str);
			callPrecon();
		}
		else if(dgp->state.at(0) == 'c')
		{
			//choose str in which cl(X) comes from us(_,X)
			string str;
			states *tempp = &currst;
			while(tempp != NULL)
			{
				if(tempp->state.at(0) == 'u' && tempp->state.at(5) == dgp->state.at(3))
					strcpy(str, dgp->state);
				tempp = tempp->next;
			}
			pushInOp(str);
			modifyState(str);
			callPrecon();
		}
		else if(dgp->state.at(0) == 'h')
		{
			// if there is ont(x) then pu else us(x,_)
			string str;
			states *tempp = &currst;
			while(tempp != NULL)
			{
				if(tempp->state.at(0) == 'o' && tempp->state.at(2) == 't' && tempp->state.at(4) == dgp->state.at(3))
					strcpy(str, dgp->state);
				else if(tempp->state.at(0) == 'u' && tempp->state.at(3) == dgp->state.at(3))
					strcpy(str, dgp->state);
				tempp = tempp->next;
			}
			pushInOp(str);
			modifyState(str);
			callPrecon();
		}
		else if(dgp->state.at(0) == 'a')
		{
			//if arm not empty then s or pd
		}
	}
	else
	{

	}
}

int main()
{
	takeInput();

	states *sp = &start;
	states *gp = &goal;
	states *dgp = &dgoals;
	states *cop = &common;
	while(gp != NULL)
	{
		int val = findinState(gp->state, start);
		if(val == 0)
		{
			dgp->state = gp->state;
			states temp;
			dgp->next = &temp;
			dgp = dgp->next;
		}
		else if (val == 1)
		{
			cop->state = gp->state;
			states temp;
			cop->next = &temp;
			cop = cop->next;
		}
		gp = gp->next;
	}
	gp = &goal;
	sp = &start;
	dgp = &dgoals;
	cop = &common;
	states *cup = &currst;

	planner();

	return 0;
}