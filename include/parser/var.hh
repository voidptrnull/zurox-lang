#ifndef VAR_HH
#define VAR_HH

#include <node.hh>
#include <version.hh>

namespace parser
{
        enum VarType
        {
                V_I8,
                V_I16,
                V_I32,
                V_I64,
                V_I128,
                V_I128,

                V_U8,
                V_U16,
                V_U32,
                V_U64,
                V_U128,
                V_U128,

                V_F32,
                V_F64,
                V_80,
                V_F128,
                V_FN,
                V_STRUCT,
                V_ENUM,
                ERR
        };


        class Variable : public Node
        {
        public:
                VarType type;
                bool reference;
                token::Token variable;
        };
}

#endif