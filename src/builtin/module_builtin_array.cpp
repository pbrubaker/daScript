#include "daScript/misc/platform.h"

#include "module_builtin.h"

#include "daScript/simulate/simulate_nodes.h"
#include "daScript/ast/ast_interop.h"
#include "daScript/ast/ast_policy_types.h"

#include "daScript/simulate/aot_builtin.h"

namespace das {

    int builtin_array_size ( const Array & arr ) {
        return arr.size;
    }

    int builtin_array_capacity ( const Array & arr ) {
        return arr.capacity;
    }

    void builtin_array_resize ( Array & pArray, int newSize, int stride, Context * context ) {
        array_resize ( *context, pArray, newSize, stride, true );
    }

    void builtin_array_reserve ( Array & pArray, int newSize, int stride, Context * context ) {
        array_reserve( *context, pArray, newSize, stride );
    }

    int builtin_array_push ( Array & pArray, int index, int stride, Context * context ) {
        uint32_t idx = pArray.size;
        array_resize(*context, pArray, idx + 1, stride, false);
        if ( index >=0 ) {
            if ( uint32_t(index) >= pArray.size ) {
                context->throw_error_ex("insert index out of range, %u of %u", uint32_t(index), pArray.size);
                return 0;
            }
            memmove ( pArray.data+(index+1)*stride, pArray.data+index*stride, (idx-index)*stride );
            idx = index;
        }
        return idx;
    }

    void builtin_array_erase ( Array & pArray, int index, int stride, Context * context ) {
        if ( uint32_t(index) >= pArray.size ) {
            context->throw_error_ex("erase index out of range, %u of %u", uint32_t(index), pArray.size);
            return;
        }
        memmove ( pArray.data+index*stride, pArray.data+(index+1)*stride, (pArray.size-index-1)*stride );
        array_resize(*context, pArray, pArray.size-1, stride, false);
    }

    void builtin_array_clear ( Array & pArray, Context * context ) {
        array_clear(*context, pArray);
    }

    void builtin_array_lock ( const Array & arr, Context * context ) {
        array_lock(*context, const_cast<Array&>(arr));
    }

    void builtin_array_unlock ( const Array & arr, Context * context ) {
        array_unlock(*context, const_cast<Array&>(arr));
    }

    void Module_BuiltIn::addArrayTypes(ModuleLibrary & lib) {
        // array functions
        addExtern<DAS_BIND_FUN(builtin_array_clear)>(*this, lib, "clear", SideEffects::modifyArgument, "builtin_array_clear");
        addExtern<DAS_BIND_FUN(builtin_array_size)>(*this, lib, "length", SideEffects::none, "builtin_array_size");
        addExtern<DAS_BIND_FUN(builtin_array_capacity)>(*this, lib, "capacity", SideEffects::none, "builtin_array_capacity");
        // array built-in functions
        addExtern<DAS_BIND_FUN(builtin_array_resize)>(*this, lib, "__builtin_array_resize", SideEffects::modifyArgument, "builtin_array_resize");
        addExtern<DAS_BIND_FUN(builtin_array_reserve)>(*this, lib, "__builtin_array_reserve", SideEffects::modifyArgument, "builtin_array_reserve");
        addExtern<DAS_BIND_FUN(builtin_array_push)>(*this, lib, "__builtin_array_push", SideEffects::modifyArgument, "builtin_array_push");
        addExtern<DAS_BIND_FUN(builtin_array_erase)>(*this, lib, "__builtin_array_erase", SideEffects::modifyArgument, "builtin_array_erase");
        addExtern<DAS_BIND_FUN(builtin_array_lock)>(*this, lib, "__builtin_array_lock", SideEffects::modifyArgument, "builtin_array_lock");
        addExtern<DAS_BIND_FUN(builtin_array_unlock)>(*this, lib, "__builtin_array_unlock", SideEffects::modifyArgument, "builtin_array_unlock");
    }
}
