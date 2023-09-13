#pragma once
#include "core/common.h"

namespace cat::scripts
{
    class script_core : public core::utils::singleton<script_core>
    {
    public:
        script_core();
        ~script_core();

        void run(const char* name);

    private:
        std::string handle_pyerror();
    };
}