#include <iostream>
#include <set>
#include <bits/stdc++.h>

using namespace std;

int magic_cube[3][3][3], play_cube[3][3][3];															//Magic cube entries
int Player[10], AI[10];
int pl_count, AI_count;
int hash[27];

int find_ele[100][3], fcount=0;
int eq_solution[100][4], diff_sum[100][3], not_coll_sum_42[100][3];	//eq_solution for all soln of x+y+z = 42; x,y,z distinct
int surface[15][3][3], diff_surf_sum_42[100][3];					//and b/w 1 to 27 diff_sum for collinear points whose 
int dcount = 0, ecount = 0, ncount = 0, dscount = 0;				//some is not 42 not_coll_sum_42 for eq_solution which are non
int tuple[3];														//collinear surface for storing surfaces
int game_on=0, hum_lines, AI_lines;

void generate_cube()												// function to enter values in cube
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				magic_cube[i][j][k] = 0;											// initiallize all entries to 0
			}
		}
	}

	magic_cube[1][0][1] = 1;														//Starting Point
	hash[0]= 1*100 + 0*10 + 1;
	int num, layer = 1; 
	int j = 1;
	int i = 0;
	for (num = 2; num < 28; num++)													
	{
		i = (i + 2) % 3;															//Movement along diagonal
		j = (j + 2) % 3;
		if (magic_cube[layer][i][j] == 0)											//check occupancy
		{
			magic_cube[layer][i][j] = num;
			hash[num-1]= layer*100 + i*10 +j;
		}
		else
		{																			//1st level collision
			layer = (layer + 2) % 3;												//Change in layer
			i = (i + 2) % 3;
			if(magic_cube[layer][i][j] == 0)
			{
				magic_cube[layer][i][j] = num;
				hash[num-1]= layer*100 + i*10 +j;
			}
			else																	//2nd level collision
			{
				layer = (layer + 1) % 3;
				j = (j + 1) % 3;
				magic_cube[layer][i][j] = num;
				hash[num-1]= layer*100 + i*10 +j;
			}
		}
	}
}

void display_cube()													//Display for reference
{
	for (int i = 2; i >= 0; i--)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				cout<<magic_cube[i][j][k]<<" ";
			}
			cout<<"\n";
		}
		cout<<"\n";
	}
}

void solve_eq()														//Get all the sol for x+y+z = 42 x,y,z distinct b/w 1 to 27 and store it in eq_solution
{
	for (int i = 1; i <= 14; i++)
	{
		for (int j = i+1; j < 21-i/2; j++)
		{
			int sumij = i + j;
			int third = 42 - sumij;
			if (third > 0 && third < 28 && third > i && third > j)
			{
				eq_solution[ecount][0] = i;
				eq_solution[ecount][1] = j;
				eq_solution[ecount][2] = third;
				eq_solution[ecount][3] = 0;
				ecount++;
			}
		}
	}
}

void gen_surface()													//Generate all surfaces and store it in surface
{
	for(int i=0; i<3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				surface[i][j][k] = magic_cube[i][j][k];								//1st index of magic cube constant -- 3 surfaces
				surface[i+3][j][k] = magic_cube[j][i][k];							//2nd index of magic cube constant -- 3 surfaces
				surface[i+6][j][k] = magic_cube[k][j][i];							//3rd index of magic cube constant -- 3surfaces
			}
		}
	}

	for (int i = 0; i < 3; i++)														//Diagonal Planes
	{
		for (int j = 0; j < 3; j++)
		{
			surface[9][i][j]= magic_cube[i][j][j];									//Diff of 2nd and 3rd index same
			surface[10][i][j]=magic_cube[i][j][2-j];								//Sum of 2nd and 3rd index same

			surface[11][i][j]= magic_cube[j][i][j];									//Diff of 1st and 3rd index same
			surface[12][i][j]=magic_cube[j][i][2-j];								//Sum of 1st and 3rd index same

			surface[13][i][j]= magic_cube[j][j][i];									//Diff of 1st and 2nd index same
			surface[14][i][j]=magic_cube[2-j][j][i];								//Sum of 1st and 2nd index same
		}
	}
}

int find_ele_in_eqs(int ele)										//Helper functins which checks the element in given exception list of tuples
{
	fcount = 0;
	int flag =0;
	for (int i = 0; i < ecount; i++)
	{
		if (eq_solution[i][0]==ele || eq_solution[i][1]==ele || eq_solution[i][2]==ele)
		{
			flag = 1;
			find_ele[fcount][0] = eq_solution[i][0];
			find_ele[fcount][1] = eq_solution[i][1];
			find_ele[fcount][2] = eq_solution[i][2];
			fcount++;
		}
	}
	if(flag == 1)
		return 1;
	return 0;
}

int find_pair_in_eqs(int x, int y)									//Helper function which checks pair in the given exception list of tuples
{
	fcount = 0;
	int flag =0;
	for (int i = 0; i < dcount; i++)
	{
		if (diff_sum[i][0]==x || diff_sum[i][1]==x || diff_sum[i][2]==x)
		{
			if (diff_sum[i][0]==y || diff_sum[i][1]==y || diff_sum[i][2]==y)
			{
				flag = 1;
				find_ele[fcount][0] = diff_sum[i][0];
				find_ele[fcount][1] = diff_sum[i][1];
				find_ele[fcount][2] = diff_sum[i][2];
				fcount++;
			}
		}
	}
	if(flag == 1)
		return 1;
	return 0;
}

int find_tuple_in_eqs()												//Helper function which checks tuple in the given exception list of tuples
{
	sort(tuple, tuple+3);
	for (int i = 0; i < ecount; i++)
	{
		if (eq_solution[i][0]==tuple[0] && eq_solution[i][1]==tuple[1] && eq_solution[i][2]==tuple[2])
		{
			eq_solution[i][3] = 1;
			return 1;
		}
	}
	return 0;
}

int find_tuple_in_nc()												//Helper function which checks tuple in the given diff exception list of tuples
{
	sort(tuple, tuple+3);
	for (int i = 0; i < ecount; i++)
	{
		if (not_coll_sum_42[i][0]==tuple[0] && not_coll_sum_42[i][1]==tuple[1] && not_coll_sum_42[i][2]==tuple[2])
		{
			return 1;
		}
	}
	return 0;
}

void add_to_dsum()													//helper function to create diff exception list
{
	int flag = 0;
	if(find_tuple_in_eqs()==0)
	{
		for (int i = 0; i < dcount; i++)
		{
			if (diff_sum[i][0] == tuple[0] && diff_sum[i][1] == tuple[1] && diff_sum[i][2] == tuple[2])				
			{
				flag = 1;
				break;
			}
		}
		
		if (flag == 0)
		{
			diff_sum[dcount][0] = tuple[0];
			diff_sum[dcount][1] = tuple[1];
			diff_sum[dcount][2] = tuple[2];
			dcount++;
		}
	}
}

void exceptions()													//Creates Exception list using 2 functions
{
	for (int i = 0; i < 15; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if(j==0)
				{
					tuple[0] = surface[i][j][k];
					tuple[1] = surface[i][j+1][k];
					tuple[2] = surface[i][j+2][k];
					add_to_dsum();

					if (k==0)
					{
						tuple[0] = surface[i][j][k];
						tuple[1] = surface[i][j][k+1];
						tuple[2] = surface[i][j][k+2];
						add_to_dsum();
						
						tuple[0] = surface[i][j][k];
						tuple[1] = surface[i][j+1][k+1];
						tuple[2] = surface[i][j+2][k+2];
						add_to_dsum();
					}					
					
					if (k==2)
					{
						tuple[0] = surface[i][j][k];
						tuple[1] = surface[i][j+1][k-1];
						tuple[2] = surface[i][j+2][k-2];
						add_to_dsum();
					}
				}
			}
		}
	}
}

void display_11surface()											//Display the Surfaces
{
	for (int i = 0; i < 11; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				cout<<surface[i][j][k]<<" ";
			}
			cout<<"\n";
		}
		cout<<"\n";
	}
}

void add_to_nc()													//Create diff Exception list
{
	for (int i = 0; i < ecount; i++)
	{
		if (eq_solution[i][3] == 0)
		{
			not_coll_sum_42[ncount][0] = eq_solution[i][0];
			not_coll_sum_42[ncount][1] = eq_solution[i][1];
			not_coll_sum_42[ncount][2] = eq_solution[i][2];
			ncount++;
		}
	}
}

int surface_contains(int index, int num)							//Helper function
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if(num == surface[index][i][j])
				return 0;
		}
	}
	return -1;
}

void diff_surface()													//Helper function
{
	for (int i = 0; i < ncount; i++)
	{
		int flag = 0;
		for (int j = 0; j < 15; j++)
		{
			if(surface_contains(j, not_coll_sum_42[i][0])==0)
			{
				if(surface_contains(j, not_coll_sum_42[i][1])==0)
				{
					if(surface_contains(j, not_coll_sum_42[i][2])==0)
						flag = 1;
				}
			}
		}
		if (flag == 0)
		{
			diff_surf_sum_42[dscount][0] = not_coll_sum_42[i][0];
			diff_surf_sum_42[dscount][1] = not_coll_sum_42[i][1];
			diff_surf_sum_42[dscount][2] = not_coll_sum_42[i][2];
			dscount++;
		}
	}
}

void iniate_play_area()												//Inializes the parameters and Playing Cube 
{
	for(int i=0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				play_cube[i][j][k]=2;
			}
		}
	}
	for (int i = 0; i < 10; i++)
	{
		Player[i]=-1;
		AI[i]=-1;
	}
	pl_count=0;
	AI_count=0;
	AI_lines = 0;
	hum_lines = 0;
}

bool go_n(int n)													//Use by AI to play at n 
{
	int i,j,k;
	int a = hash[n-1];
	k = a%10;
	j = (a%100-k)/10;
	i = a/100;

	if(play_cube[i][j][k]==2)
	{
		play_cube[i][j][k]=5;
		int b = magic_cube[i][j][k];
		AI[AI_count]=b;
		AI_count++;
		return true;
	}

	return false;
}

bool play_n(int n)													//Use by Player to play at n
{
	int i,j,k;
	int a = hash[n-1];
	k = a%10;
	j = (a%100-k)/10;
	i = a/100;

	if(play_cube[i][j][k]==2)
	{
		play_cube[i][j][k]=3;
		int b = magic_cube[i][j][k];
		Player[pl_count]=b;
		pl_count++;
		return true;
	}

	return false;
}

int pos_win(int x, int  y, int z)									//No of possible wins in next chance if played at this posn
{
	int Arr[15];
	int count=0;
	int b = magic_cube[x][y][z];
	for (int i = 0; i < AI_count; i++)
	{
		int a = AI[i];
		int k = 42-a-b;
		tuple[0] = a;
		tuple[1] = b;
		tuple[2] = k;
		if(find_tuple_in_nc()==0)
			{
				int flag=0;
				for (int p = 0; p < count; p++)
				{
					if(Arr[p]==k)
						flag =1;
				}
				if (flag==0)
				{
					Arr[count]=k;
					count++;
				}
			}
		else if(find_pair_in_eqs(a,b)==1)
		{
			for (int j = 0; j < fcount; j++)
			{
				int z = find_ele[j][0], q =find_ele[j][1], r =find_ele[j][2];

				if(z!=a && z!=b)
				{
					int flag=0;
					for (int p = 0; p < count; p++)
					{
						if(Arr[p]==z)
							flag =1;
					}
					if (flag==0)
					{
						Arr[count]=z;
						count++;
					}
				}
				else if(q!=a && q!=b)
				{
					int flag=0;
					for (int p = 0; p < count; p++)
					{
						if(Arr[p]==q)
							flag =1;
					}
					if (flag==0)
					{
						Arr[count]=q;
						count++;
					}	
				}
				else
				{
					int flag=0;
					for (int p = 0; p < count; p++)
					{
						if(Arr[p]==r)
							flag =1;
					}
					if (flag==0)
					{
						Arr[count]=r;
						count++;
					}
				}
			}
		}
	}
	return count;
}

int next_win()														//Gives the value for next win for AI
{
	int a,b;
	for (int i = AI_count-1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{

			a = AI[i];
			b = AI[j];
			int k = 42-a-b;
			tuple[0] = a;
			tuple[1] = b;
			tuple[2] = k;
			if(k>0 && k<28 && find_tuple_in_nc()==0)
				return k;
			else if(find_pair_in_eqs(a,b)==true)
			{
				if(find_ele[0][0]!=a && find_ele[0][0]!=b)
					return find_ele[0][0];
				else if(find_ele[0][1]!=a && find_ele[0][1]!=b)
					return find_ele[0][1];
				else 
					return find_ele[0][2];
			}	
		}
	}
	return -1;
}

int check_opp_win()													//Gives the vlaue for next win for play
{
	int a,b;
	for (int i = pl_count-1; i >=0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			a = Player[i];
			b = Player[j];
			int k = 42-a-b;
			tuple[0] = a;
			tuple[1] = b;
			tuple[2] = k;
			if(k>0 && k<28 && find_tuple_in_nc()==false)
				return k;
			else if(find_pair_in_eqs(a,b)==true)
				{
					if(find_ele[0][0]!=a && find_ele[0][0]!=b)
						return find_ele[0][0];
					else if(find_ele[0][1]!=a && find_ele[0][1]!=b)
						return find_ele[0][1];
					else 
						return find_ele[0][2];
				}
		}
	}
	return -1;
}

int check_in_AI(int a )												//Helper function
{
	int flag = 0;
	for (int i = 0; i < AI_count; i++)
	{
		if (a == AI[i])
		{
			flag = 1;
		}
	}
	return flag;
}

void check_AI()														//To check no of AI lines after AI plays
{
	int last_move = AI[AI_count-1];
	for (int i = 0; i < AI_count-1; i++)
	{
		int a = AI[i];
		int k = 42 - a - last_move;
		tuple[0] = a;
		tuple[1] = last_move;
		tuple[2] = k;
		if(k>0 && k<28 && find_tuple_in_nc()==0 && check_in_AI(k)==1)
			AI_lines++;
		else if(find_pair_in_eqs(a,last_move)==true)
		{
			if(find_ele[0][0]!=a && find_ele[0][0]!=last_move && check_in_AI(find_ele[0][0]))
				AI_lines++;
			if(find_ele[0][1]!=a && find_ele[0][1]!=last_move && check_in_AI(find_ele[0][1]))
				AI_lines++;
			if(find_ele[0][2]!=a && find_ele[0][2]!=last_move && check_in_AI(find_ele[0][2]))
				AI_lines++;
		}
	}
}

int check_in_hum(int a )											//Helper Function
{
	int flag = 0;
	for (int i = 0; i < pl_count; i++)
	{
		if (a == Player[i])
		{
			flag = 1;
		}
	}
	return flag;
}

void check_hum()													//To check no of Player lines after AI plays
{
	int last_move = Player[pl_count-1];
	for (int i = 0; i < pl_count-1; i++)
	{
		int a = Player[i];
		int k = 42 - a - last_move;
		tuple[0] = a;
		tuple[1] = last_move;
		tuple[2] = k;
		if(k>0 && k<28 && find_tuple_in_nc()==0 && check_in_hum(k)==1)
		{
			hum_lines++;
		}
		else if(find_pair_in_eqs(a,last_move)==true)
		{
			if(find_ele[0][0]!=a && find_ele[0][0]!=last_move && check_in_hum(find_ele[0][0]))
				hum_lines++;
			if(find_ele[0][1]!=a && find_ele[0][1]!=last_move && check_in_hum(find_ele[0][1]))
				hum_lines++;
			if(find_ele[0][2]!=a && find_ele[0][2]!=last_move && check_in_hum(find_ele[0][2]))
				hum_lines++;
		}
	}
}

void make_2()														//Find optimal posn for next move by AI
{
	if (next_win()!=-1 && go_n(next_win()))
	{
		return;
	}

	else if(check_opp_win() != -1 && go_n(check_opp_win()))
	{
		return;
	}

	else
	{
		if(play_cube[1][1][1]==2)
		{
			play_cube[1][1][1]=5;
			int b = magic_cube[1][1][1];
			AI[AI_count] = b;
			AI_count++;
		}
		else
		{
			int num_wins = -1;
			int next_i, next_j, next_k;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						if(play_cube[i][j][k] == 2)
						{
							play_cube[i][j][k] = 5;
							int b = magic_cube[i][j][k];
							AI[AI_count] = b;
							AI_count++;

							int temp = pos_win(i,j,k);
							if(temp>num_wins)
							{
								num_wins = temp;
								next_i = i;
								next_j = j;
								next_k = k;
							}
							play_cube[i][j][k] = 2;
							AI_count--;
						}
					}
				}
			}
			play_cube[next_i][next_j][next_k] = 5;
			int b = magic_cube[next_i][next_j][next_k];
			AI[AI_count] = b;
			AI_count++;
		}	
	}
}

void main_game()													//Compilation of diff functions to give gaming interface
{
	char ch;
	int pturn, inp;
	cout<<"Would you like your turn first(y/n): ";
	cin>>ch;
	switch(ch)
	{
		case 'n':
			game_on = 1;
			pturn = 1;
			break;
		case 'y':
			game_on = 1;
			pturn = 0;
			break;
		default: 
			cout<<"Please give valid input";
			break;
	}
	
	for (int i = 0; i < 27; i++)
	{
		if(i == 20)
		{
			game_on = 0;
			if (AI_lines > hum_lines)
			{
				cout<<"AI score: "<< AI_lines<<" Player score: "<<hum_lines;
				cout<<"\nAI wins\n";
			}
			else if(hum_lines > AI_lines)
			{
				cout<<"AI score: "<< AI_lines<<" Player score: "<<hum_lines;
				cout<<"\nPlayer wins\n";
			}
			else 
			{
				cout<<"AI score: "<< AI_lines<<" Player score: "<<hum_lines;
				cout<<"Draw\n";
			}
		}

		if(game_on == 1)
		{
			if(i%2 == pturn)
			{
				cout<<"Enter your desired place : ";
				Label: cin>>inp;
				if (0<inp<28)
				{
					if(play_n(inp) == false)
					{
						cout<<"Please try again.\n";
						goto Label;
					}
				}
				check_hum();
			}
			else
			{
				make_2();
				check_AI();
			}
			cout<<"\n\n";
			for (int as = 0; as < 3; as++)
			{
				for (int bs = 0; bs < 3; bs++)
				{
					for (int cs = 0; cs < 3; cs++)
					{
						cout<<magic_cube[as][bs][cs]<<" ";
					}
					cout<<"\t\t";
					for (int cs = 0; cs < 3; cs++)
					{
						if(play_cube[as][bs][cs] == 2)
							cout<<"_"<<" ";
						if(play_cube[as][bs][cs] == 3)
							cout<<"X"<<" ";
						if(play_cube[as][bs][cs] == 5)
							cout<<"O"<<" ";
					}
					cout<<"\n";
				}
				cout<<"\n";
			}
		}
	}
}

int main()
{
	generate_cube();												//generate magic cube
	solve_eq();														//solve eqn
	gen_surface();													//Generate all surfaces
	exceptions();													
	add_to_nc();													
	diff_surface();													//Create Exception lists

	for (int i = 0; i < dscount; i++)
	{
		cout<<diff_surf_sum_42[i][0]<<" "<<diff_surf_sum_42[i][1]<<" "<<diff_surf_sum_42[i][2]<<endl;
	}

	cout<<"Here are 11 required surfaces: "<<endl;
	display_11surface();							
	iniate_play_area();
	main_game();

	return 0;
}