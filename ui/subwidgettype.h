#ifndef SUBWIDGETTYPE_H
#define SUBWIDGETTYPE_H

#include <QString>
#include <unordered_map>

namespace CheckMe::SubwidgetInfo {
    enum class Type;
    struct hash_Type;
}

namespace SubwidgetInfo{

    enum class Type{
        RECEIPT_IMAGE = 0,
        RECEIPT_NAME,
        ENUM_COUNT

    };

    struct hash_Type{
        std::size_t operator()(const Type& x) const { return static_cast<std::size_t>(x); }
    };


    using Container = std::unordered_map<Type, std::pair<bool, QString>, hash_Type>;


};




#endif // SUBWIDGETTYPE_H
