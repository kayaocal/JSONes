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
        size_t len = strlen(js);
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

    std::pair<std::string, JVal*> JPair(const std::string& key, const char* val)
    {
        return std::pair<std::string, JVal*>(key, new JStr(val));
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, const std::string& val)
    {
        return std::pair<std::string, JVal*>(key, new JStr(val));
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, double val)
    {
        return std::pair<std::string, JVal*>(key, new JNumber(val));
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, float val)
    {
        return std::pair<std::string, JVal*>(key, new JNumber(val));
    }
 
    std::pair<std::string, JVal*> JPair(const std::string& key, bool val)
    {
        return std::pair<std::string, JVal*>(key, new JBool(val));
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, int val)
    {
        return std::pair<std::string, JVal*>(key, new JNumber(val));
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, JObj* val)
    {
        return std::pair<std::string, JVal*>(key, val);
    }
    

    std::pair<std::string, JVal*> JPair(const std::string& key, JObj&& val)
    {
         JObj* obj = new JObj(std::move(val));
        return std::pair<std::string, JVal*>(key, obj);
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, JArr* arr)
    {
        return std::pair<std::string, JVal*>(key, arr);
    }

    std::pair<std::string, JVal*> JPair(const std::string& key, JArr& arr)
    {
        return std::pair<std::string, JVal*>(key, &arr);
    }

    JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JVal* jVal)
    {
        return JSONES_API std::pair<std::string, JVal*>(key, jVal);
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
                std::cout<<"[STR][" << m.first << "] : " << dynamic_cast<JStr*>(m.second)->str;
            }
            else if(m.second->type == JType::BOOL)
            {
                std::cout<<"[BOOL][" << m.first << "] : " << (dynamic_cast<JBool*>(m.second)->val ? "true" : "false");
            }
            else if(m.second->type == JType::NUM)
            {
                JNumber* num = dynamic_cast<JNumber*>(m.second);
                std::cout<<"[NUM][" << m.first << "] : ";
                std::cout<< (num->IsInteger() ? num->AsInt() : num->AsFloat());
            }
            else if(m.second->type == JType::ARR)
            {
               std::cout<<"[ARR][" << m.first << "] : ";
            
                for(auto it : dynamic_cast<JArr*>(m.second)->arr)
                {
                    if(it->type == JType::STR)
                    {
                        std::cout<<dynamic_cast<JStr*>(it)->str << ", ";
                    }
                    else if(it->type == JType::NUM)
                    {
                        std::cout<<dynamic_cast<JNumber*>(it)->str << ", ";
                    }
                    else if(it->type == JType::BOOL)
                    {
                        std::cout<<(dynamic_cast<JBool*>(it)->val?"true" : "false") << ", ";
                    }
                    else if(it->type == JType::OBJ)
                    {
                        std::cout<<"{"<<std::endl;
                        PrintJson(dynamic_cast<JObj*>(it), tab+1);
                        std::cout<<std::endl<<"}";
                    }
                }
            }
            else
            {
                std::cout<<"[OBJ][" << m.first << "] : " ;
                PrintJson(dynamic_cast<JObj*>(m.second), tab+1);
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
               cursor->Add(JPair(lastKey, obj));
               cursor = obj;
               beforeColon = true;
                lastKey = "";
                value = "";
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_CLOSE)
            {
                if(cursor->type == JType::ARR)
                {
                    dynamic_cast<JArr*>(cursor)->PushBack(value);   
                   value = "";
                }
                else if(value != "")
                {
                   cursor->AddToObjFromParsedString(lastKey, value);   
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
               cursor->Add(JPair(lastKey, arr));
               cursor = arr;
               beforeColon = true;
               lastKey = "";
               value = "";
            }
            else if(tokens[i]->type == TokenType::ANGLE_BRACKET_CLOSE)
            {
                if(valueAsObj!=nullptr)
                {
                    dynamic_cast<JArr*>(cursor)->PushBack(valueAsObj);   
                    valueAsObj = nullptr;
                }
                else if(value != "")
                {
                    dynamic_cast<JArr*>(cursor)->PushBack(value);   
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
                        dynamic_cast<JArr*>(cursor)->PushBack(valueAsObj);   
                        valueAsObj = nullptr;
                    }
                    else
                    {
                         dynamic_cast<JArr*>(cursor)->PushBack(value);
                        value = "";
                    }
                }
                else if(lastKey!="")
                {
                    cursor->AddToObjFromParsedString(lastKey, value);
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

    void JVal::operator=(const int& v)
    {
        jassert(type == JType::NUM, "integer type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if(num != nullptr)
        {
            num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const float& v)
    {
        jassert(type == JType::NUM, "float type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if(num != nullptr)
        {
            num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const double& v)
    {
        jassert(type == JType::NUM, "double type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if(num != nullptr)
        {
            num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const bool v)
    {
        jassert(type == JType::BOOL, "bool type can only be assigned to JBool types");
        JBool* num = dynamic_cast<JBool*>(this);
        if(num != nullptr)
        {
            num->val = v;
        }
    }

    void JVal::operator=(const std::string& str)
    {
        jassert(type == JType::STR, "std::string type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if(num != nullptr)
        {
            num->str = str;
        }
    }

    void JVal::operator=(const char* str)
    {
        jassert(type == JType::STR, "const char* type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if(num != nullptr)
        {
            num->str = str;
        }
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

    JNumber::JNumber(double d)
        : JVal(JType::NUM), str(std::to_string(d))
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

    double JNumber::AsDouble()
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
            std::cout<<"[DELETE] : " << m.first<<std::endl;
            delete m.second;
        }
    }

    JObj::JObj(JObj&& objR) noexcept
        :JVal(JType::OBJ), parentObj((objR.parentObj))
    {
        objR.parentObj = nullptr;
        objects.insert(std::make_move_iterator(begin(objR.objects)),
            std::make_move_iterator(end(objR.objects)));
        objR.objects.clear();
    }

    JObj::JObj(std::initializer_list<std::pair<std::string, JVal*>>list)
        : JVal(JType::OBJ), parentObj(nullptr)
    {
        auto it = list.begin();
        while (it != list.end())
        {
            Add(*it);
            it++;
        }
    }

    void JObj::Add(std::pair<std::string, JVal*> add)
    {
        objects.insert(add);
    }

    void JObj::Add(std::pair<std::string, JArr*> add)
    {
        objects.insert(add);
        add.second->parentObj = this;
    }

    void JObj::Add(std::pair<std::string, JObj*> add)
    {
        objects.insert(add);
        add.second->parentObj = this;
    }

    void JObj::AddToObjFromParsedString(std::string key, std::string val)
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
            Add(JPair(key, val));
        }
        else if(val == "false")
        {
           Add(JPair(key, false));
        }
        else if(val=="true")
        {
            Add(JPair(key, true));
        }
        else
        {
            Add(JPair(key, val));
        }
    }

    JVal* JObj::Get(const std::string& key)
    {
        auto found = objects.find(key);
        if (found != objects.end())
        {
            std::cout << "Succesfully Found!"<<std::endl;
            return found->second;
        }
        

        return nullptr;
    }

    JObj* JObj::GetObj(const std::string& key)
    {
        JVal* val = Get(key);
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::OBJ, "Given key is not an JObj*");

        return dynamic_cast<JObj*>(val);
    }

    JArr* JObj::GetArr(const std::string& key)
    {
        JVal* val = Get(key);
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::ARR, "Given key is not an JArr*");

        return dynamic_cast<JArr*>(val);
    }

    JVal& JObj::operator[](const std::string&str)
    {
        JVal* val = Get(str);
        jassert(val!=nullptr , "given key is not exist");
        return *val;
    }


    void JArr::PushBack(JVal* val)
    {
        arr.push_back(val);
    }

    void JArr::PushBack(std::string s)
    {
        arr.push_back(new JStr(s));
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

    void JArr::Add(JObj* obj)
    {
        arr.push_back(obj);
    }
    
    void JArr::Add(double value)
    {
        arr.push_back(new JNumber(value));
    }
    void JArr::Add(int value)
    {
        arr.push_back(new JNumber(value));
    }
    void JArr::Add(float value)
    {
        arr.push_back(new JNumber(value));
    }
    void JArr::Add(std::string& value)
    {
        arr.push_back(new JStr(value));
    }

    void JArr::Add(const char* str)
    {
        arr.push_back(new JStr(str));
    }

    void JArr::Add(bool value)
    {
        arr.push_back(new JBool(value));
    }

    JVal& JArr::operator[](int index)
    {
        jassert(index < arr.size(), "JArr index exceeds vector size!");
        return *arr[index];
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

    std::string beautifiedTab ("    ");
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

    std::stringstream JWrite(JArr* arr, bool beautify, int tab)
    {
        std::stringstream ss;
        ss << NewLine(beautify) << Tab(beautify, tab);
        ss << "[" << NewLine(beautify);

        bool subFirst = true;
        for (auto it : arr->arr)
        {
            if (!subFirst)
            {
                ss << "," << NewLine(beautify);
            }
            ss << Tab(beautify, tab + 1);

            subFirst = false;
            if (it->type == JType::STR)
            {
                ss << Space(beautify) << "\"" << dynamic_cast<JStr*>(it)->str << "\"";
            }
            else if (it->type == JType::NUM)
            {
                ss << Space(beautify) << dynamic_cast<JNumber*>(it)->str;
            }
            else if (it->type == JType::BOOL)
            {
                ss << Space(beautify) << (dynamic_cast<JBool*>(it)->val ? "true" : "false");
            }
            else if (it->type == JType::OBJ)
            {
                ss << JWrite(dynamic_cast<JObj*>(it), tab + 1, beautify).rdbuf();
            }
        }
        ss << NewLine(beautify) << Tab(beautify, tab) << "]";
        return ss;
    }

    std::stringstream JWrite(JObj* root, bool beautify, int tab)
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
                ss<<"\"" << m.first <<"\""<<Space(beautify)<<":"<<Space(beautify)<<"\""<< dynamic_cast<JStr*>(m.second)->str << "\""; 
            }
            else if(m.second->type == JType::BOOL)
            {
                ss<<"\"" << m.first <<"\""<<Space(beautify)<<":"<<Space(beautify)<< (dynamic_cast<JBool*>(m.second)->val ? "true" : "false"); 
            }
            else if(m.second->type == JType::NUM)
            {
                JNumber* num = dynamic_cast<JNumber*>(m.second);
                ss<<"\"" << m.first <<"\"" <<Space(beautify)<< ":"<<Space(beautify) << num->str; 
            }
            else if(m.second->type == JType::ARR)
            {
                if(m.first != "")
                {
                    ss<<"\""<<m.first<<"\""<<Space(beautify)<<":";
                }
                ss<<JWrite(dynamic_cast<JArr*>(m.second), beautify, tab).rdbuf();
                
            }
            else if(root->type == JType::OBJ)
            {
                ss<<"\"" << m.first << "\""<< Space(beautify) << ":";
                ss<<JWrite(dynamic_cast<JObj*>(m.second), tab+1, beautify).rdbuf();
            }
        }
    
        ss<<NewLine(beautify) << Tab(beautify, tab-1)<<"}";
        return ss;
    }
}
