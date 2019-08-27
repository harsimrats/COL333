#include <iostream>	
#include <string>
using namespace std;

struct states
{
	string state;
	bool iflist;
	states *next;
}start, goal, dgoals, common, *operatorlist, *opH, *opT, currst;

bool armempty = true;
string startStr, goalStr;

void fillstates(string str, states *temp)
{
	int flag = 0;
	for (int i=0; i<str.length(); i++)
	{
		temp->iflist = false;
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
			temp->next = new states;
			temp = temp->next;
		}
		else if (str.at(i) != 32)
		{
			temp->state.push_back(str.at(i));
		}	
	}
}

void takeInput()
{
	cout<<"Enter start state : ";
	cin>>startStr;
	cout<<"Enter goal state : ";
	cin>>goalStr;

	fillstates(startStr, &start);
	fillstates(startStr, &currst);
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
	states *pre = &precon;
	if (str.at(0) == 's')
	{
		string combined;
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->iflist = false;
		pre->state = "cl(";
		pre->state.push_back(str.at(4));
	    pre->state = pre->state + ")";
		combined = pre->state + ",";
		pre = pre->next;
		pre->next = NULL;
		pre->iflist = false;
		pre->state = "hold(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ")";
		combined = combined + pre->state;
		states *temp1;
		temp1 = new states;
		temp1->state = combined;
		temp1->iflist = true;
		pre->next = temp1;
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		string combined;
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "ont(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
		pre->iflist = false;
		combined = pre->state + ",";
		pre = pre->next;
		pre->iflist = false;
		pre->state = "cl(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
		states *temp1;
		temp1 = new states;
		pre->next = temp1;
		pre->iflist =false;
		combined = combined + pre->state + ",";
		pre = pre->next;
		pre->iflist = false;
		pre->next = NULL;
		pre->state = "ae";
		combined = combined + pre->state;
		states *temp2;
		temp2 = new states;
		temp2->state = combined;
		temp2->iflist = true;
		pre->next = temp2;
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		string combined;
		pre->state = "hold(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state +  ")";
		pre->iflist = false;
		combined = pre->state;
		states *temp1;
		temp1 = new states;
		temp1->state = combined;
		temp1->iflist = true;
		pre->next = temp1;
	}
	else if (str.at(0) == 'u')
	{
		string combined;
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "on(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ",";
		pre->state.push_back(str.at(4));
		pre->state = pre->state + ")";
		pre->iflist = false;
		combined = pre->state + ",";
		pre = pre->next;
		pre->iflist = false;
		pre->state = "cl(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ")";
		states *temp1;
		temp1 = new states;
		pre->next = temp1;
		pre->iflist = false;
		combined = combined + pre->state + ",";
		pre = pre->next;
		pre->iflist = false;
		pre->next = NULL;
		pre->state = "ae";
		combined = combined + pre->state;
		states *temp2;
		temp2 = new states;
		temp2->state = combined;
		temp2->iflist = true;
		pre->next = temp2;
	}
	return precon;
}

states addfunc(string str)
{
	states add;
	states *pre = &add;
	if (str.at(0) == 's')
	{	
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "ae";
		pre = pre->next;
		pre->next = NULL;
		pre->state = "on(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ",";
		pre->state.push_back(str.at(4));
		pre->state = pre->state + ")";
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		pre->state = "hold(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "ont(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
		pre = pre->next;
		pre->state = "ae";
	}
	else if (str.at(0) == 'u')
	{
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "hold(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ")";
		pre = pre->next;
		pre->state = "cl(";
		pre->state.push_back(str.at(4));
		pre->state = pre->state + ")";
	}
	return add;
}

states delfunc(string str)
{
	states del;
	states *pre = &del;
	if (str.at(0) == 's')
	{	
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "cl(";
		pre->state.push_back(str.at(4));
		pre->state =  pre->state + ")";
		pre = pre->next;
		pre->next = NULL;
		pre->state = "hold(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ")";
	}
	else if (str.at(0) == 'p' && str.at(1) == 'u')
	{
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "ont(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
		pre = pre->next;
		pre->state = "ae";
	}
	else if (str.at(0) == 'p' && str.at(1) == 'd')
	{
		pre->state = "hold(";
		pre->state.push_back(str.at(3));
		pre->state = pre->state + ")";
	}
	else if (str.at(0) == 'u')
	{
		states *temp;
		temp = new states;
		pre->next = temp;
		pre->state = "on(";
		pre->state.push_back(str.at(2));
		pre->state = pre->state + ",";
		pre->state.push_back(str.at(4));
		pre->state = pre->state + ")";
		pre = pre->next;
		pre->state = "ae";
	}
	return del;
}

void pushInOpl(string str)
{
	states *temp;
	temp = new states;
	temp->state = str;
	temp->next = operatorlist;
	operatorlist = temp;
}

void pushInOp(string str)
{
	if(opH == NULL)
		{
			states *temp;
			temp = new states;
			temp->state = str;
			opH = temp;
			opT = opH;
		}
		else
		{
			states *temp;
			temp = new states;
			temp->state = str;
			opT->next = temp;
			opT = opT->next;
		}
}

void callPrecon(string str, states *dgp)
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
			if (del->state.compare(tcur->next->state) == 0)
			{
				tcur->next = tcur->next->next;
			}
		}
	}
	if (del->state.compare(tcur->state) == 0)
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

	tcur = &currst;
	int flag = 0;
	while(tcur != NULL)
	{
		if (tcur->state.at(0) == 'h')
		{
			flag = 1;
			armempty = false;
			break;
		}
		tcur = tcur->next;
	}
	if (flag == 0)
	{
		armempty = true;
	}
}

//on(B,A),ont(C),ont(A),ont(D),cl(B),cl(C),cl(D),ae
//on(C,A),on(B,D),ont(A),ont(D),cl(C),cl(B),ae
void planner(states *dgp)
{
	//if state is false
	if (findinState(dgp->state, currst) == 0)
	{
		if (dgp->iflist == true)
		{
			states tstate;
			fillstates(dgp->state, &tstate);
			states *tsp = &tstate;
			int flag = 0;
			while(tsp != NULL)
			{
				if (findinState(tsp->state, currst) == 0)
				{
					flag = 1;
					states newst = *tsp;
					states newst2 = dgoals;
					newst.next = &newst2;
					dgp = &newst;
					dgoals = *dgp;
				}
			}
			if (flag == 0)
			{
				if(operatorlist != NULL)
				{
					modifyState(operatorlist->state);
					pushInOp(operatorlist->state);
					operatorlist = operatorlist->next;
				}
				dgp= dgp->next;
				dgoals = *dgp;
			}
		}
		else
		{
			if(dgp->state.at(0) == 'o' && dgp->state.at(2) == 't')
			{
				string str;
				str = "pd(";
				str.push_back(dgp->state.at(4));
				str = str + ")";

				pushInOpl(str);
				callPrecon(str, dgp);
			}
			else if(dgp->state.at(0) == 'o')
			{
				string str;
				str = "s(";
				str.push_back(dgp->state.at(3));
				str = str + ",";
				str.push_back(dgp->state.at(5));
				str = str + ")";

				pushInOpl(str);
				callPrecon(str, dgp);
			}
			else if(dgp->state.at(0) == 'c')
			{	
				//choose str in which cl(X) comes from us(_,X)
				string str = "us(";
				states *tempp = &currst;
				while(tempp != NULL)
				{
					if(tempp->state.at(0) == 'o' && tempp->state.at(2) != 't' && tempp->state.at(5) == dgp->state.at(3))
					{
						str = str + tempp->state.at(3);
						break;
					}
					tempp = tempp->next;
				}
				str = str + "," + tempp->state.at(5) + ")";
				pushInOpl(str);
				callPrecon(str, dgp);
			}
			else if(dgp->state.at(0) == 'h')
			{
				states *tp = &currst;
				int flag = 0;
				while(tp!=NULL)
				{
					if(tp->state.at(0) == 'c' && tp->state.at(3) == dgp->state.at(2))
					{
						flag = 1;
						// if there is ont(x) then pu else us(x,_)
						string str;
						states *tempp = &currst;
						while(tempp != NULL)
						{
							if(tempp->state.at(0) == 'o' && tempp->state.at(2) == 't' && tempp->state.at(4) == dgp->state.at(3))
								str = dgp->state;
							else if(tempp->state.at(0) == 'u' && tempp->state.at(3) == dgp->state.at(3))
								str = dgp->state;
							tempp = tempp->next;
						}
						pushInOpl(str);
						callPrecon(str, dgp);
					}
				}
				if(flag==0)
				{
					string s = "cl(";
					s.push_back(dgp->state.at(2));
					s.push_back(')');
					states newst;
					newst.state = s;
					newst.iflist = false;
					newst.next = dgp;
					dgp = &newst;
				}
			}
			else if(dgp->state.at(0) == 'a')
			{
				if(!armempty)
				{
					//if arm not empty then s or pd
					states *tp = &currst;
					int flag = 0;
					while(tp!=NULL)
					{
						if(tp->state.at(0) == 'h' && tp->iflist == false)
						{
							string s;
							s = "pd(";
							s.push_back(tp->state.at(2));
							s.push_back(')');
							tp->state = s;
							armempty = true;
						}
					}
				}	
			}
		}
	}
	else
	{
		dgp = dgp->next;
		dgoals = *dgp;
	}

	if (dgp!=NULL)
	{
		planner(dgp);
	}
}

int main()
{

	states *sp = &start;
	takeInput();

	states *gp = &goal;
	states *dgp = &dgoals;
	states *cop = &common;
	while(gp != NULL)
	{
		int val = findinState(gp->state, start);
		if(val == 0)
		{
			dgp->state = gp->state;
			dgp->iflist = false;
			dgp->next = new states;
			dgp = dgp->next;
		}
		else if (val == 1)
		{
			cop->state = gp->state;
			cop->iflist = false;
			cop->next = new states;
			cop = cop->next;
		}
		gp = gp->next;
	}
	dgp->state = goalStr;
	dgp->iflist = true;
	gp = &goal;
	sp = &start;
	dgp = &dgoals;
	cop = &common;

	if (dgp!=NULL)
	{
		planner(dgp);
	}
	// cout<<"Operations to be done : ";
	// while(opH!=NULL)
	// {
	// 	cout<<opH->state<<", ";
	// 	opH = opH->next;
	// }

	return 0;
}