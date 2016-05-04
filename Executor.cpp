#include "Executor.hpp"

void Executor::get2ops(stack<double>& st, double& op1, double& op2)
{
	op2 = st.top();
	st.pop();
	op1 = st.top();
	st.pop();
}

void Executor::get2ops(stack<string>& st, string& op1, string& op2)
{
	op2 = st.top();
	st.pop();
	op1 = st.top();
	st.pop();
}

Executor::execute(vector<Lexeme> code)
{
	try
	{
		stack<double> st_num; //stack for processing number data
		stack<string> st_str; //stack for processing string data
		stack<int> st_assign; //stack for storing identifiers for assignment
		type_of_stack st_type;//type of current processing stack
		Lexeme curr_lex;
		int i, curr_pos = 0; //position in POLIZ code
		double n, num_op1, num_op2, num_res;
		string s, str_op1, str_op2, str_res;
		//main cycle
		while (curr_pos < code.size)
		{
			curr_lex = code.at(curr_pos);
			switch (curr_lex.table)
			{
			case num:
				st_type = num;
				st_num.push(san.lan.Numbers.at(curr_lex.num));
				break;
			case str:
				st_type = str;
				st_str.push(san.lan.Strings.at(curr_lex.num));
				break;
			case POLIZ_LABEL:
				st_type = num;
				st_num.push(curr_lex.num);
				break;
			case ASSIGN_POS:
				st_assign.push(curr_lex.num);
				break;
			case id:
				if (san.lan.Identifiers.at(curr_lex.num).GetType == undef_type) throw "undefined identifier";
				if (san.lan.Identifiers.at(curr_lex.num).GetType == number_type)
				{
					st_type = num;
					san.lan.Identifiers.at(curr_lex.num).TryGetVal(n); //n = value of current id
					st_num.push(n);
				}
				else
				{
					st_type = str;
					san.lan.Identifiers.at(curr_lex.num).TryGetVal(s); 
					st_str.push(s);
				}
				break;
			case op: //got an operation
				switch (curr_lex.num) //what is it?
				{
				case EQU:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 == num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 == str_op2));
					}
					st_type = num;
					break;
				case NOT_EQU:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 != num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 != str_op2));
					}
					st_type = num;
					break;
				case LESS:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 < num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 < str_op2));
					}
					st_type = num;
					break;
				case GRTR:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 > num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 > str_op2));
					}
					st_type = num;
					break;
				case LS_EQU:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 <= num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 <= str_op2));
					}
					st_type = num;
					break;
				case GR_EQU:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 >= num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 >= str_op2));
					}
					st_type = num;
					break;
				case LOGIC_AND:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 && num_op2));
						st_type = num;
					}
					else throw "incorrect type of LOGIC_AND operands";
					break;
				case LOGIC_OR:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 || num_op2));
						st_type = num;
					}
					else throw "incorrect type of LOGIC_OR operands";
					break;
				case LOGIC_NOT:
					if (st_type == num)
					{
						num_op1 = st_num.top();
						st_num.pop();
						st_num.push((double)(int)(!num_op1));
						st_type = num;
					}
					else throw "incorrect type of LOGIC_NOT operand";
					break;
				case PLUS:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 + num_op2);
						st_type = num;
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_str.push(str_op1 + str_op2);
						st_type = str;
					}
				case MINUS:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 - num_op2);
						st_type = num;
					}
					else throw "incorrect type of operator - operands";
					break;
				case MULT:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 * num_op2);
						st_type = num;
					}
					else throw "incorrect type of operator * operands";
					break;
				case DIV:
					if (st_type == num)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 / num_op2);
						st_type = num;
					}
					else throw "incorrect type of operator / operands";
					break;
				case GOTO:
					curr_pos = st_num.top() - 1;
					st_num.pop();
					break;
				case GOTO_ON_FALSE: //ñondition check required
					get2ops(st_num, num_op1, num_op2);
					if (!num_op1) curr_pos = num_op2 - 1;
					break;
				case ASSIGN:
					if (st_type == num)
					{
						num_op2 = st_num.top();
						i = st_assign.top();
						st_assign.pop();
						if (!san.lan.Identifiers.at(i).Set(num_op2)) throw "incompatible types for assignment";
					}
					else
					{
						str_op2 = st_str.top();
						i = st_assign.top();
						st_assign.pop();
						if (!san.lan.Identifiers.at(i).Set(str_op2)) throw "incompatible types for assignment";
					}
					break;
				} //end of inner switch
				break;
			case kw:
				if (curr_lex.num == LIRE)
				{
					i = st_assign.top();
					st_assign.pop();
					if (san.lan.Identifiers.at(i).GetType == undef_type) throw "undefined identifier";
					if (san.lan.Identifiers.at(i).GetType == number_type)
					{
						cin >> num_res;
						san.lan.Identifiers.at(i).Set(num_res);
					}
					else
					{
						cin >> str_res;
						san.lan.Identifiers.at(i).Set(str_res);
					}
				}
				else //ECRIRE
				{
					if (st_type = num)
					{
						num_op1 = st_num.top();
						st_num.pop();
						cout << num_op1;
					}
					else
					{
						str_op1 = st_str.top();
						st_str.pop();
						cout << str_op1;
					}
				}
				break;
			default: throw "wtf is this table?";
			} //end of big switch
			curr_pos++;
		} //end of the main cycle
		cout << "Execution is finished!" << endl;
	}
	catch (const char *s)
	{
		cerr << s << endl;
		return 1;
	}
	catch (...)
	{
		cerr << "unknown exclusion O_o" << endl;
		return 1;
	}
}