#include <sstream>
#include <iostream>
#include "Jsones.h"


namespace Jsones
{
    enum class TokenType
    {
        CURLY_BRACKET_OPEN,
        CURLY_BRACKET_CLOSE,
        ANGLE_BRACKET_OPEN,
        ANGLE_BRACKET_CLOSE,
        COLON,
        COMMA,
        KEY
    };

    struct Token
    {
        TokenType type;
        std::string str;

        Token(TokenType ty, std::string s)
            :type( ty), str(s)
        {
            
        }

        std::string ToString() const;
    };


    void Tokenize(const char* js, std::vector<Token*>& tokens)
    {
        bool keyStarted = false;
        std::string key;
        int len = strlen(js);
        std::string ts = "";
        bool dittoMarked = false;
        for(int i = 0; i < len; i++)
        {
            if(js[i] == '\n' || js[i] == '\t')
            {
                continue;
            }
            ts = "";
            ts.append(1, js[i]);
            if(keyStarted)
            {
                if(js[i] == '\"')
                {
                    tokens.push_back(new Token(TokenType::KEY, key));
                    keyStarted = false;
                    dittoMarked = false;
                }
                else if(js[i] == ',' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new Token(TokenType::KEY, key));
                    tokens.push_back(new Token(TokenType::COMMA, ","));
                }
                else if(js[i] == '}' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new Token(TokenType::KEY, key));
                    tokens.push_back(new Token(TokenType::CURLY_BRACKET_CLOSE, "}"));
                }
                else if(js[i] == ']' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new Token(TokenType::KEY, key));
                    tokens.push_back(new Token(TokenType::ANGLE_BRACKET_CLOSE, "]"));
                }
                else
                {
                    if(js[i] == ' ')
                    {
                        if(dittoMarked)
                        {
                            key.append(1, js[i]);
                        }
                    }
                    else
                    {
                        key.append(1, js[i]);
                    }
                
                }
                continue;
            }

            if(js[i] == ' ')
            {
                continue;
            }
       
            else if(js[i] == '{')
            {
                tokens.push_back(new Token(TokenType::CURLY_BRACKET_OPEN, "{"));
            }
            else if(js[i] == '}')
            {
                tokens.push_back(new Token(TokenType::CURLY_BRACKET_CLOSE, "}"));
            }
            else if(js[i] == '[')
            {
                tokens.push_back(new Token(TokenType::ANGLE_BRACKET_OPEN, "["));
            }
            else if(js[i] == ']')
            {
                tokens.push_back(new Token(TokenType::ANGLE_BRACKET_CLOSE, "]"));
            }else if(js[i] == ',')
            {
                tokens.push_back(new Token(TokenType::COMMA, ","));
            }else if(js[i] == ':')
            {
                tokens.push_back(new Token(TokenType::COLON, ":"));
            }
            else if(js[i] == '\"')
            {
                dittoMarked = true;
                keyStarted = true;
                key = "";
            }
            else
            {
                key = "";
                key.append(1, js[i]);
                keyStarted = true;
            }
        }
    }


    void PrintJson(JObj* root, int tab)
    {
        if(root==nullptr)
        {
            return;
        }

        for(auto m : root->objects)
        {
            int i = 0;
            std::cout<<std::endl;
            while( i < tab)
            {
                std::cout<<"\t";
                i++;
            }
            if(m.second->type == JType::STR)
            {
                std::cout<<"[STR][" << m.first << "] : " << static_cast<JStr*>(m.second)->str;
            }
            else if(m.second->type == JType::BOOL)
            {
                std::cout<<"[BOOL][" << m.first << "] : " << (static_cast<JBool*>(m.second)->val ? "true" : "false");
            }
            else if(m.second->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(m.second);
                std::cout<<"[NUM][" << m.first << "] : ";
                std::cout<< (num->IsInteger() ? num->AsInt() : num->AsFloat());
            }
            else if(m.second->type == JType::ARR)
            {
               std::cout<<"[" << m.first << "] : " << "ARR : ";
            
                for(auto it : static_cast<JArr*>(m.second)->arr)
                {
                    if(it->type == JType::STR)
                    {
                        std::cout<<static_cast<JStr*>(it)->str << ", ";
                    }
                    else if(it->type == JType::NUM)
                    {
                        std::cout<<static_cast<JNumber*>(it)->str << ", ";
                    }
                    else if(it->type == JType::BOOL)
                    {
                        std::cout<<(static_cast<JBool*>(it)->val?"true" : "false") << ", ";
                    }
                    else if(it->type == JType::OBJ)
                    {
                        std::cout<<"{"<<std::endl;
                        PrintJson(static_cast<JObj*>(it), tab+1);
                        std::cout<<std::endl<<"}";
                    }
                }
            }
            else
            {
                std::cout<<"[" << m.first << "] : " << "OBJ" ;
                PrintJson(static_cast<JObj*>(m.second), tab+1);
            }
        }
    
    }


    JObj* JsonGenerator(std::vector<Token*>& tokens)
    {
        JObj* cursor = nullptr;
    
        JObj* root = new JObj(cursor);
        cursor = root;
        bool beforeColon = true;
        std::string lastKey = "";
        std::string value = "";
        JObj* valueAsObj = nullptr;
        for(int i = 0; i < tokens.size(); i++)
        {
            if(tokens[i]->type == TokenType::CURLY_BRACKET_OPEN)
            {
               JObj* obj = new JObj(cursor);
               cursor->AddValue(lastKey, obj);
               cursor = obj;
               beforeColon = true;
                lastKey = "";
                value = "";
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_CLOSE)
            {
                if(cursor->type == JType::ARR)
                {
                    static_cast<JArr*>(cursor)->PushBack(value);   
                   value = "";
                }
                else if(value != "")
                {
                   cursor->AddValue(lastKey, value);   
                   value = "";
                }
                lastKey = "";
                if(cursor->parentObj != nullptr && cursor->parentObj->type == JType::ARR)
                {
                    valueAsObj = cursor;
                }
                cursor = cursor->parentObj;
            }
            else if(tokens[i]->type == TokenType::ANGLE_BRACKET_OPEN)
            {
               JArr* arr = new JArr(cursor);
               cursor->AddValue(lastKey, arr);
               cursor = arr;
               beforeColon = true;
               lastKey = "";
               value = "";
            }
            else if(tokens[i]->type == TokenType::ANGLE_BRACKET_CLOSE)
            {
                if(valueAsObj!=nullptr)
                {
                    static_cast<JArr*>(cursor)->PushBack(valueAsObj);   
                    valueAsObj = nullptr;
                }
                else if(value != "")
                {
                    static_cast<JArr*>(cursor)->PushBack(value);   
                   value = "";
                }
                lastKey = "";
                cursor = cursor->parentObj;
            }
            else if(tokens[i]->type == TokenType::KEY)
            {
                if(tokens[i]->str == "templateOverridePath")
                {
                    std::cout<<"";
                }
                if(beforeColon && cursor->type!=JType::ARR)
                {
                    lastKey.append(tokens[i]->str);
                    value = "";
                }
                else
                {
                    value.append((tokens[i]->str));
                }
            }
            else if(tokens[i]->type == TokenType::COLON)
            {
               beforeColon = false;
               value = "";
            }
            else if(tokens[i]->type == TokenType::COMMA)
            {
                if(cursor->type == JType::ARR)
                {
                    if(valueAsObj!=nullptr)
                    {
                        static_cast<JArr*>(cursor)->PushBack(valueAsObj);   
                        valueAsObj = nullptr;
                    }
                    else
                    {
                         static_cast<JArr*>(cursor)->PushBack(value);
                        value = "";
                    }
                }
                else if(lastKey!="")
                {
                    cursor->AddValue(lastKey, value);
                    lastKey = "";
                    value = "";
                }
            
                beforeColon = true;
            }
            else
            {
           
            }
        
        }

        return root;
    }

    JVal::JVal(JType t)
     : type(t)
    {
    
    }

    JVal::~JVal()
    {
    }

    JStr::JStr(std::string s)
        : JVal(JType::STR), str(s)
    {
    
    }

    JStr::~JStr()
    {
    
    }

    JNumber::JNumber(std::string s)
        : JVal(JType::NUM), str(s)
    {
    }

    JNumber::JNumber(int v)
        :JVal(JType::NUM), str(std::to_string(v))
    {
    
    }

    JNumber::JNumber(float v)
        :JVal(JType::NUM), str(std::to_string(v))
    {
    }

    JNumber::~JNumber()
    {
    }

    bool JNumber::IsInteger()
    {
        for(int i = 0; i < str.length(); i++)
        {
            if(str[i] == '.')
            {
                return false;
            }
        }

        return true;
    }

    int JNumber::AsInt()
    {
        return std::stoi(str);
    }

    float JNumber::AsFloat()
    {
        return std::stof(str);
    }

    float JNumber::AsDouble()
    {
        return std::stod(str);
    }

    JBool::JBool(bool b)
        :JVal(JType::BOOL), val(b)
    {
    }

    JBool::~JBool()
    {
    }

    JObj::JObj(JObj* parent)
         : JVal(JType::OBJ), parentObj(parent)
    {
    
    }

    JObj::~JObj()
    {
        for(auto m : objects)
        {
            delete m.second;
        }
    }

    void JObj::AddArr(std::string key, JArr* arr)
    {
        objects.insert(std::pair<std::string, JArr*>(key, arr));
    }

    void JObj::AddObj(std::string key, JObj* obj)
    {
        objects.insert(std::pair<std::string, JVal*>(key, obj));
    }

    void JObj::AddFloat(std::string key, float value)
    {
        objects.insert(std::pair<std::string, JVal*>(key, new JNumber(value)));
    }

    void JObj::AddInt(std::string key, int value)
    {
        objects.insert(std::pair<std::string, JVal*>(key, new JNumber(value)));
    }

    void JObj::AddString(std::string key, std::string value)
    {
        objects.insert(std::pair<std::string, JVal*>(key, new JStr(value)));
    }


    void JObj::AddBool(std::string key, bool value)
    {
        objects.insert(std::pair<std::string, JVal*>(key, new JBool(value)));
    }

    void JObj::AddValue(std::string key, std::string val)
    {
        bool isNumber = true;
        if(val.length() == 0 )
        {
            isNumber = false;
        }
        for(int i = 0; i < val.length(); i++)
        {
           if(val[i] != '.' && (val[i] < '0' || val[i] > '9'))
           {
               isNumber = false;
           }
        }
    
        if(isNumber)
        {
            objects.insert(std::pair<std::string, JVal*>(key, new JNumber(val)));
        }
        else if(val == "false")
        {
            AddBool(key, false);
        }
        else if(val=="true")
        {
            AddBool(key, true);
        }
        else
        {
            AddString(key, val);
        }
    }

    void JObj::AddValue(std::string s, JVal* val)
    {
        objects.insert(std::pair<std::string, JVal*>(s, val));
    }

    void JArr::PushBack(JVal* val)
    {
        arr.push_back(val);
    }

    void JArr::PushBack(std::string s)
    {
        arr.push_back(new JStr(s));
    }

    JVal* JObj::operator[](std::string key)
    {
        std::map<std::string,JVal*>::iterator it;
        it = objects.find(key);
        if(it != objects.end())
        {
            return it->second;
        }

        return nullptr;
    }

    JVal* JObj::operator[](const char* str)
    {
        std::string key (str);
        std::map<std::string,JVal*>::iterator it;
        it = objects.find(key);
        if(it != objects.end())
        {
            return it->second;
        }
        return nullptr;
    }

    JArr::JArr(JObj* parent)
        : JObj(parent)
    {
        type = JType::ARR;
    }

    JArr::~JArr()
    {
        for(auto m : arr)
        {
            delete m;
        }
    }

    void JArr::AddObj(JObj* obj)
    {
        arr.push_back(obj);
    }

    void JArr::AddFloat(float value)
    {
        arr.push_back(new JNumber(value));
    }

    void JArr::AddInt(int value)
    {
        arr.push_back(new JNumber(value));
    }

    void JArr::AddString(std::string value)
    {
        arr.push_back(new JStr(value));
    }

    void JArr::AddBool(bool value)
    {
        arr.push_back(new JBool(value));
    }

    std::string Token::ToString() const
    {
        switch (type)
        {
            case TokenType::CURLY_BRACKET_OPEN: return std::string("CURLY_BRACKET_OPEN");
            case TokenType::CURLY_BRACKET_CLOSE: return std::string("CURLY_BRACKET_CLOSE");
            case TokenType::ANGLE_BRACKET_OPEN: return std::string("ANGLE_BRACKET_OPEN");
            case TokenType::ANGLE_BRACKET_CLOSE: return std::string("ANGLE_BRACKET_CLOSE");
            case TokenType::COLON: return std::string("COLON");
            case TokenType::COMMA: return std::string("COMMA");
            case TokenType::KEY: return std::string("KEY");
            default: return std::string("NONE");
        }
    }

    JObj* JParse(const char* str)
    {
        using namespace std;
        vector<Token*> tokens;
        Tokenize(str, tokens);
        JObj* obj = JsonGenerator(tokens);

        for(auto token : tokens)
        {
            delete token;
        }
    
        return obj;
    }

    std::string beautifiedTab ("\t");
    std::string beautifiedNewLine ("\n");
    std::string beautifiedSpace (" ");
    std::string emptyString ("");

    std::string& NewLine(bool beautify)
    {
        if(beautify)
        {
            return beautifiedNewLine;
        }
        return emptyString;
    }

    std::string& Tab(bool beautify)
    {
        if(beautify)
        {
            return beautifiedTab;
        }
        return emptyString;
    }

    std::string& Space(bool beautify)
    {
        if(beautify)
        {
            return beautifiedSpace;
        }
        return emptyString;
    }

    std::string Tab(bool beautify, int count)
    {
        if(beautify)
        {
            std::string str;
            for(int i = 0; i < count; i++)
            {
                str+=beautifiedTab;
            }
            return str;
        }
        return emptyString;
    }

    std::stringstream JWrite(JObj* root, float tab, bool beautify)
    {
        std::stringstream ss;

        if(root==nullptr)
        {
            return ss;
        }
    
        ss<<NewLine(beautify)<<Tab(beautify, tab) << "{" << NewLine(beautify);
        tab++;
        bool first = true;
        for(auto m : root->objects)
        {
            if(!first)
            {
                ss<<","<< NewLine(beautify);
            }
            ss<<Tab(beautify, tab);
            first = false;
        
            if(m.second->type == JType::STR)
            {
                ss<<"\"" << m.first <<"\""<<Space(beautify)<<":"<<Space(beautify)<<"\""<< static_cast<JStr*>(m.second)->str << "\""; 
            }
            else if(m.second->type == JType::BOOL)
            {
                ss<<"\"" << m.first <<"\""<<Space(beautify)<<":"<<Space(beautify)<< (static_cast<JBool*>(m.second)->val ? "true" : "false"); 
            }
            else if(m.second->type == JType::NUM)
            {
                JNumber* num = static_cast<JNumber*>(m.second);
                ss<<"\"" << m.first <<"\"" <<Space(beautify)<< ":"<<Space(beautify) << num->str; 
            }
            else if(m.second->type == JType::ARR)
            {
                if(m.first != "")
                {
                    ss<<"\""<<m.first<<"\""<<Space(beautify)<<":";
                }
                ss<<NewLine(beautify)<<Tab(beautify, tab);
                ss<<"["<<NewLine(beautify); 
            
                bool subFirst = true;
                for(auto it : static_cast<JArr*>(m.second)->arr)
                {
                    if(!subFirst)
                    {
                        ss<<","<<NewLine(beautify);
                    }
                    ss<<Tab(beautify, tab+1);
              
                    subFirst = false;
                    if(it->type == JType::STR)
                    {
                        ss<<Space(beautify)<<"\""<<static_cast<JStr*>(it)->str<<"\"";
                    }
                    else if(it->type == JType::NUM)
                    {
                        ss<<Space(beautify)<<static_cast<JNumber*>(it)->str;
                    }
                    else if(it->type == JType::BOOL)
                    {
                        ss<<Space(beautify)<<(static_cast<JBool*>(it)->val ? "true" : "false");
                    }
                    else if(it->type == JType::OBJ)
                    {
                        ss<<JWrite(static_cast<JObj*>(it), tab+1, beautify).rdbuf();
                        //PrintJson(static_cast<JObj*>(it), tab+1);
                    }
                }
                ss<<NewLine(beautify)<<Tab(beautify, tab)<<"]";
            }
            else if(root->type == JType::OBJ)
            {
                ss<<"\"" << m.first << "\""<< Space(beautify) << ":";
                ss<<JWrite(static_cast<JObj*>(m.second), tab+1, beautify).rdbuf();
            }
        }

    
        ss<<NewLine(beautify) << Tab(beautify, tab-1)<<"}";
        return ss;
    }
}
