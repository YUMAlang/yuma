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

void Executor::execute()
{
	try
	{
		stack<double> st_num; //stack for processing number data
		stack<string> st_str; //stack for processing string data
		stack<int> st_assign; //stack for storing identifiers for assignment
		type_of_stack st_type;//type of current processing stack
		Lexeme curr_lex;
		unsigned int i, curr_pos = 0; //position in POLIZ code
		double n, num_op1, num_op2, num_res;
		string s, str_op1, str_op2, str_res;
		//cerr << san.poliz.code.size()<<endl;
		//main cycle
		while (curr_pos < san.poliz.code.size())
		{
			curr_lex = san.poliz.code.at(curr_pos);
			switch (curr_lex.table)
			{
			case num:
				//cerr<<"met number\n";
				st_type = num_stack;
				st_num.push(san.lan.Numbers.at(curr_lex.num));
				break;
			case str:
				//cerr<<"met string\n";
				st_type = str_stack;
				st_str.push(san.lan.Strings.at(curr_lex.num));
				break;
			case POLIZ_LABEL:
				//cerr<<"met poliz label\n";
				st_type = num_stack;
				st_num.push(curr_lex.num);
				break;
			case ASSIGN_POS:
				//cerr<<"met assign pos\n";
				st_assign.push(curr_lex.num);
				break;
			case id:
				//cerr<<"met identifier\n";
				if (san.lan.Identifiers.at(curr_lex.num).GetType() == undef_type) throw "undefined identifier";
				if (san.lan.Identifiers.at(curr_lex.num).GetType() == number_type)
				{
					st_type = num_stack;
					san.lan.Identifiers.at(curr_lex.num).TryGetVal(n); //n = value of current id
					st_num.push(n);
				}
				else
				{
					st_type = str_stack;
					san.lan.Identifiers.at(curr_lex.num).TryGetVal(s); 
					st_str.push(s);
				}
				break;
			case op: //got an operation
				cerr << "OP\n";	
				switch (curr_lex.num) //what is it?
				{
				case EQU:
					cerr << "EQU\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 == num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 == str_op2));
					}
					st_type = num_stack;
					break;
				case NOT_EQU:
					cerr << "NOT EQU\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 != num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 != str_op2));
					}
					st_type = num_stack;
					break;
				case LESS:
					cerr<<"LESS\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 < num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 < str_op2));
					}
					st_type = num_stack;
					break;
				case GRTR:
					cerr<<"GRTR\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 > num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 > str_op2));
					}
					st_type = num_stack;
					break;
				case LS_EQU:
					cerr<<"met ls_equ\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 <= num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 <= str_op2));
					}
					st_type = num_stack;
					break;
				case GR_EQU:
					cerr<<"GREQU";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 >= num_op2));
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_num.push((double)(int)(str_op1 >= str_op2));
					}
					st_type = num_stack;
					break;
				case LOGIC_AND:
					cerr<<"AND";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 && num_op2));
						st_type = num_stack;
					}
					else throw "incorrect type of LOGIC_AND operands";
					break;
				case LOGIC_OR:
					cerr<<"OR";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push((double)(int)(num_op1 || num_op2));
						st_type = num_stack;
					}
					else throw "incorrect type of LOGIC_OR operands";
					break;
				case LOGIC_NOT:
					cerr<<"NOT";
					if (st_type == num_stack)
					{
						num_op1 = st_num.top();
						st_num.pop();
						st_num.push((double)(int)(!num_op1));
						st_type = num_stack;
					}
					else throw "incorrect type of LOGIC_NOT operand";
					break;
				case PLUS:
					cerr << "PLUS\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						cerr << num_op1<< ' ' << num_op2 << endl;
						st_num.push(num_op1 + num_op2);
						st_type = num_stack;
					}
					else
					{
						get2ops(st_str, str_op1, str_op2);
						st_str.push(str_op1 + str_op2);
						st_type = str_stack;
					}
				case MINUS:
					cerr<< "MINUS\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 - num_op2);
						st_type = num_stack;
					}
					else throw "incorrect type of operator - operands";
					break;
				case MULT:
					cerr << "MUL\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 * num_op2);
						st_type = num_stack;
					}
					else throw "incorrect type of operator * operands";
					break;
				case DIV:
					cerr << "DIV\n";
					if (st_type == num_stack)
					{
						get2ops(st_num, num_op1, num_op2);
						st_num.push(num_op1 / num_op2);
						st_type = num_stack;
					}
					else throw "incorrect type of operator / operands";
					break;
				case GOTO:
					cerr << "GOTO\n";
					curr_pos = st_num.top() - 1;
					st_num.pop();
					break;
				case GOTO_ON_FALSE: //condition check required
					cerr << "GOTOF\n";
					get2ops(st_num, num_op1, num_op2);
					if (!num_op1) curr_pos = num_op2 - 1;
					break;
				case ASSIGN:
					cerr<<"met assignment\n";
					if (st_type == num_stack)
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
				case LIRE:
					cerr<<"met lire\n";
					i = st_assign.top();
					st_assign.pop();
					if (san.lan.Identifiers.at(i).GetType() == undef_type) {cerr<<i<<endl; throw "LIRE: undefined identifier";}
					if (san.lan.Identifiers.at(i).GetType() == number_type)
					{
						cin >> num_res;
						san.lan.Identifiers.at(i).Set(num_res);
					}
					else
					{
						cin >> str_res;
						san.lan.Identifiers.at(i).Set(str_res);
					}
					break;
				case ECRIRE:
					cerr << "ECRIRE\n";
					if (st_type == num_stack)
					{
						num_op1 = st_num.top();
						st_num.pop();
						cout << num_op1 << endl;
					}
					else
					{
						str_op1 = st_str.top();
						st_str.pop();
						cout << str_op1 << endl;
					}
					break;
				default: throw "wtf is this operation?";
				} //end of inner switch
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
	}
	catch (...)
	{
		cerr << "unknown exclusion O_o" << endl;
	}
}
