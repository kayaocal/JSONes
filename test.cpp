#include "Jsones.h"
#include <iostream>

void CreateJsonTest()
{
    using namespace Jsones;

    JObj h = {JPair("tst", "tst")};
    
    
    JObj obj = {
        JPair("integerTest", 9),
        JPair("boolTest", false),
        JPair("floatTest", 3.5f),
        JPair("doubleTest", 3.5),
        JPair("strTest", std::string("This is a test str")),
        JPair("constCharTest", "This is a  const char* str"),
        JPair("hObj", std::move(h)),
        JPair("childObj", JObj{
                JPair("child", "childStr"),
                JPair("childBool", false),
                JPair("childInt", 339),
                JPair("childArr", JArray({3,6,9,12,15,18,21,24,27,30}))
            }),
        
        JPair("intArr", new JArr{ 5,2,3,4,5 }),
        JPair("boolArr", new JArr{true, false,true,false}),
        JPair("strArr", JArray({ "hello", "world", "initalizer", "list" }))
        };
    


    JArr arrRef={3,5,8,12,5};
    arrRef[2] = 1222;
    std::cout<<"htest  : " << JWrite(&h).rdbuf();
    std::cout<<"RefTest : " << JWrite(&arrRef).rdbuf();
    //((obj->GetObj("childObj")->GetArr("childArr"))[0]) = 888;
        obj["integerTest"] = 5155;

    std::cout<<std::endl << "JSON : " << JWrite(&obj, true).rdbuf();
    std::string str = JWrite(&obj).str();

     std::cout<<std::endl<<"----------------"<<std::endl;
    JObj* parsedObj = JParse(str.c_str());
    PrintJson(parsedObj);

}



int main()
{
    using namespace std;
     CreateJsonTest();
    //const char* js = "  {\"glossary\":{ \"bool\": true   ,\"int\":101,\"float\":100.5,\"title\": \"Bu bir test\",\"GlossDiv\":{\"title\":\"S\",\"GlossList\":{\"GlossEntry\":{\"ID\":\"SGML\",\"SortAs\":\"SGML\",\"GlossTerm\":\"Standard Generalized Markup Language\",\"Acronym\":\"SGML\",\"Abbrev\":\"ISO 8879:1986\",\"GlossDef\":{\"para\":\"A meta-markup language, used to create markup languages such as DocBook.\",\"GlossSeeAlso\": [\"GML\", \"XML\"]},\"GlossSee\":\"markup\"}}}, \"test\":\"Bu bir testtir\"}, \"Iki\":\" uc\"""}";
    //const char* js = "[{\"test\":15,\"bool\":false,\"str\":\"strtest\"},{\"test\":18,\"bool\":true,\"str\":\"strtes2t\"}]";
    //const char* js = "[15,25,158,11,66,12]";
   /* const char* js = "{\"widget\": {\
    \"debug\": \"on\",\
    \"window\": {\
        \"title\": \"Sample Konfabulator Widget\",\
        \"name\": \"main_window\",\
        \"width\": 500,\
        \"height\": 500\
    },\
    \"image\": { \
        \"src\": \"Images/Sun.png\",\
        \"name\": \"sun1\",\
        \"hOffset\": 250,\
        \"vOffset\": 250,\
        \"alignment\": \"center\"\
    },\
    \"text\": {\
        \"data\": \"Click Here\",\
        \"size\": 36,\
        \"style\": \"bold\",\
        \"name\": \"text1\",\
        \"hOffset\": 250,\
        \"vOffset\": 100,\
        \"alignment\": \"center\",\
        \"onMouseUp\": \"sun1.opacity = (sun1.opacity / 100) * 90;\"\
    }\
}} ";*/

    /*const char* js = "{\"web-app\": {\
  \"servlet\": [   \
    {\
      \"servlet-name\": \"cofaxCDS\",\
      \"servlet-class\": \"org.cofax.cds.CDSServlet\",\
      \"init-param\": {\
        \"configGlossary:installationAt\": \"Philadelphia, PA\",\
        \"configGlossary:adminEmail\": \"ksm@pobox.com\",\
        \"configGlossary:poweredBy\": \"Cofax\",\
        \"configGlossary:poweredByIcon\": \"/images/cofax.gif\",\
        \"configGlossary:staticPath\": \"/content/static\",\
        \"templateProcessorClass\": \"org.cofax.WysiwygTemplate\",\
        \"templateLoaderClass\": \"org.cofax.FilesTemplateLoader\",\
        \"templatePath\": \"templates\",\
        \"templateOverridePath\": \"\",\
        \"defaultListTemplate\": \"listTemplate.htm\",\
        \"defaultFileTemplate\": \"articleTemplate.htm\",\
        \"useJSP\": false,\
        \"jspListTemplate\": \"listTemplate.jsp\",\
        \"jspFileTemplate\": \"articleTemplate.jsp\",\
        \"cachePackageTagsTrack\": 200,\
        \"cachePackageTagsStore\": 200,\
        \"cachePackageTagsRefresh\": 60,\
        \"cacheTemplatesTrack\": 100,\
        \"cacheTemplatesStore\": 50,\
        \"cacheTemplatesRefresh\": 15,\
        \"cachePagesTrack\": 200,\
        \"cachePagesStore\": 100,\
        \"cachePagesRefresh\": 10,\
        \"cachePagesDirtyRead\": 10,\
        \"searchEngineListTemplate\": \"forSearchEnginesList.htm\",\
        \"searchEngineFileTemplate\": \"forSearchEngines.htm\",\
        \"searchEngineRobotsDb\": \"WEB-INF/robots.db\",\
        \"useDataStore\": true,\
        \"dataStoreClass\": \"org.cofax.SqlDataStore\",\
        \"redirectionClass\": \"org.cofax.SqlRedirection\",\
        \"dataStoreName\": \"cofax\",\
        \"dataStoreDriver\": \"com.microsoft.jdbc.sqlserver.SQLServerDriver\",\
        \"dataStoreUrl\": \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\",\
        \"dataStoreUser\": \"sa\",\
        \"dataStorePassword\": \"dataStoreTestQuery\",\
        \"dataStoreTestQuery\": \"SET NOCOUNT ON;select test='test';\",\
        \"dataStoreLogFile\": \"/usr/local/tomcat/logs/datastore.log\",\
        \"dataStoreInitConns\": 10,\
        \"dataStoreMaxConns\": 100,\
        \"dataStoreConnUsageLimit\": 100,\
        \"dataStoreLogLevel\": \"debug\",\
        \"maxUrlLength\": 500}},\
    {\
      \"servlet-name\": \"cofaxEmail\",\
      \"servlet-class\": \"org.cofax.cds.EmailServlet\",\
      \"init-param\": {\
      \"mailHost\": \"mail1\",\
      \"mailHostOverride\": \"mail2\"}},\
    {\
      \"servlet-name\": \"cofaxAdmin\",\
      \"servlet-class\": \"org.cofax.cds.AdminServlet\"},\
 \
    {\
      \"servlet-name\": \"fileServlet\",\
      \"servlet-class\": \"org.cofax.cds.FileServlet\"},\
    {\
      \"servlet-name\": \"cofaxTools\",\
      \"servlet-class\": \"org.cofax.cms.CofaxToolsServlet\",\
      \"init-param\": {\
        \"templatePath\": \"toolstemplates/\",\
        \"log\": 1,\
        \"logLocation\": \"/usr/local/tomcat/logs/CofaxTools.log\",\
        \"logMaxSize\": \"\",\
        \"dataLog\": 1,\
        \"dataLogLocation\": \"/usr/local/tomcat/logs/dataLog.log\",\
        \"dataLogMaxSize\": \"\",\
        \"removePageCache\": \"/content/admin/remove?cache=pages&id=\",\
        \"removeTemplateCache\": \"/content/admin/remove?cache=templates&id=\",\
        \"fileTransferFolder\": \"/usr/local/tomcat/webapps/content/fileTransferFolder\",\
        \"lookInContext\": 1,\
        \"adminGroupID\": 4,\
        \"betaServer\": true}}],\
  \"servlet-mapping\": {\
    \"cofaxCDS\": \"/\",\
    \"cofaxEmail\": \"/cofaxutil/aemail/*\",\
    \"cofaxAdmin\": \"/admin/*\",\
    \"fileServlet\": \"/static/*\",\
    \"cofaxTools\": \"/tools/*\"},\
 \
  \"taglib\": {\
    \"taglib-uri\": \"cofax.tld\",\
    \"taglib-location\": \"/WEB-INF/tlds/cofax.tld\"}}}";
    */

  //  JObj* root = JParse(js);
    //delete root;


   
    
}
