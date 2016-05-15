#include "Executor.hpp"

void Executor::execute()
{
    try
	{
	    Lexeme curr_lex;
	    Value op_1;
	    Value op_2;
	    Value buf;
	    double res_num;
	    string res_str;
	    int iter = 0;
	    while(iter < san.poliz.size())
		{
		    curr_lex = san.poliz.at(iter);
		    switch(curr_lex.table)
			{
			case id:
			    if(san.lan.get_type(curr_lex.num) == undef_type)
				throw Exception("undefined identifier");
			    args.push(san.lan.get_value(curr_lex.num));
			    break;

			case str:
			    args.push(Value(san.lan.get_string(curr_lex.num)));
			    break;

			case num:
			    args.push(Value(san.lan.get_num(curr_lex.num)));
			    break;

			case POLIZ_LABEL:
			    args.push(Value(pol_lab_type, curr_lex.num));
			    break;

			case ASSIGN_POS:
			    args.push(Value(ass_pos_type, curr_lex.num));
			    break;

			case op:

			    switch(curr_lex.num)
				{
				case EQU:
				case NOT_EQU:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == string_type)
						{
						    res_num = (op_1.s == op_2.s);
						}
					    else if(op_1.type == number_type)
						{
						    res_num = (op_1.n == op_2.n);
						}
					    else
						{
						    throw Exception("Bad == or != arguments");
						}
					    if(curr_lex.num == NOT_EQU)
						{
						    if(res_num == 0)
							res_num = 1;
						    else
							res_num = 0;
						}
					    args.push(Value(res_num));
					}
				    else
					{
					    throw Exception("Types mismatch in == or !=");
					}
				    break;

				case LOGIC_AND:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type && op_1.type == number_type)
					{
					    if(op_1.n != 0 && op_2.n != 0)
						res_num = 1;
					    else
						res_num = 0;
					    args.push(Value(res_num));
					}
				    else
					{
					    throw Exception("Types mismatch in &&");
					}
				    break;

				case LOGIC_OR:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type && op_1.type == number_type)
					{
					    if(op_1.n != 0 || op_2.n != 0)
						res_num = 1;
					    else
						res_num = 0;
					    args.push(Value(res_num));
					}
				    else
					{
					    throw Exception("Types mismatch in ||");
					}
				    break;

				case LOGIC_NOT:
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == number_type)
					{
					    if(op_1.n != 0)
						res_num = 0;
					    else
						res_num = 1;
					    args.push(Value(res_num));
					}
				    else
					{
					    throw Exception("Types mismatch in !");
					}
				    break;

				case PLUS:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == number_type)
						{
						    res_num = op_1.n + op_2.n;
						    args.push(Value(res_num));
						}
					    else if(op_1.type == string_type)
						{
						    res_str = op_1.s + op_2.s;
						    args.push(Value(res_str));
						}
					    else
						{
						    throw Exception("Bad + arguments");
						}
					}
				    else
					{
					    throw Exception("Types mismatch in +");
					}
				    break;

				case MINUS:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type && op_1.type == number_type)
					{
					    args.push(Value(op_1.n - op_2.n));
					}
				    else
					{
					    throw Exception("Types mismatch in -");
					}
				    break;

				case MULT:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type && op_1.type == number_type)
					{
					    args.push(Value(op_1.n * op_2.n));
					}
				    else
					{
					    throw Exception("Types mismatch in *");
					}
				    break;

				case DIV:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type && op_1.type == number_type)
					{
					    args.push(Value(op_1.n / op_2.n));
					}
				    else
					{
					    throw Exception("Types mismatch in /");
					}
				    break;

				case ASSIGN:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    buf = san.lan.get_value(op_1.i);
				    if(op_1.type != ass_pos_type)
					{
					    throw Exception(
					        "<internal error> expected ass_pos as 1st assignment argument");
					}
				    if(op_2.type == number_type)
					buf.Set(op_2.n);
				    else if(op_2.type == string_type)
					buf.Set(op_2.s);
				    else
					throw Exception("Bad types for =");
				    san.lan.set_value(op_1.i, buf);
				    break;

				case LESS:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == number_type)
						{
						    if(op_1.n < op_2.n)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else if(op_1.type == string_type)
						{
						    if(op_1.s < op_2.s)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else
						throw Exception("Bad < arguments");
					}
				    else
					{
					    throw Exception("Types mismatch in <");
					}
				    break;

				case GRTR:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == number_type)
						{
						    if(op_1.n > op_2.n)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else if(op_1.type == string_type)
						{
						    if(op_1.s > op_2.s)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else
						throw Exception("Bad > arguments");
					}
				    else
					{
					    throw Exception("Types mismatch in >");
					}
				    break;

				case LS_EQU:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == number_type)
						{
						    if(op_1.n <= op_2.n)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else if(op_1.type == string_type)
						{
						    if(op_1.s <= op_2.s)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else
						throw Exception("Bad <= arguments");
					}
				    else
					{
					    throw Exception("Types mismatch in <=");
					}
				    break;

				case GR_EQU:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == op_2.type)
					{
					    if(op_1.type == number_type)
						{
						    if(op_1.n >= op_2.n)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else if(op_1.type == string_type)
						{
						    if(op_1.s >= op_2.s)
							res_num = 1;
						    else
							res_num = 0;
						    args.push(Value(res_num));
						}
					    else
						throw Exception("Bad >= arguments");
					}
				    else
					{
					    throw Exception("Types mismatch in >=");
					}
				    break;

				case GOTO:
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == pol_lab_type)
					{
					    iter = op_1.i - 1; // One will be added later, by While loop
					}
				    else
					{
					    throw Exception("<internal error, tell Max> loop problem in goto");
					}
				    break;

				case GOTO_ON_FALSE:
				    op_2 = args.top();
				    args.pop();
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == number_type)
					{
					    if(op_2.type == pol_lab_type)
						{
						    if(op_1.n == 0)
							{
							    iter = op_2.i - 1;
							}
						}
					    else
						{
						    throw Exception(
						        "<internal error, tell Max> loop/if problem in goto_f");
						}
					}
				    else
					{
					    throw Exception("Logic expression expected in if or while condition");
					}
				    break;

				case LIRE:
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type != ass_pos_type)
					throw Exception("<internal error> expected ass_pos before LIRE");
				    buf = san.lan.get_value(op_1.i);
				    if(buf.type == number_type)
					{
					    cin >> res_num;
					    buf.Set(res_num);
					}
				    else
					{
					    if(buf.type == string_type)
						{
						    cin >> res_str;
						    buf.Set(res_str);
						}
					    else
						throw Exception("Undefined identifier");
					}
				    san.lan.set_value(op_1.i, buf);
				    break;

				case ECRIRE:
				    op_1 = args.top();
				    args.pop();
				    if(op_1.type == number_type)
					{
					    cout << op_1.n;
					}
				    else if(op_1.type == string_type)
					{
					    cout << op_1.s;
					}
				    else
					{
					    throw Exception("Types mismatch in ecrire");
					}
				    cout << endl;
				    break;

				default:
				    throw Exception("WTF is this operation");
				} // inner switch ended
			    break;

			default:
			    throw Exception("WTF is this table");
			} // outer switch ended
		    iter++;
		} // main loop ended
	}
    catch(Exception& ex)
	{
	    string s = "Runtime error: ";
	    s += ex.what();
	    throw Exception(s);
	}
    catch(...)
	{
	    throw;
	}
}
