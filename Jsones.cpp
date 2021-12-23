#include <sstream>
#include <iostream>
#include <fstream>
#include "Jsones.h"

#include "build/lookup3.h"


namespace Jsones
{
    enum class TokenType : char
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

         Token(TokenType ty)
            : type(ty)
        {
        }
        
        std::string ToString() const;
    };

    struct StrToken : public Token
    {
        size_t beginIndex;
        size_t endIndex;

        StrToken(TokenType ty, size_t begin, size_t end)
            : Token(ty), beginIndex(begin), endIndex(end)
        {
        }
    };
    std::string GetStr(const char* str, int b, int e);

    bool strCmp(const char* str, int beg, int end, const char* comp);
    std::map<uint32_t, std::string> stringHashes;

    Token* TokenPtrComma = new Token(TokenType::COMMA);
    Token* TokenPtrColon = new Token(TokenType::COLON);
    Token* TokenPtrCurlyBracketOpen = new Token(TokenType::CURLY_BRACKET_OPEN);
    Token* TokenPtrCurlyBracketClose = new Token(TokenType::CURLY_BRACKET_CLOSE);
    Token* TokenPtrAngleBracketOpenToken = new Token(TokenType::ANGLE_BRACKET_OPEN);
    Token* TokenPtrAngleBracketClose = new Token(TokenType::ANGLE_BRACKET_CLOSE);


    void Tokenize(const char* js, std::vector<Token*>& tokens)
    {
        bool keyStarted = false;
        size_t len = strlen(js);
        bool dittoMarked = false;
        int indexS;
        for (int i = 0; i < len; i++)
        {
            if (keyStarted)
            {
                if (js[i] == '\"' || (js[i] == '\n' && js[i-1] != '\\') || (js[i] == '\t' && js[i-1] != '\\') || js[i] == '\r')
                {
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    keyStarted = false;
                    dittoMarked = false;
                }
                else if (js[i] == ',' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrComma);
                }
                else if (js[i] == '}' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrCurlyBracketClose);
                }
                else if (js[i] == ']' && !dittoMarked)
                {
                    keyStarted = false;
                    tokens.push_back(new StrToken(TokenType::KEY, indexS, i));
                    tokens.push_back(TokenPtrAngleBracketClose);
                }
             
                continue;
            }

            if (js[i] == ' ')
            {
                continue;
            }
            if (js[i] == '\n' || js[i] == '\t' || js[i] == '\r')
            {
                continue;
            }

            else if (js[i] == '{')
            {
                tokens.push_back(TokenPtrCurlyBracketOpen);
            }
            else if (js[i] == '}')
            {
                tokens.push_back(TokenPtrCurlyBracketClose);
            }
            else if (js[i] == '[')
            {
                tokens.push_back(TokenPtrAngleBracketOpenToken);
            }
            else if (js[i] == ']')
            {
                tokens.push_back(TokenPtrAngleBracketClose);
            }
            else if (js[i] == ',')
            {
                tokens.push_back(TokenPtrComma);
            }
            else if (js[i] == ':')
            {
                tokens.push_back(TokenPtrColon);
            }
            else if (js[i] == '\"')
            {
                dittoMarked = true;
                keyStarted = true;
                indexS = i+1;
            }
            else
            {
                keyStarted = true;
                indexS = i;
            }
          
        }

    }

    // std::pair<std::string, JVal*> JPair(const std::string& key, const char* val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JStr(val));
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, const std::string& val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JStr(val));
    // }

    // std::pair<std::string, JVal*> JPair(const std::string& key, double val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JNumber(val));
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, float val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JNumber(val));
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, bool val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JBool(val));
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, int val)
    // {
    //     return std::pair<std::string, JVal*>(key, new JNumber(val));
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, JObj* val)
    // {
    //     return std::pair<std::string, JVal*>(key, val);
    // }
    //
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, JObj&& val)
    // {
    //     JObj* obj = new JObj(std::move(val));
    //     return std::pair<std::string, JVal*>(key, obj);
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, JArr* arr)
    // {
    //     return std::pair<std::string, JVal*>(key, arr);
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, JArr&& arr)
    // {
    //     JArr* marr = new JArr(std::move(arr));
    //     return std::pair<std::string, JVal*>(key, marr);
    // }
    //
    // std::pair<std::string, JVal*> JPair(const std::string& key, JArr& arr)
    // {
    //     return std::pair<std::string, JVal*>(key, &arr);
    // }
    //
    // JSONES_API std::pair<std::string, JVal*> JPair(const std::string& key, JVal* jVal)
    // {
    //     return JSONES_API std::pair<std::string, JVal*>(key, jVal);
    // }

    std::string PrintKey(uint32_t key)
    {
        auto it  = stringHashes.find(key);
        if(it!=stringHashes.end())
        {
            return it->second;
        }

        return "";
    }

    const char* GetTypeStr(JType t)
    {
        switch (t)
        {
            case JType::ARR:
                return "[ARR]";
            case JType::OBJ:
                return "[OBJ]";
            case JType::NUL:
                return "[NULL]";
            case JType::NUM:
                return "[NUM]";
            case JType::STR:
                return "[STR]";
            case JType::BOOL:
                return "[BOOL]";
            default: return "[NONE]";
        }
    }
    
    void PrintArray(JArr* arr, int tab);
    void PrintObj(JObj* obj, int tab)
    {
        for (auto m : obj->objects)
        {
            int i = 0;
            std::cout << std::endl;
            while (i < tab)
            {
                std::cout << "\t";
                i++;
            }
            if (m.second->type == JType::ARR)
            {
                std::cout << "[ARR][" << PrintKey(m.first) << "] : ";

                PrintArray(dynamic_cast<JArr*>(m.second), tab);
                
            }
            else if(m.second->type == JType::OBJ)
            {
                std::cout << "[OBJ][" << PrintKey(m.first) << "] : ";
                PrintObj(dynamic_cast<JObj*>(m.second), tab + 1);
            }
            else 
            {
                std::cout << GetTypeStr(m.second->type)<<"[" << PrintKey(m.first) << "] : " << m.second->ToString();
            }
        }
    }
    
    void PrintArray(JArr* arr, int tab)
    {
        if (arr == nullptr)
        {
            return;
        }

         for (auto it :arr->arr)
        {
            
            if (it->type == JType::OBJ)
            {
                std::cout << "{" << std::endl;
                PrintObj(dynamic_cast<JObj*>(it), tab + 1);
                std::cout << std::endl << "}";
            }
            else if (it->type == JType::STR || it->type == JType::NUM || it->type == JType::BOOL || it->type == JType::NUL)
            {
                std::cout << it->ToString() << ", ";
            }
        }
        
    }
    
    bool strCmp(const char* str, int beg, int end, const char* comp)
    {
        if(strlen(comp) != end - beg)
        {
            return false;
        }

        int index = 0;
        for(int i = beg; i < end; i++)
        {
            if(str[i] != comp[index])
            {
                return false;
            }
            index++;
        }
        return true;
    }
    JVal* GetValueTypeByString(const char* str, int beg, int end)
    {
        bool isNumber = true;
        if (beg == end)
        {
            isNumber = false;
        }
        for (int i = beg; i < end; i++)
        {
            if (str[i] != '.' && (str[i] < '0' || str[i] > '9'))
            {
                isNumber = false;
            }
        }

        if (isNumber)
        {
            return new JNumber(str, beg, end);
        }
        else if(strCmp(str,beg,end, "null"))
        {
            return new JNull();
        }
       else if(strCmp(str,beg,end, "false"))
        {
            return new JBool(false);
        }
        else if(strCmp(str,beg,end, "true"))
        {
            return new JBool(true);
        }
        else
        {
            return new JStr(str, beg, end);
        }
    }

    JObj* ParseJObj(JVal* parent, std::vector<Token*>& tokens, int& index, const char* str);
    
    JArr* ParseJArray(JVal* parent, std::vector<Token*>& tokens, int& index, const char* str)
    {
        JArr* array = new JArr(parent);
        bool valueSetted = false;

        int valueBeg = 0;
        int valueEnd = 0;
        for(int i = index; i < tokens.size(); i++)
        {
            if(tokens[i]->type == TokenType::ANGLE_BRACKET_CLOSE)
            {
                if(valueSetted)
                {
                    array->PushBack(GetValueTypeByString(str, valueBeg, valueEnd));
                    valueSetted = false;
                }
                index = i;
                return array;
            }

            if(tokens[i]->type == TokenType::CURLY_BRACKET_OPEN)
            {
                array->PushBack(ParseJObj(array, tokens, i, str));
            }
            else if(tokens[i]->type == TokenType::COMMA)
            {
                 if(valueSetted)
                 {
                    array->PushBack(GetValueTypeByString(str, valueBeg, valueEnd));
                    valueSetted = false;
                 }
            }
            else if(tokens[i]->type == TokenType::KEY)
            {
                StrToken* tkn = static_cast<StrToken*>(tokens[i]);
                valueBeg = tkn->beginIndex;
                valueEnd = tkn->endIndex;
                valueSetted = true;
            }
        }
        
        index;
        return array;
    }

    std::string GetStr(const char* str, int b, int e)
    {
        std::string s= "";
        s.append(&str[b], e- b);
        return s;
    }
    
    uint32_t GetStrHash(const char* str, int b, int e)
    {
        uint32_t hash = Oyun::hashlittle(&str[b], e-b, 0);
        auto it = stringHashes.begin();
        auto found = stringHashes.find(hash);
        if(found == stringHashes.end())
        {
            stringHashes.insert(std::pair<uint32_t, std::string>(hash, GetStr(str, b, e)));
        }
        return hash;
    }
    JObj* ParseJObj(JVal* parent, std::vector<Token*>& tokens, int& index, const char* str)
    {
        JObj* obj = new JObj(parent);


        int keyStart = 0;
        int keyEnd = 0;
        int valueStart = 0;
        int valueEnd = 0;
        bool setKey = true;
        
        for(int i = index; i < tokens.size(); i++)
        {
            if(tokens[i]->type == TokenType::KEY)
            {
                StrToken* tkn = static_cast<StrToken*>(tokens[i]);
                if(setKey)
                {
                    keyStart = tkn->beginIndex;
                    keyEnd = tkn->endIndex;
                }
                else
                {
                    valueStart = tkn->beginIndex;
                    valueEnd = tkn->endIndex;
                }
            }
            else if(tokens[i]->type == TokenType::COLON)
            {
                setKey = !setKey;
            }
            else if(tokens[i]->type == TokenType::COMMA)
            {
                obj->Add(std::pair<uint32_t, JVal*>(GetStrHash(str, keyStart, keyEnd), GetValueTypeByString(str, valueStart, valueEnd)));
                setKey = true;
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_OPEN)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetStrHash(str, keyStart, keyEnd), ParseJObj(obj, tokens, i, str)));
                }
            }
            else if(tokens[i]->type == TokenType::CURLY_BRACKET_CLOSE)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetStrHash(str, keyStart, keyEnd), GetValueTypeByString(str, valueStart, valueEnd)));
                }
                index = i;
                return obj;
            }
            else if(tokens[i]->type == TokenType::ANGLE_BRACKET_OPEN)
            {
                if(!setKey)
                {
                    obj->Add(std::pair<uint32_t, JVal*>(GetStrHash(str, keyStart, keyEnd), ParseJArray(obj, tokens, i, str)));
                }
            }
            
        }
        
        index;
        return obj;
    }
    

    JVal::JVal(JType t)
        : type(t)
    {
    }


    JVal::~JVal()
    {
        //std::cout<<"DELETED : " << ToString(); 
    }

    void JVal::operator=(const int& v)
    {
        jassert(type == JType::NUM, "integer type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const float& v)
    {
        jassert(type == JType::NUM, "float type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const double& v)
    {
        jassert(type == JType::NUM, "double type can only be assigned to JNum types");
        JNumber* num = dynamic_cast<JNumber*>(this);
        if (num != nullptr)
        {
            // num->str = std::to_string(v);
        }
    }

    void JVal::operator=(const bool v)
    {
        jassert(type == JType::BOOL, "bool type can only be assigned to JBool types");
        JBool* num = dynamic_cast<JBool*>(this);
        if (num != nullptr)
        {
            num->val = v;
        }
    }

    void JVal::operator=(const std::string& str)
    {
        jassert(type == JType::STR, "std::string type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if (num != nullptr)
        {
            // num->str = str;
        }
    }

    void JVal::operator=(const char* str)
    {
        jassert(type == JType::STR, "const char* type can only be assigned to JStr types");
        JStr* num = dynamic_cast<JStr*>(this);
        if (num != nullptr)
        {
            num->str = str;
        }
    }

    std::string JVal::ToString()
    {
        return "VAL";
    }

    JNull::JNull()
        :JVal(JType::NUL)
    {
    }

    JNull::~JNull()
    {
    }

    std::string JNull::ToString()
    {
        return "null";
    }


    JStr::JStr(const char* s, size_t b, size_t e)
        :JVal(JType::STR), str(s), begin(b), end(e)
    {
    }

    JStr::~JStr()
    {
    }

    std::string JStr::ToString()
    {
       return "\"" + GetStr(str, begin, end) + "\"";
    }

    JNumber::JNumber(const char* s, size_t b, size_t e)
        :JVal(JType::NUM), str(s), begin(b), end(e)
    {
    }

    // JNumber::JNumber(int v)
    //     : JVal(JType::NUM), str(std::to_string(v))
    // {
    // }
    //
    // JNumber::JNumber(float v)
    //     : JVal(JType::NUM), str(std::to_string(v))
    // {
    // }
    //
    // JNumber::JNumber(double d)
    //     : JVal(JType::NUM), str(std::to_string(d))
    // {
    // }

    JNumber::~JNumber()
    {
    }

    bool JNumber::IsInteger()
    {
        for (int i = begin; i < end; i++)
        {
            if (str[i] == '.')
            {
                return false;
            }
        }

        return true;
    }

    int JNumber::AsInt() const
    {
        return std::stoi(str);
    }

    float JNumber::AsFloat() const
    {
        return std::stof(str);
    }

    double JNumber::AsDouble() const
    {
        return std::stod(str);
    }

    std::string JNumber::ToString()
    {
        return GetStr(str, begin, end);
    }

    JBool::JBool(bool b)
        : JVal(JType::BOOL), val(b)
    {
    }

    JBool::~JBool()
    {
    }

    std::string JBool::ToString()
    {
        return (val ? "true" : "false");
    }

    JObj::JObj(JVal* parent)
        : JVal(JType::OBJ), parentObj(parent)
    {
    }

    JObj::~JObj()
    {
        for (auto m : objects)
        {
            //std::cout<<"[DELETE] : " << m.first<<std::endl;
            delete m.second;
        }
    }

    JObj::JObj(JObj&& objR) noexcept
        : JVal(JType::OBJ), parentObj((objR.parentObj)), objects(std::move(objR.objects))
    {
        objR.parentObj = nullptr;
        objR.objects.clear();
    }


    JObj::JObj(std::initializer_list<std::pair<uint32_t, JVal*>> list)
        : JVal(JType::OBJ), parentObj(nullptr)
    {
        auto it = list.begin();
        while (it != list.end())
        {
            Add(*it);
            it++;
        }
    }

    void JObj::Add(std::pair<uint32_t, JVal*> add)
    {
        objects.insert(add);
    }

    void JObj::Add(std::pair<uint32_t, JArr*> add)
    {
        objects.insert(add);
        add.second->parentObj = this;
    }

    void JObj::Add(std::pair<uint32_t, JObj*> add)
    {
        objects.insert(add);
        add.second->parentObj = this;
    }

    JVal* JObj::Get(uint32_t hash)
    {
        auto found = objects.find(hash);
        if (found != objects.end())
        {
            std::cout << "Succesfully Found!" << std::endl;
            return found->second;
        }


        return nullptr;
    }

    JObj* JObj::GetObj(const std::string& key)
    {
        JVal* val = Get(Oyun::hashlittle(key.c_str(), key.length(), 0));
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::OBJ, "Given key is not an JObj*");

        return dynamic_cast<JObj*>(val);
    }

    JArr* JObj::GetArr(const std::string& key)
    {
        JVal* val = Get(Oyun::hashlittle(key.c_str(), key.length(), 0));
        assert(val != nullptr, "Given key is exist");
        assert(val->type == JType::ARR, "Given key is not an JArr*");

        return dynamic_cast<JArr*>(val);
    }

    JVal& JObj::operator[](const std::string& str)
    {
        JVal* val = Get(Oyun::hashlittle(str.c_str(), str.length(), 0));
        jassert(val!=nullptr, "given key is not exist");
        return *val;
    }


    void JArr::PushBack(JVal* val)
    {
        arr.push_back(val);
    }

    void JArr::PushBack(const char* str, int b, int e)
    {
        arr.push_back(new JStr(str, b, e));
    }


    JArr::JArr(JVal* parent)
        : JVal(JType::ARR), parentObj(nullptr)
    {
    }

    JArr::~JArr()
    {
        //std::cout<<"Delete Array";
        for (auto m : arr)
        {
            //std::cout<<"[DELETE ARR ELEMENT] : " << m->ToString()<<std::endl;
            delete m;
        }
    }

    JArr::JArr(JArr&& ar) noexcept
        : JVal(JType::ARR), parentObj(ar.parentObj), arr(std::move(ar.arr))
    {
        ar.parentObj = nullptr;
        ar.arr.clear();
    }

    void JArr::Add(JObj* obj)
    {
        arr.push_back(obj);
    }

    // void JArr::Add(double value)
    // {
    //     arr.push_back(new JNumber(value));
    // }
    //
    // void JArr::Add(int value)
    // {
    //     arr.push_back(new JNumber(value));
    // }
    //
    // void JArr::Add(float value)
    // {
    //     arr.push_back(new JNumber(value));
    // }

    void JArr::Add(const char* str, int b, int e)
    {
        arr.push_back(new JStr(str, b, e));
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
        int index = 0;
        JArr* arr = ParseJArray(nullptr, tokens, index, str);
        //PrintArray(arr, 0);

        for (auto token : tokens)
        {
            if(token->type == TokenType::KEY)
            {
                delete token;
            }
        }


        {
            fstream stream;
            stream.open("C:\\Development\\Jsones\\test.json", std::ifstream::out | std::ifstream::binary);
            stream<<JWrite(arr, true).rdbuf();
            stream.close();
        }
        return nullptr;
    }

    std::string beautifiedTab("    ");
    std::string beautifiedNewLine("\n");
    std::string beautifiedSpace(" ");
    std::string emptyString("");

    std::string& NewLine(bool beautify)
    {
        if (beautify)
        {
            return beautifiedNewLine;
        }
        return emptyString;
    }

    std::string& Tab(bool beautify)
    {
        if (beautify)
        {
            return beautifiedTab;
        }
        return emptyString;
    }

    std::string& Space(bool beautify)
    {
        if (beautify)
        {
            return beautifiedSpace;
        }
        return emptyString;
    }

    std::string Tab(bool beautify, int count)
    {
        if (beautify)
        {
            std::string str;
            for (int i = 0; i < count; i++)
            {
                str += beautifiedTab;
            }
            return str;
        }
        return emptyString;
    }

    std::stringstream JWrite(JObj* obj, bool beautify, int tab);
    std::stringstream JWrite(JArr* arr, bool beautify, int tab)
    {
         std::stringstream ss;
        ss << NewLine(beautify)<< Tab(beautify, tab);
        ss << "[" << NewLine(beautify);

        bool subFirst = true;
        for (auto it : arr->arr)
        {
            if (!subFirst)
            {
                ss << "," << NewLine(beautify);
            }
            subFirst = false;
            if (it->type == JType::OBJ)
            {
                ss << JWrite(dynamic_cast<JObj*>(it), beautify, tab + 1).rdbuf();
            }
            else if(it->type == JType::ARR)
            {
                ss<<JWrite(dynamic_cast<JArr*>(it), true, tab+1).rdbuf();
            }
            else
            {
                ss<<it->ToString();
            }
                      
        }
        ss << NewLine(beautify) << Tab(beautify, tab) << "]";
        return ss;
    }
    
    std::stringstream JWrite(JObj* obj, bool beautify, int tab)
    {
         std::stringstream ss;
        ss <<  Tab(beautify, tab);
        ss << "{" << NewLine(beautify);

        bool subFirst = true;
        for (auto it : obj->objects)
        {
            if (!subFirst)
            {
                ss << "," << NewLine(beautify);
            }
            ss<<Tab(beautify, tab+1);

            subFirst = false;
            ss<<"\""<< PrintKey(it.first) <<"\"" << Space(beautify) << ":" << Space(beautify);

            if (it.second->type == JType::OBJ)
            {
                ss << JWrite(dynamic_cast<JObj*>(it.second), beautify, tab + 1).rdbuf();
            }
            else if(it.second->type == JType::ARR)
            {
                ss<<JWrite(dynamic_cast<JArr*>(it.second), true, tab+1).rdbuf();
            }
            else
            {
                ss<<it.second->ToString();
            }
                      
        }
        ss << NewLine(beautify) << Tab(beautify, tab) << "}";
        return ss;
    }
    
}
