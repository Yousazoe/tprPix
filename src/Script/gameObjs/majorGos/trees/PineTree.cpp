/*
 * ========================= PineTree.cpp ==========================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#include "Script/gameObjs/majorGos/trees/PineTree.h"

//-------------------- CPP --------------------//
#include <functional>
#include <string>

//-------------------- Engine --------------------//
#include "Density.h"
#include "animSubspeciesId.h"
#include "dyParams.h"
#include "GoSpecFromJson.h"

#include "tprAssert.h"
#include "esrc_shader.h" 
#include "esrc_gameSeed.h"
#include "esrc_animFrameSet.h"

//-------------------- Script --------------------//


using namespace std::placeholders;

#include "tprDebug.h" 


namespace gameObjs {//------------- namespace gameObjs ----------------


//namespace pineTree_inn {//----------- namespace: pineTree_inn ----------------//
    //inline std::uniform_int_distribution<int>     uDistribution_int(      3,   50 );
    //inline std::uniform_real_distribution<double> uDistribution_double( -1.3, 1.3 );
//}//-------------- namespace: pineTree_inn end ----------------//


struct PineTree_PvtBinary{
    int        tmp {};
};



void PineTree::init(GameObj &goRef_, const DyParam &dyParams_ ){

    //================ go.pvtBinary =================//
    auto *pvtBp = goRef_.init_pvtBinary<PineTree_PvtBinary>();

    //================ dyParams =================//
    size_t typeHash = dyParams_.get_typeHash();
    tprAssert( typeHash == typeid(DyParams_Blueprint).hash_code() );
    const DyParams_Blueprint *bpParamPtr = dyParams_.get_binaryPtr<DyParams_Blueprint>();
    const GoDataForCreate *goDataPtr = bpParamPtr->goDataPtr;

    //tprAssert( !goDataPtr->isMultiGoMesh ); // must single gomesh
    //const GoDataEntForCreate &goDataEntRef = *(*goDataPtr->goMeshDataUPtrs.cbegin());


    //-- set lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    goRef_.set_pos_lAltiRange( goSpecFromJsonRef.get_lAltiRange( goDataPtr->goAltiRangeLabel ) );

    //----- must before creat_new_goMesh() !!! -----//
    goRef_.actionDirection.reset( goDataPtr->direction );

    if( auto retOpt = goDataPtr->get_brokenLvl(); retOpt.has_value() ){
        goRef_.brokenLvl.reset( retOpt.value() );
    }else{
        tprAssert(0);
    }


    /*
    //-- 重新设置 lAltiRange ---
    const GoSpecFromJson &goSpecFromJsonRef = GoSpecFromJson::get_goSpecFromJsonRef( goDataPtr->goSpeciesId );
    GoAltiRange lAltiRange {};
    switch (goDataEntRef.animLabel){
        case AnimLabel::Mid: lAltiRange = goSpecFromJsonRef.get_lAltiRange( GoAltiRangeLabel::Mid ); break;
        case AnimLabel::Sml: lAltiRange = goSpecFromJsonRef.get_lAltiRange( GoAltiRangeLabel::Sml ); break;
        default:
            tprAssert(0);
            break;
    }
    goRef_.set_pos_lAltiRange( lAltiRange );
    */



    //================ animFrameSet／animFrameIdxHandle/ goMesh =================//


    for( const auto &uptrRef : goDataPtr->goMeshDataUPtrs ){
        const GoDataEntForCreate &goDataEntRef = *uptrRef;
        auto &goMeshRef = goRef_.creat_new_goMesh( 
                                goDataEntRef.goMeshName,
                                goDataEntRef.subspeciesId,
                                goDataEntRef.animActionEName,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                goDataEntRef.zOff,  //- zOff
                                true //- isVisible
                                );
    } 


        /*
        //-- 制作唯一的 mesh 实例: "root" --
        goRef_.creat_new_goMesh("root", //- gmesh-name
                                goDataEntRef.subspeciesId,
                                AnimActionEName::Idle,
                                RenderLayerType::MajorGoes, //- 不设置 固定zOff值
                                &esrc::get_shaderRef(ShaderType::UnifiedColor),  // pic shader
                                goDataEntRef.dposOff, //- pposoff
                                0.0,  //- zOff
                                true //- isVisible
                                );
        */

        
    //================ bind callback funcs =================//
    //-- 故意将 首参数this 绑定到 保留类实例 dog_a 身上
    goRef_.RenderUpdate = std::bind( &PineTree::OnRenderUpdate,  _1 );   
    goRef_.LogicUpdate  = std::bind( &PineTree::OnLogicUpdate,   _1 );

    //-------- actionSwitch ---------//
    goRef_.actionSwitch.bind_func( std::bind( &PineTree::OnActionSwitch,  _1, _2 ) );
    goRef_.actionSwitch.signUp( ActionSwitchType::Idle );


    //================ go self vals =================//   

}

/* -- 在 “工厂”模式中，将本具象go实例，与 一个已经存在的 go实例 绑定。
 * -- 这个 go实例 的类型，应该和 本类一致。
 */
void PineTree::bind( GameObj &goRef_ ){
}

/* -- 从硬盘读取到 go实例数据后，重bind callback
 * -- 会被 脚本层的一个 巨型分配函数 调用
 */
void PineTree::rebind( GameObj &goRef_ ){
}


void PineTree::OnRenderUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    //auto *pvtBp = goRef_.get_pvtBinaryPtr<PineTree_PvtBinary>();

    //=====================================//
    //            AI
    //-------------------------------------//
    //...

    //=====================================//
    //         更新 位移系统
    //-------------------------------------//
    //goRef_.move.RenderUpdate();

    //=====================================//
    //  将 确认要渲染的 goMeshs，添加到 renderPool         
    //-------------------------------------//
    goRef_.render_all_goMesh();
}


void PineTree::OnLogicUpdate( GameObj &goRef_ ){
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PineTree_PvtBinary>();
    //=====================================//

    // 什么也没做...
}



void PineTree::OnActionSwitch( GameObj &goRef_, ActionSwitchType type_ ){

        cout << "PineTree::OnActionSwitch" << endl;
    //=====================================//
    //            ptr rebind
    //-------------------------------------//
    auto *pvtBp = goRef_.get_pvtBinaryPtr<PineTree_PvtBinary>();
    //=====================================//

    auto dir = goRef_.actionDirection.get_newVal();
    auto brokenLvl = goRef_.brokenLvl.get_newVal();

    //-- 获得所有 goMesh 的访问权 --
    GameObjMesh &goMeshRef = goRef_.get_goMeshRef("root");

    //-- 处理不同的 actionSwitch 分支 --
    switch( type_ ){
        case ActionSwitchType::Idle:
            goMeshRef.set_animActionEName( AnimActionEName::Idle );
            goMeshRef.bind_animAction();
            break;

        default:
            break;
            tprAssert(0);
    }

    //goRef_.rebind_rootAnimActionPosPtr(); //- 临时性的方案 ...


}


}//------------- namespace gameObjs: end ----------------

