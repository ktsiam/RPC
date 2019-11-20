import json

with open("structs.json", "r") as f:
    contents = f.read()
tree = json.loads(contents)

def filter_type(s):
    if len(s) < 2 or s[0:2] != "__":
        return s
    return s[2:]

def params(arg_types, arg_names):
    params = []
    for (t, n) in zip(arg_types, arg_names):
        base_type, sep, rest = t.partition('[')
        p = base_type + " " + n + sep + rest
        params.append(p)
    return params

def type_to_tuple(t):
    base_type, sep, rest = t.partition('[')
    if sep != '':
        return type_to_tuple(base_type) + sep + rest
    if t in ['int', 'bool', 'string', 'float', 'void']:
        return t
    else:
        return t + '_T'

# get tree[types]
def generate_tuple_types(types):
    s = ''
    f_types = filter(lambda t : 'members' in types[t], types)
    for t in f_types:
        s += 'using ' + type_to_tuple(filter_type(t)) \
          + ' = std::tuple<' \
          + ', '.join(reversed([type_to_tuple(filter_type(m['type'])) for m in types[t]['members']])) \
          + '>;\n'
        
    return s
            

def get_proxy_funcs(funcs):
    str_funs = []
    for fun in funcs:
        funname = fun
        ret_type = funcs[fun]['return_type']
        arg_types = [filter_type(arg['type']) 
                for arg in funcs[fun]['arguments']]
        arg_names = [arg['name'] 
                for arg in funcs[fun]['arguments']]

        
        
        s = ret_type + ' ' + funname + '('            \
          + ', '.join(params(arg_types, arg_names))   \
          + ') {\n\treturn reinterpret_cast<'         \
          + ret_type + '&&>(\n\t'                     \
          + 'serialize_and_apply<' + type_to_tuple(ret_type) + ', '  \
          + ", ".join(map(type_to_tuple, arg_types)) + '>(\n\t\t'       \
          + '"' + funname + '"'
          
        
        for t, n in zip(arg_types, arg_names):
            s += ',\n\t\treinterpret_cast<' + type_to_tuple(t) + '&>' \
              + '(' + n + '))'
                
                
        s += ');\n}'

        str_funs.append(s)
    return str_funs

def get_stub_funcs(funcs):
    str_funs = []
    for fun in funcs:
        funname = fun
        ret_type = funcs[fun]['return_type']
        arg_types = [filter_type(arg['type']) 
                for arg in funcs[fun]['arguments']]
        arg_names = [arg['name'] 
                for arg in funcs[fun]['arguments']]

        inter_funname = '__' + funname + '_inter'
        
        s = type_to_tuple(ret_type) + ' ' + inter_funname + '('
        sep = ''
        for t, n in zip(arg_types, arg_names):
            s += sep + type_to_tuple(t) + '& ' + n
            sep = ', '
        s += ') {\n\treturn reinterpret_cast<' \
          + type_to_tuple(ret_type) + '&&>(\n\t' \
          + funname + '('
        
        sep = ''
        for t, n in zip(arg_types, arg_names):
            s += '\n\t\t' + sep + 'reinterpret_cast<' + t + '&>' \
              + '(' + n + ')'
            sep = ', '
        s += '));\n}\n\n'
        
        s += 'void __' + funname     \
          + '() {\n\t__deserialize_and_apply<' \
          + type_to_tuple(ret_type) + ', ' \
          + ", ".join(map(type_to_tuple, arg_types))      \
          + '>(' + inter_funname + ');\n}' 
        
        str_funs.append(s)
        
    # dispatch function
    dispatch_fun = """void dispatchFunction() {
    char functionNameBuffer[50];
    getFunctionNameFromStream(functionNameBuffer,
                              sizeof(functionNameBuffer));
    if (!RPCSTUBSOCKET-> eof()) {"""
    
    for fun in funcs:
        dispatch_fun += '\n\tif (strcmp(functionNameBuffer,"' + fun + '") == 0) {\n\t\t' \
        + '__' + fun + '();\n\t\treturn;\n\t}'
       
    dispatch_fun += '\n\tassert(false && "function contract not found");\n\t}\n}'
    
    str_funs.append(dispatch_fun)
    return str_funs


def get_stub(tree):
    contents = """
#include "rpcstubhelper.h"
#include <tuple>
#include <cassert>
#include "structs.idl"\n\n""" 
    
    contents += generate_tuple_types(tree['types']) + '\n\n'
    contents += "\n\n".join(get_stub_funcs(tree['functions']))
    return contents

def get_proxy(tree):
    contents = """
#include "rpcproxyhelper.h"
#include <tuple>
#include "structs.idl"\n\n"""
    contents += generate_tuple_types(tree['types']) + '\n\n'
    contents += "\n\n".join(get_proxy_funcs(tree['functions']))
    return contents
