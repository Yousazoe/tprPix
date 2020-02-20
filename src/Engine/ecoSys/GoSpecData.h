/*
 * ========================= GoSpecData.h =======================
 *                          -- tpr --
 *                                        CREATE -- 2019.09.13
 *                                        MODIFY -- 
 * ----------------------------------------------------------
 */
#ifndef TPR_GO_SPEC_DATA_H
#define TPR_GO_SPEC_DATA_H

//-------------------- CPP --------------------//
#include <vector>

//------------------- Engine --------------------//
#include "AnimLabel.h"
#include "GameObjType.h"

#include "GoAssembleData.h"


//--------------- Script ------------------//


//-- 仅用于 ecoObj -> create a go 

        //  这个名字太糟糕了，应该被修改

        //  在未来，它可能跟随 整个旧版 mp-go 分配系统 被取消


class GoSpecData{
public:
    GoSpecData( goSpeciesId_t rootGoSpeciesId_,
                bool       isMultiGoMesh_,
                const std::string &afsName_,
                AnimLabel   animLabel_):
        rootGoSpeciesId(rootGoSpeciesId_),
        isMultiGoMesh(isMultiGoMesh_),
        afsName(afsName_),
        animLabel(animLabel_)
        {
            tprAssert( !isMultiGoMesh_ );
        }

    GoSpecData( goSpeciesId_t rootGoSpeciesId_,
                bool       isMultiGoMesh_,
                const std::string &afsName_,
                goLabelId_t multiGoMeshType_):
        rootGoSpeciesId(rootGoSpeciesId_),
        isMultiGoMesh(isMultiGoMesh_),
        afsName(afsName_),
        multiGoMeshType(multiGoMeshType_)
        {
            tprAssert( isMultiGoMesh_ );
        }

    //----- get -----//
    inline goSpeciesId_t   get_rootGoSpeciesId()const noexcept{ return this->rootGoSpeciesId; }
    inline bool         get_isMultiGoMesh()const noexcept{ return this->isMultiGoMesh; }
    inline const std::string &get_afsName()const noexcept{ return this->afsName; }
    inline AnimLabel get_animLabel()const noexcept{ return this->animLabel; }

    inline goLabelId_t get_multiGoMeshType()const noexcept{
        tprAssert( this->isMultiGoMesh );
        return this->multiGoMeshType;
    }


private:
    goSpeciesId_t              rootGoSpeciesId {};

    bool                    isMultiGoMesh;

    std::string             afsName    {};
    AnimLabel               animLabel {};
    goLabelId_t     multiGoMeshType {};
};


#endif 

