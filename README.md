# JSONes

* Just another small json parser and writer. It has no reflection or fancy specs.
* It is tested with examples at json.org
* **This library does not validate json yet!**
* **null** type is not supported
* Only standart library. No any other 3rd party library required.
* Written in cpp 11.

## Use
* Just copy Jsones.h and Jsones.cpp to your project.

## Compile
* Cmake can be used to generate project files

## Structure
```c++
enum class JSONES_API JType{ OBJ = 1, NUM = 2, STR = 3, BOOL = 4, ARR = 5 };
//base struct.
struct JSONES_API JVal{
    JType type;
    ...
};

//struct for string value. Also type is JType::STR
struct JSONES_API JStr : public JVal{...};

//struct for number value as integer, float double. Also type is JType::NUM
struct JSONES_API JNumber : public JVal{...}

//struct for boolean value. Also type is JType::BOOL
struct JSONES_API JBool : public JVal{...}

//struct for json objects. Also type is JType::OBJ
struct JSONES_API JObj : public  JVal
{
    std::map<std::string, JVal*> objects;
    ...
}

//struct for json arrays. Also type is JType::ARR
struct JSONES_API JArr : public JObj{
    std::vector<JVal*> arr;
    ...
}
```

## How to parse Json

```c++
using namespace Jsones;
JObj* parsedObj = JParse(s.c_str());
```
## Delete Json
```c++
using namespace Jsones;

JObj* parsedObj = JParse(s.c_str());
//It deletes all child values, objects and arrays.
delete parsedObj;

//or
JObj obj = JObject({JPair("integerTest", 9)}
//It deletes all child values, objects and arrays.
delete &obj;
```

## How to change an object key's value
```c++
using namespace Jsones;
JObj* parsedObj = JParse(s.c_str());

//assertion if key is not exist or given value is not match with JVal type. 
//E.g: if value is bool, you can only set it to JBool or if value is integer, 
//then it can only be set to JNumber
parsedObj->Set("key", value);

//or...
//JObject returns JObj Refrence, You should delete JObj to prevent memory leak.
JObj& obj = JObject({JPair("integerTest", 9)}
obj["integerTest"] = 5155;
```

## How to change an array's nth element's value
```c++
using namespace Jsones;
//Create array
JArr arrRef = {3,5,8,12,5};
//Change it's element
arrRef[2] = 1222;

//or...

JArr* arr = new JArr{3, 4, 5, 6, 7, 8, 9};
(*arr)[3] = 444;

```

## How to write Json

```c++
using namespace Jsones;
 JObj* obj = new JObj{
        JPair("integerTest", 9),
        JPair("boolTest", false),
        JPair("floatTest", 3.5f),
        JPair("doubleTest", 3.5),
        JPair("strTest", std::string("This is a test str")),
        JPair("constCharTest", "This is a  const char* str"),
        JPair("childObj", new JObj{
                JPair("child", "childStr"),
                JPair("childBool", false),
                JPair("childInt", 339),
                JPair("childArr", new JArr{3,6,9,12,15,18,21,24,27,30})
            }),
        JPair("intArr", new JArr{ 5,2,3,4,5 }),
        JPair("boolArr", new JArr{true, false,true,false}),
        JPair("strArr", new JArr{ "hello", "world", "initalizer", "list" })
    };
std::string jsonOutput = JWrite(obj); //0 as tab(spacing) level and true for beautfy
```
Result of jsonOutput will be;
```json

{"boolArr":[true,false,true,false],"boolTest":false,"childObj":{"child":"childStr","childArr":[3,6,9,12,15,18,21,24,27,30],"childBool":false,"childInt":339},"constCharTest":"This is a  const char* str","doubleTest":3.500000,"floatTest":3.500000,"intArr":[5,2,3,4,5],"integerTest":9,"strArr":["hello","world","initalizer","list"],"strTest":"This is a test str"}
```
Also you can beautify json as following code
```c++
std::string jsonOutput = JWrite(obj, true); //true for beautfy
```
```json
{
        "boolArr" :
        [
                 true,
                 false,
                 true,
                 false
        ],
        "boolTest" : false,
        "childObj" :
                {
                        "child" : "childStr",
                        "childArr" :
                        [
                                 3,
                                 6,
                                 9,
                                 12,
                                 15,
                                 18,
                                 21,
                                 24,
                                 27,
                                 30
                        ],
                        "childBool" : false,
                        "childInt" : 339
                },
        "constCharTest" : "This is a  const char* str",
        "doubleTest" : 3.500000,
        "floatTest" : 3.500000,
        "intArr" :
        [
                 5,
                 2,
                 3,
                 4,
                 5
        ],
        "integerTest" : 9,
        "strArr" :
        [
                 "hello",
                 "world",
                 "initalizer",
                 "list"
        ],
        "strTest" : "This is a test str"
}
```
