#include "Jsones.h"
#include <iostream>

void CreateJsonTest()
{
    using namespace Jsones;

    // JObj h = {JPair("tst", "tst")};
    //
    //
    // JObj obj = {
    //     JPair("integerTest", 9),
    //     JPair("boolTest", false),
    //     JPair("floatTest", 3.5f),
    //     JPair("doubleTest", 330.65),
    //     JPair("strTest", std::string("This is a test str")),
    //     JPair("constCharTest", "This is a  const char* str"),
    //     JPair("hObj", std::move(h)),
    //     JPair("childObj", JObj{
    //             JPair("child", "childStr"),
    //             JPair("childBool", false),
    //             JPair("childInt", 339),
    //             JPair("childArr", JArr{93,6,9,12,15,18,21,24,27,30})
    //         }),
    //     
    //     JPair("intArr", JArr{ 5,2,3,4,5 }),
    //     JPair("boolArr", JArr{true, false,true,false}),
    //     JPair("strArr", JArr({ "hello", "world", "initalizer", "list" }))
    //     };
    //

    //
    // JArr arrRef={3,5,8,12,5};
    // arrRef[2] = 1222;
    // std::cout<<"htest  : " << JWrite(&h).rdbuf();
    // std::cout<<"RefTest : " << JWrite(&arrRef).rdbuf();
    // //((obj->GetObj("childObj")->GetArr("childArr"))[0]) = 888;
    //     obj["integerTest"] = 5155;
    //
    // std::cout<<std::endl << "JSON : " << JWrite(&obj, true).rdbuf();
    // std::string str = JWrite(&obj).str();
    //
    //  std::cout<<std::endl<<"----------------"<<std::endl;
    // JObj* parsedObj = JParse(str.c_str());
    // PrintJson(parsedObj);

}

#include <fstream>
void ReadArsenal()
{

    
    using namespace Jsones;
    std::basic_ifstream<char> stream;
    stream.open("C:\\Development\\Jsones\\arsenal.json", std::ifstream::in | std::ifstream::binary);
    if(stream.is_open())
	{
        std::string text, line;
        while(std::getline(stream, line))
       {
           text += line + "\n";
       }
       const char* data = text.c_str();
        
		JParse(data);
        std::cout<<"Bitti";
	}
}


int main()
{
    std::cout<<"Basladik"<<std::endl;
    using namespace Jsones;

    JObj obj{
        JPair("Hello", "world"),
        JPair("fucking", "fuck"),
        JPair("intArr12", JArr{true,false}),
        JPair("SubObject", JObj{JPair("subObjName", "subObject"), JPair("subObjInt", 112)})
    };

    JArr arr = JArr();
    
    arr.Add(obj)
    .Add(obj)
    .Add(JObj{JPair("test", "testStr")});

    PrintArray(&arr, 0);

    
    return 0;
}