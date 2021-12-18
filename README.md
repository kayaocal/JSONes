# JSONes

* Just another small json library. it is just a parser and writer. No reflection or fancy specs.
* It is tested with examples at json.org.
* *null* type is not supported
* No dependency. Only standart library functions.
* Cpp11

## Use
* Just copy Jsones.h and Jsones.cpp to your project.

## Compile
* Cmake can be used to generate project files


## How to parse Json

```c++
using namespace Jsones;
JObj* parsedObj = JParse(s.c_str());
```

## How to write Json

```c++
using namespace Jsones;
 JObj* obj = new JObj{
        JValue("integerTest", 9),
        JValue("boolTest", false),
        JValue("floatTest", 3.5f),
        JValue("doubleTest", 3.5),
        JValue("strTest", std::string("This is a test str")),
        JValue("constCharTest", "This is a  const char* str"),
        JValue("childObj", new JObj{
                JValue("child", "childStr"),
                JValue("childBool", false),
                JValue("childInt", 339),
                JValue("childArr", new JArr{3,6,9,12,15,18,21,24,27,30})
            }),
        JValue("intArr", new JArr{ 5,2,3,4,5 }),
        JValue("boolArr", new JArr{true, false,true,false}),
        JValue("strArr", new JArr{ "hello", "world", "initalizer", "list" })
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
