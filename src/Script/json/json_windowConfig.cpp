/*
 * =================== json_windowConfig.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.08.08
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <string>

//--------------- Libs ------------------//
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "tprGeneral.h"

//-------------------- Engine --------------------//
#include "tprAssert.h"
#include "global.h"
#include "fileIO.h"

#include "esrc_state.h"

//--------------- Script ------------------//
#include "Script/json/json_all.h"
#include "Script/json/json_oth.h"

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;


namespace json{//------------- namespace json ----------------


/* ===========================================================
 *                parse_from_windowConfigJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
WindowConfigJsonData parse_from_windowConfigJsonFile(){

    cout << "   ----- parse_from_windowConfigJsonFile: start ----- " << endl;

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file = tprGeneral::path_combine(path_jsons, "windowConfig.json");
    auto jsonBufUPtr = read_a_file( path_file );

    //-----------------------------//
    //      parce JSON data
    //-----------------------------//
    WindowConfigJsonData windowConfig {}; // 这组数据很小，直接值传递

    Document doc;
    doc.Parse( jsonBufUPtr->c_str() );

    tprAssert( doc.IsObject() );
    {//--- windowPixW ---//
        const auto &a = check_and_get_value( doc, "windowPixW", JsonValType::Int );
        windowConfig.windowPixW = a.GetInt();
    }
    {//--- windowPixH ---//
        const auto &a = check_and_get_value( doc, "windowPixH", JsonValType::Int );
        windowConfig.windowPixH = a.GetInt();
    }
    {//--- isFullScreen ---//
        const auto &a = check_and_get_value( doc, "isFullScreen", JsonValType::Bool );
        windowConfig.isFullScreen = a.GetBool();
    }

    cout << "   ----- parse_from_windowConfigJsonFile: end ----- " << endl;

    esrc::insertState("json_windowConfig");
    return windowConfig;
}


}//------------- namespace json: end ----------------