/*
 * ====================== json_UIGo.cpp =====================
 *                          -- tpr --
 *                                        CREATE -- 2019.07.02
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
//--------------- CPP ------------------//
#include <unordered_map>
#include <string>
#include <utility>

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
#include "Script/resource/ssrc.h" 

using namespace rapidjson;

#include <iostream>
using std::cout;
using std::endl;

namespace json{//------------- namespace json ----------------
namespace uiJson_inn {//-------- namespace: uiJson_inn --------------//

    const std::vector<std::string> lpath_files {
        "/uiGos/uiGos.json"
    };


}//------------- namespace: uiJson_inn end --------------//


/* ===========================================================
 *                parse_from_uiGoJsonFile
 * -----------------------------------------------------------
 * Do Not Worry About Performance !!!
 */
void parse_from_uiGoJsonFile(){

    cout << "   ----- parse_from_uiGoJsonFile: start ----- " << endl;

    //-----------------------------//
    //         load file
    //-----------------------------//
    std::string path_file {};
    for( const auto &lpath : uiJson_inn::lpath_files ){
        path_file = tprGeneral::path_combine(path_jsons, lpath);
        auto jsonBufUPtr = read_a_file( path_file );

        //-----------------------------//
        //      parce JSON data
        //-----------------------------//
        Document doc;
        doc.Parse( jsonBufUPtr->c_str() );

        tprAssert( doc.IsArray() );
        for( auto &ent : doc.GetArray() ){

            UIGoJsonData  uiGoJsonData {};

            {//--- gameObjType ---//
                const auto &a = check_and_get_value( ent, "gameObjType", JsonValType::String );
                uiGoJsonData.gameObjType = a.GetString();
            }
            {//--- specID ---//
                const auto &a = check_and_get_value( ent, "specID", JsonValType::Uint );
                uiGoJsonData.specID = static_cast<goSpecId_t>( a.GetUint() );
            }
            {//--- parentID ---//
                const auto &a = check_and_get_value( ent, "parentID", JsonValType::Uint64 );
                uiGoJsonData.parentID = static_cast<goid_t>( a.GetUint64() );
            }
            {//--- family ---//
                const auto &a = check_and_get_value( ent, "family", JsonValType::String );
                uiGoJsonData.family = str_2_GameObjFamily( a.GetString() );
            }
            {//--- moveType ---//
                const auto &a = check_and_get_value( ent, "moveType", JsonValType::String );
                uiGoJsonData.moveType = str_2_MoveType( a.GetString() );
            }
            {//--- speedLvl ---//
                const auto &a = check_and_get_value( ent, "speedLvl", JsonValType::Int );
                uiGoJsonData.speedLvl = int_2_SpeedLevel( a.GetInt() );
            }

            //====================================//
            //  Now, the uiGoJsonData is inited
            //  insert it into all ssrc::containers
            //------------------------------------//
            ssrc::insert_2_uiGo_specId_names_containers( uiGoJsonData.specID, uiGoJsonData.gameObjType );
            ssrc::insert_2_uiGo_jsonDatas( uiGoJsonData );

        }

    }

    esrc::insertState("json_uiGo");
    cout << "   ----- parse_from_uiGoJsonFile: end ----- " << endl;
}


/* ===========================================================
 *               assemble_uiGoJsonData_2_newUIGo    
 * -----------------------------------------------------------
 */
void assemble_uiGoJsonData_2_newUIGo( goSpecId_t specID_, 
                                    GameObj &goRef_ ){

    const auto &d = ssrc::get_uiGoJsonData( specID_ );

    goRef_.species   = d.specID;
    goRef_.parentId  = d.parentID;
    goRef_.family    = d.family;

    goRef_.move.set_MoveType( d.moveType );
    goRef_.move.set_speedLvl( d.speedLvl );
   

    // 对于 uiGo 来说，很多 go成员 都不需要
    // 直接使用 默认值...

    // 有些则需要手动设置
    //...
    goRef_.isMoveCollide = false;

    //------ default -------//
    goRef_.isDirty = false;
    goRef_.isControlByPlayer = false;
    //...
    //------ tmp -------//
    goRef_.isActive = true; //- tmp. 是否进入激活圈，应该由 mpos 计算出来 未实现
    //...
}



}//------------- namespace json: end ----------------
