

def function_banner_maker():
    Arg_List = []
    Arg_Desc = []
    index = 0
    print("Enter function name:\n")
    Func_name = input()
    print("Enter function description:\n")
    Func_Desc = input()
    print("Enter function return type\n")
    Func_Return = input()
    print("Enter function return description\n")
    Func_Return_Description = input()
    print("Enter No. of function input arguments:\n")
    Max_Function_Ip_Args = int(input())
    for index in range(0, Max_Function_Ip_Args):
        print("Enter argument name:\n")
        Arg_List.append(input())
        print("Enter argument description:\n")
        Arg_Desc.append(input())
    index = 0
    print("/*------------------------------------------------------------")
    print("|")
    print("| Function Name: {}".format(Func_name))
    print("| Function Description: {}".format(Func_Desc))
    print("|")
    print("|-------------------------------------------------------------")
    print("|")
    print("| Function Inputs: ")
    for index in range(0, Max_Function_Ip_Args):
        print("| {} : {}".format(Arg_List[index], Arg_Desc[index]))
        print("| ")  
    print("|-------------------------------------------------------------")
    print("|")
    print("| Function Output:")
    print("| {} : {}".format(Func_Return, Func_Return_Description))
    print("|")
    print("-------------------------------------------------------------*/")